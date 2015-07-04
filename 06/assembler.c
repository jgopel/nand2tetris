#include <stdio.h>
#include <stdlib.h>
#include "jg-input.h"
#include "jg-files.h"

typedef struct list_node {
	char *assembler;
	unsigned int machine_language;
	struct list_node *next;
} list_node_t;

void build_list( list_node_t*, FILE* );
void add_char_to_string( char*, char );

int main( void ) {
	// TODO: Reenable this
	// FILE *file_pointer = open_specified_file();

	FILE *file_pointer = fopen( "add/Add.asm", "r" );

	// Split file to linked list
	list_node_t *head = malloc( sizeof( list_node_t ) );
	build_list( head, file_pointer );
	fclose( file_pointer );

	// Iterate through array and replace array elements with their equivalent machine language code

	// Output array to file

	// Normal exit
	return 0;
}

void build_list( list_node_t *current, FILE *fp ) {
	// Set first line
	char *string = read_line( fp );

	// Iterate through the file
	while( 1 ) {
		// Clean up any trailing characters
		character_to_null( string, '\n' );
		character_to_null( string, '\r' );

		// Store string
		current->assembler = string;

		// Setup for next iteration
		string = read_line( fp );

		// Check for termination conditions
		if ( string == NULL ) {
			break;
		} else {
			current->next = malloc( sizeof( list_node_t ) );
			current = current->next;
		}
	}
}
