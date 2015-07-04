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
	// Setup variables
	FILE *file_pointer = NULL;
	char *string;

	// Get to read
	// open_specified_file( file_pointer );
	while ( file_pointer == NULL ) {
		printf( "\nFile to use: " );
		string = read_line();
		file_pointer = fopen( string, "r" );
	}

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
	// Setup variables
	size_t length = 0;

	while( ( getline( &current->command, &length, fp ) ) != -1 ) {
		current = current->next;
	}
}
