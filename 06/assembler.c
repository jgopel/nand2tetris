#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jg-input.h"
#include "jg-files.h"

typedef struct list_node {
	char *assembler;
	unsigned int machine_language;
	struct list_node *next;
} list_node_t;

void build_list( list_node_t*, FILE* );
void clean_list( list_node_t* );
char *strip_comments( char* );
char *trim_spaces( char* );

int main( void ) {
	// TODO: Reenable this
	// FILE *file_pointer = open_specified_file();

	FILE *file_pointer = fopen( "add/Add.asm", "r" );

	// Split file to linked list
	list_node_t *head = malloc( sizeof( list_node_t ) );
	build_list( head, file_pointer );
	fclose( file_pointer );

	// Remove blank lines and comments
	clean_list( head );

	// Iterate through array and replace array elements with their equivalent machine language code

	// Output array to file

	// Normal exit
	return 0;
}

void build_list( list_node_t *current, FILE *fp ) {
	// Set first line
	char *string = read_line( fp );

	// Iterate through the file
	while ( 1 ) {
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

void clean_list( list_node_t *current ) {
	// char *string = "   I should remain //I should go";

	printf( "\n1: %s::", current->assembler );

	current->assembler = strip_comments( current->assembler );
	printf( "\n2: %s::", current->assembler );

	current->assembler = trim_spaces( current->assembler );
	printf( "\n3: %s::", current->assembler );
}

char *strip_comments( char *string ) {
	// Iterate through string
	for ( int i = 0; i < strlen( string ) - 1; i++ ) {
		// Check current and next character for /
		if ( string[ i ] == '/' && string[ i + 1 ] == '/' ) {
			// If both are, make current character the end of the string
			string[ i ] = '\0';
			return string;
		}
	}

	return string;
}
char *trim_spaces( char *string ) {
	// Mostly from linux kernel

	// Setup variables
	size_t size;
	char *end;
	size = strlen( string );

	if ( ! size )
		return string;

	end = string + size - 1;
	while ( end >= string && isspace( *end ) ) {
		end--;
	}
	*( end + 1 ) = '\0';

	while ( *string && isspace( *string ) ) {
		string++;
	}

	return string;
}
