#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"
#include "jg-files.h"
#include "jg-string.h"

typedef struct list_node {
	char *assembler;
	unsigned int machine_language;
	struct list_node *next;
} list_node_t;

void build_list( list_node_t*, FILE* );

int main( void ) {
	// TODO: Reenable this
	// FILE *file_pointer = open_specified_file();

	FILE *file_pointer = fopen( "add/Add.asm", "r" );

	// Split file to linked list
	list_node_t *head = malloc( sizeof( list_node_t ) );
	build_list( head, file_pointer );
	fclose( file_pointer );

	// XXX: Debug only
	list_node_t *current = head;
	while( current != NULL ) {
		printf( "\n%s", current->assembler );
		current = current->next;
	}

	// Iterate through array and replace array elements with their equivalent machine language code

	// Output array to file

	// Normal exit
	return 0;
}

void build_list( list_node_t *current, FILE *fp ) {
	// Setup variables
	char *string = read_line( fp );

	while( string != NULL ) {
		// Clean up trailing characters
		character_to_null( string, '\n' );
		character_to_null( string, '\r' );

		// Remove comments and strip spaces
		string = strip_comments( string );
		string = trim_spaces( string );

		if ( strlen( string ) > 1 ) {
			// Store string in current node if current node is unused (mostly just first instance)
			if ( current->assembler == NULL ) {
				current->assembler = string;
				current->machine_language = 0b0;
			// Otherwise create node for the string to go into and store it there
			} else {
				current->next = malloc( sizeof( list_node_t ) );
				current = current->next;

				current->assembler = string;
				current->machine_language = 0b0;
			}
		}

		string = read_line( fp );
	}
	current->next = NULL;
}
