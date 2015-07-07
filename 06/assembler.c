#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"
#include "jg-string.h"

typedef struct list_node {
	char *assembly;
	unsigned int machine_code;
	struct list_node *next;
} list_node_t;

void build_list( list_node_t*, FILE* );
void generate_machine_code( list_node_t* );
unsigned int a_instruction( char* );
unsigned int c_instruction( char* );
void generate_file( list_node_t*, FILE* );

int main( int argc, char *argv[] ) {
	// Setup variables
	char *filename;
	char default_file[] = "test.asm";

	// Set file name
	if ( argc < 2 ) {
		// Add one for \0 character
		filename = malloc( strlen( default_file ) + 1 );
		strcpy( filename, default_file );
	} else {
		// Add one for \0 character
		filename = malloc( strlen( argv[ 1 ] + 1 ) );
		strcpy( filename, argv[ 1 ] );
	}

	// Open file
	FILE *file_pointer = fopen( filename, "r" );
	if ( file_pointer == NULL && strstr( filename, ".asm" ) == NULL ) {
		// Lengthen string for concatenation
		filename = realloc( filename, strlen( filename ) + 5 );
		strcat( filename, ".asm" );

		// Retry fopen
		file_pointer = fopen( filename, "r" );
	}

	// Check that a file was opened
	if ( file_pointer == NULL ) {
		// File could not be opened
		printf( "\n%s does not exist.", filename );
		return 0;
	}

	// Split file to linked list
	list_node_t *head = malloc( sizeof( list_node_t ) );
	build_list( head, file_pointer );
	fclose( file_pointer );

	// Remove .asm extension if it exists
	char *dot_location = strrchr( filename, '.' );
	if ( dot_location != NULL ) {
		*dot_location = '\0';
	}

	// Add .hack extension
	filename = realloc( filename, strlen( filename ) + 6 );
	filename = strcat( filename, ".hack" );

	// Create file for output
	file_pointer = fopen( filename, "w" );
	if ( file_pointer == NULL ) {
		printf( "\n%s could not be created", filename );
	}
	free( filename );

	// Output to file
	generate_file( head, file_pointer );

	// Close file
	fclose( file_pointer );

	// Normal exit
	return 0;
}

/**
 * Creates a linked list from a file of Hack assembly
 *
 * Removes all comment-only and blank lines, generating a linked list with only
 * lines containing commands. Each line corresponds to a single element of the
 * list. The machine_code variable may contain junk, it must be set
 * independently. Final node has node->next == NULL.
 *
 * @author Jonathan Gopel
 * @param current Current element of the linked list
 * @param fp      Assembly file
 */
void build_list( list_node_t *head, FILE *fp ) {
	// Setup variables
	list_node_t *current = head;
	char *string = read_line( fp );

	while ( string != NULL ) {
		// Remove comments and strip spaces
		string = strip_comments( string );
		string = trim_spaces( string );

		if ( strlen( string ) > 1 ) {
			// Store string in current node if current node is unused (mostly just first instance)
			if ( current->assembly == NULL ) {
				current->assembly = string;

			// Otherwise create node for the string to go into and store it there
			} else {
				// Create node
				current->next = malloc( sizeof( list_node_t ) );
				current = current->next;

				// Store value in new node
				current->assembly = string;
			}

			if ( current->assembly[ 0 ] == '@' ) {
				current->machine_code = a_instruction( current->assembly );
			} else {
				current->machine_code = c_instruction( current->assembly );
			}
		}

		string = read_line( fp );
	}
	current->next = NULL;
}

/**
 * Generates machine code for the A instruction
 *
 * @author Jonathan Gopel
 * @param  assembly String to turn into machine code
 * @return          Machine code
 */
unsigned int a_instruction( char *assembly ) {
	// A Instruction: 0vvv vvvv vvvv vvvv

	// Setup variables
	unsigned int output = 0b0;

	// Remove leading @
	if ( assembly[0] == '@' ) {
		assembly++;
	}

	// Convert string to int
	unsigned int _assembly = (unsigned int) strtol( assembly, NULL, 10 );

	// Set output bits
	output |= 0b0 << 15;
	output |= _assembly << 0;

	return output;
}

/**
 * Generates machine code for the C instruction
 *
 * @author Jonathan Gopel
 * @param  assembly String to turn into machine code
 * @return          Machine code
 */
