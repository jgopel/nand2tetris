#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jg-input.h"
#include "jg-string.h"

typedef struct asm_node {
	char *assembly;
	unsigned int machine_code;
	struct asm_node *next;
} asm_node_t;
typedef struct sym_node {
	char *symbol;
	int memory_location;
	char offset;
	struct sym_node *next;
} sym_node_t;

void build_lists( asm_node_t*, sym_node_t*, FILE* );
void output_list( asm_node_t*, sym_node_t*, FILE* );
unsigned int find_symbol( sym_node_t*, char* );
int add_to_sym_list( sym_node_t*, char*, int, char, char );
void add_defaults( sym_node_t* );
unsigned int a_instruction( char* );
unsigned int c_instruction( char* );

int main( int argc, char *argv[] ) {
	// Setup variables
	char *filename;
	char default_file[] = "test2.asm";

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

	// Build linked list and symbol table
	asm_node_t *asm_head = calloc( 1, sizeof( asm_node_t ) );
	sym_node_t *sym_head = calloc( 1, sizeof( sym_node_t ) );
	build_lists( asm_head, sym_head, file_pointer );
	fclose( file_pointer );




	// XXX: DEBUG ONLY
	sym_node_t *current = sym_head;
	while ( current != NULL ) {
		printf( "\n%s\t%i", current->symbol, current->memory_location );
		current = current->next;
	}




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
	output_list( asm_head, sym_head, file_pointer );
	fclose( file_pointer );

	// TODO: Free memory

	// Normal exit
	return 0;
}

void build_lists( asm_node_t *head, sym_node_t *sym_head, FILE *fp ) {
	// Setup variables
	asm_node_t *current = head;
	char *string = read_line( fp );
	char *symbol;
	unsigned int line = 0;

	while ( string != NULL ) {
		// Data sanitization
		string = strip_comments( string );
		string = trim_spaces( string );

		// Handle jumps
		if ( string[ 0 ] == '(' ) {
			// Resize symbol
			symbol = malloc( strlen( string ) );
			strcpy( symbol, string + 1 );

			// Remove closing paren
			if ( symbol[ strlen( symbol ) - 1 ] == ')' ) {
				symbol[ strlen( symbol ) - 1 ] = '\0';
			}

			// Add to sym table
			add_to_sym_list( sym_head, symbol, line, 0, 1 );

		// Everything that's not a jump
		} else if ( strlen( string ) > 1 ) {

			// Check if current node is empty
			if ( current->assembly == NULL ) {
				current->assembly = string;

			// Create new node if needed
			} else {
				// Create and select node
				current->next = malloc( sizeof( asm_node_t ) );
				current = current->next;

				// Store value in new node
				current->assembly = string;
			}

			line++;
		}

		// Get next chunk
		string = read_line( fp );
	}

	// Ensure final next is set null
	current->next = NULL;
}

void output_list( asm_node_t *head, sym_node_t *sym_head, FILE *fp ) {
	// Setup variables
	asm_node_t *current = head;
	char string[ 16 ];

	while ( current != NULL ) {
		if ( current->assembly[ 0 ] == '@' ) {
			if ( ! isdigit( (int) current->assembly[ 1 ] ) ) {
				unsigned int memory_location = find_symbol( sym_head, current->assembly + 1 );
				sprintf( current->assembly, "@%d", memory_location );
			}
			current->machine_code = a_instruction( current->assembly );
		} else {
			current->machine_code = c_instruction( current->assembly );
		}

		// Generate binary
		int_to_binary_string( current->machine_code, string, 16 );

		// Prevent leading or trailing newline
		if ( current == head ) {
			fprintf( fp, "%s", string );
		} else {
			fprintf( fp, "\n%s", string );
		}

		// Go to next node
		current = current->next;
	}
}

unsigned int find_symbol( sym_node_t *head, char *string ) {
	// Setup variables
	sym_node_t *current = head;
	unsigned int output = 0;

	while ( current != NULL ) {
		// Check for termination condition
		if ( strcmp( current->symbol, string ) == 0 ) {
			break;
		}

		current = current->next;
	}

	// Set output
	output = current->memory_location;

	// Add offset if needed
	if ( current->offset == 1 ) {
		output += 16;
	}

	// Do output
	return output;
}

int add_to_sym_list( sym_node_t *head, char *string, int value, char offset, char do_update ) {
	sym_node_t *current = head;
	int updated = 0;

	// Check for last node or string equivalence
	while ( current->next != NULL && strcmp( current->symbol, string ) != 0 ) {
		// Do loop again
		current = current->next;
	}

	if ( current->symbol != NULL ) {
		// See if value is merely being updated
		if ( strcmp( current->symbol, string ) == 0 ) {
			updated = 1;
		}

		// Catch first node empty
		if ( updated == 0 ) {
			current->next = calloc( 1, sizeof( sym_node_t ) );
			current = current->next;
		}
	}

	if ( updated == 0 || ( updated == 1 && do_update == 1 ) ) {
		current->symbol = string;
		current->memory_location = value;
		current->offset = offset;
	}

	return updated;
}

void add_defaults( sym_node_t *head ) {
	// Setup default values
	// Registers
	add_to_sym_list( head, "R0", 0, 0, 0 );
	add_to_sym_list( head, "R1", 1, 0, 0 );
	add_to_sym_list( head, "R2", 2, 0, 0 );
	add_to_sym_list( head, "R3", 3, 0, 0 );
	add_to_sym_list( head, "R4", 4, 0, 0 );
	add_to_sym_list( head, "R5", 5, 0, 0 );
	add_to_sym_list( head, "R6", 6, 0, 0 );
	add_to_sym_list( head, "R7", 7, 0, 0 );
	add_to_sym_list( head, "R8", 8, 0, 0 );
	add_to_sym_list( head, "R9", 9, 0, 0 );
	add_to_sym_list( head, "R10", 10, 0, 0 );
	add_to_sym_list( head, "R11", 11, 0, 0 );
	add_to_sym_list( head, "R12", 12, 0, 0 );
	add_to_sym_list( head, "R13", 13, 0, 0 );
	add_to_sym_list( head, "R14", 14, 0, 0 );
	add_to_sym_list( head, "R15", 15, 0, 0 );
	// Keywords
	add_to_sym_list( head, "SP", 0, 0, 0 );
	add_to_sym_list( head, "LCL", 1, 0, 0 );
	add_to_sym_list( head, "ARG", 2, 0, 0 );
	add_to_sym_list( head, "THIS", 3, 0, 0 );
	add_to_sym_list( head, "THAT", 4, 0, 0 );
	// Memory maps
	add_to_sym_list( head, "SCREEN", 0x4000, 0, 0 );
	add_to_sym_list( head, "KBD", 0x6000, 0, 0 );
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
	if ( assembly[ 0 ] == '@' ) {
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
