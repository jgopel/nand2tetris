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
void clean_list( list_node_t** );
char *strip_comments( char* );
char *trim_spaces( char* );
void list_pop( list_node_t** );

int main( void ) {
	// TODO: Reenable this
	// FILE *file_pointer = open_specified_file();

	FILE *file_pointer = fopen( "add/Add.asm", "r" );

	// Split file to linked list
	list_node_t *head = calloc( 1, sizeof( list_node_t ) );
	build_list( head, file_pointer );
	fclose( file_pointer );

	// XXX: Debug only
	list_node_t *current = head;
	while( current != NULL ) {
		printf( "\n%s", current->assembler );
		current = current->next;
	}

	// Remove blank lines and comments
	clean_list( &head );

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

		// Setup node
		current->assembler = string;
		current->machine_language = 0b0;

		// Setup for next iteration
		string = read_line( fp );

		// Check for termination conditions
		if ( string == NULL ) {
			current->next = NULL;
			break;
		} else {
			current->next = malloc( sizeof( list_node_t ) );
			current = current->next;
		}
	}
}

void clean_list( list_node_t **head ) {
	// Setup variables
	list_node_t *current = *head;

	// Get a valid first item to work with
	while( current == *head ) {
		// Clean string
		current->assembler = strip_comments( current->assembler );
		current->assembler = trim_spaces( current->assembler );

		if( strlen( current->assembler ) < 1 ) {
			list_pop( head );
			current = *head;
		} else {
			current = current->next;
		}
	}


	// Check all list items
	list_node_t *next = current->next;
	while ( next != NULL ) {
		// Clean string
		next->assembler = strip_comments( next->assembler );
		next->assembler = trim_spaces( next->assembler );

		// Remove node if empty, otherwise advance one
		if ( strlen( next->assembler ) < 1 ) {
			free( next );
			current->next = current->next->next;
			next = current->next;
		} else {
			current = current->next;
			next = current->next;
		}
	}
}

char *strip_comments( char *string ) {
	// Setup variables
	const int length = strlen( string );

	// Make sure the string is sufficiently long
	if ( length < 2 ) {
		return string;
	}

	// Iterate through string
	for ( int i = 0; i < length - 1; i++ ) {
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

void list_pop( list_node_t **head ) {
	// Make current value is not last
	if ( head == NULL ) {
		return;
	}

	// Set head to head->next
	list_node_t *next = ( *head )->next;
	free( ( *head ) );
	( *head ) = next;
}