unsigned int c_instruction( char *assembly ) {
	// C Instruction: 111a cccc ccdd djjj

	// Setup variables
	unsigned int output = 0b0;
	char *command = NULL;
	char *destination = NULL;
	char *jump = NULL;
	unsigned int _a = 0b0;
	unsigned int _command = 0b0;
	unsigned int _destination = 0b0;
	unsigned int _jump = 0b0;

	if ( strchr( assembly, ';' ) != NULL ) {
		command = strtok( assembly, ";" );
		jump = strtok( NULL, ";" );
	}
	if ( strchr( assembly, '=' ) != NULL ) {
		destination = strtok( assembly, "=" );
		command = strtok( NULL, "=" );
	}

	// Set a (12th) bit
	char *m_position = strchr( command, 'M' );
	if ( m_position != NULL ) {
		_a = 0b1;

		// Replace M with A for command switch
		*m_position = 'A';
	}

	// Set command bits
	if ( strcmp( command, "0" ) == 0 ) {
		_command = 0b101010;
	} else if ( strcmp( command, "1" ) == 0 ) {
		_command = 0b111111;
	} else if ( strcmp( command, "-1" ) == 0 ) {
		_command = 0b111010;
	} else if ( strcmp( command, "D" ) == 0 ) {
		_command = 0b001100;
	} else if ( strcmp( command, "A" ) == 0 ) {
		_command = 0b110000;
	} else if ( strcmp( command, "!D" ) == 0 ) {
		_command = 0b001101;
	} else if ( strcmp( command, "!A" ) == 0 ) {
		_command = 0b110001;
	} else if ( strcmp( command, "-D" ) == 0 ) {
		_command = 0b001111;
	} else if ( strcmp( command, "-A" ) == 0 ) {
		_command = 0b110011;
	} else if ( strcmp( command, "D+1" ) == 0 ) {
		_command = 0b011111;
	} else if ( strcmp( command, "A+1" ) == 0 ) {
		_command = 0b110111;
	} else if ( strcmp( command, "D-1" ) == 0 ) {
		_command = 0b001110;
	} else if ( strcmp( command, "A-1" ) == 0 ) {
		_command = 0b110010;
	} else if ( strcmp( command, "D+A" ) == 0 ) {
		_command = 0b000010;
	} else if ( strcmp( command, "D-A" ) == 0 ) {
		_command = 0b010011;
	} else if ( strcmp( command, "A-D" ) == 0 ) {
		_command = 0b000111;
	} else if ( strcmp( command, "D&A" ) == 0 ) {
		_command = 0b000000;
	} else if ( strcmp( command, "D|A" ) == 0 ) {
		_command = 0b010101;
	} else {
		_command = 0b101010;
	}

	// Set destination bits
	if ( destination != NULL ) {
		if ( strchr( destination, 'A' ) != NULL ) {
			_destination |= 1 << 2;
		}
		if ( strchr( destination, 'D' ) != NULL ) {
			_destination |= 1 << 1;
		}
		if ( strchr( destination, 'M' ) != NULL ) {
			_destination |= 1 << 0;
		}
	}

	// Set jump bits
	if ( jump != NULL ) {
		if ( strcmp( jump, "JGT" ) == 0 ) {
			_jump = 0b001;
		} else if ( strcmp( jump, "JEQ" ) == 0 ) {
			_jump = 0b010;
		} else if ( strcmp( jump, "JGE" ) == 0 ) {
			_jump = 0b011;
		} else if ( strcmp( jump, "JLT" ) == 0 ) {
			_jump = 0b100;
		} else if ( strcmp( jump, "JNE" ) == 0 ) {
			_jump = 0b101;
		} else if ( strcmp( jump, "JLE" ) == 0 ) {
			_jump = 0b110;
		} else if ( strcmp( jump, "JMP" ) == 0 ) {
			_jump = 0b111;
		} else {
			_jump = 0b000;
		}
	}

	// Build output string
	output |= 0b111 << 13;
	output |= _a << 12;
	output |= _command << 6;
	output |= _destination << 3;
	output |= _jump << 0;

	return output;
}

/**
 * Outputs machine code to given file
 *
 * @author Jonathan Gopel
 * @param head First element of the linked list
 * @param fp   Pointer to the output file
 */
void generate_file( list_node_t *head, FILE *fp ) {
	// Setup variables
	list_node_t *current = head;
	char string[ 16 ];

	while ( current != NULL ) {
		int_to_binary_string( current->machine_code, string, 16 );

		// Prevent leading or trailing newline
		if( current == head ) {
			fprintf( fp, "%s", string );
		} else {
			fprintf( fp, "\n%s", string );
		}

		// Go to next node
		current = current->next;
	}
}
