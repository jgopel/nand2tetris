#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"

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
			buffer[ strlen( buffer ) - 1 ] = '\0';

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
