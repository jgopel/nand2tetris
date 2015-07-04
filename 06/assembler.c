#include <stdio.h>
#include <stdlib.h>
#include "jg-input.h"
#include "jg-files.h"

typedef struct list_node {
	char *command;
	unsigned int machine_language;
	struct list_node *next;
} list_node_t;

void build_list( list_node_t*, FILE* );
void add_char_to_string( char*, char );

int main( void ) {
	// TODO: Reenable this
	// FILE *file_pointer = open_specified_file();

	file_pointer = fopen( "add/Add.asm", "r" );

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
	char *string = read_line( fp );
	while( string != NULL ) {
		printf( "%s", string );
		string = read_line( fp );
	}
}
