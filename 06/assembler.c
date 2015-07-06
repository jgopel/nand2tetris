#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"
#include "jg-files.h"
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

int main( void ) {
	// TODO: Reenable this
	// FILE *file_pointer = open_specified_file();

	FILE *file_pointer = fopen( "MaxL-mod.asm", "r" );

	// Split file to linked list
	list_node_t *head = malloc( sizeof( list_node_t ) );
	build_list( head, file_pointer );
	fclose( file_pointer );

	// Iterate through array and replace array elements with their equivalent machine language code
	generate_machine_code( head );

	// Output array to file

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
		}

		string = read_line( fp );
	}
	current->next = NULL;
}

/**
 * Generates machine code for an entire linked list
 *
 * @author Jonathan Gopel
 * @param head First node of the linked list
 */
void generate_machine_code( list_node_t *head ) {
	// A Instruction: 0vvv vvvv vvvv vvvv
	// C Instruction: 111a cccc ccdd djjj

	// Setup variables
	list_node_t *current = head;

	// Iterate through all nodes
	while( current != NULL ) {
		if( current->assembly[ 0 ] == '@' ) {
			current->machine_code = a_instruction( current->assembly );
		} else {
			current->machine_code = c_instruction( current->assembly );
		}

		// Setup for next iteration
		current = current->next;
	}
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
