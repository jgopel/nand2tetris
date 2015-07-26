#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"

/**
 * Reads a line from stdin
 *
 * Will strip newline and return from the end of the line to make the outputted
 * line clean.
 *
 * @return string
 */
char *read_line_io() {
	// Setup variables
	char *string = read_line( stdin );

	// Strip unwanted characters from end of string
	character_to_null( string, '\n' );
	character_to_null( string, '\r' );

	return string;
}

/**
 * Reads a line of arbitrary length from a filestream.
 *
 * There is no max lenght, but longer lines require more memory. The way this
 * function is setup, each time the looked at characters are not the end of the
 * line, the line length looked at doubled, so memory usage growth is
 * exponential.
 *
 * @param  stream Filestream to read line from
 * @return        string
 */
char *read_line( FILE *stream ) {
	// From http://stackoverflow.com/questions/4872898/string-input-and-output-in-c

	// Setup variables
	size_t size = 80; // Shortest string to look for
	size_t offset = 0;
	char *buffer = malloc( size );

	// Get string
	while ( fgets( buffer + offset, size - offset, stream ) ) {

		// Newline character is success condition
		if ( strchr( buffer + offset, '\n' ) ) {
			// Strip trailing newline
			buffer[ strlen( buffer ) ] = '\0';

			// Free memory
			char *output = malloc( strlen( buffer ) );
			strcpy( output, buffer );
			free( buffer );
			return output;
		}

		// Failure - no line end found: Reset variables and prepare for next pass
		offset = size - 1;                   // Demarkate memory already read
		size *= 2;                           // Increase string length to check
		char *tmp = realloc( buffer, size ); // Increase allocated memory

		if ( tmp == NULL ) {
			// Memory could not be allocated
			return buffer;
		}
		buffer = tmp;
	}

	// Fgets fails
	free( buffer );
	return '\0';
}

/**
 * Converts character to null
 *
 * @author Jonathan Gopel
 * @param  string    String to modify
 * @param  character Character to remove
 * @return           void
 */
void character_to_null( char *string, char character ) {
	// Setup variables
	char *position;

	// Remove character
	while ( ( position = strchr( string, character ) ) != NULL ) {
		*position = '\0';
	}
}
