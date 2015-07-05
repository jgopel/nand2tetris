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

	FILE *file_pointer = fopen( "add/Add.asm", "r" );

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
 * Generates a linked list from a file of Hack assembly
 *
 * Removes all comment-only and blank lines, generating a linked list with only
 * lines containing commands. Each line corresponds to a single element of the
 * list. All machine code values are forced to 0b0 on initization of the list
 * node. Final node has node->next == NULL.
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
				current->machine_code = 0b0;

			// Otherwise create node for the string to go into and store it there
			} else {
				// Create node
				current->next = malloc( sizeof( list_node_t ) );
				current = current->next;

				// Store value in new node
				current->assembly = string;
				current->machine_code = 0b0;
			}
		}

		string = read_line( fp );
	}
	current->next = NULL;
}

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

unsigned int a_instruction( char *assembly ) {
	return 0;
}
unsigned int c_instruction( char *assembly ) {
	return 0;
}
