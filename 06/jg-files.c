#include <stdio.h>
#include "jg-input.h"
#include "jg-files.h"

void open_specified_file( FILE *fp ) {
	// Setup variables
	char *string;

	// Iterate through loop until valid file input is given
	while ( fp == NULL ) {
		printf( "\nFile to use: " );
		string = read_line( stdin );
		fp = fopen( string, "r" );
	}
}

/**
 * Counts the number of lines in a given file
 *
 * @author Jonathan Gopel
 * @param  fp Pointer to the file whose lines are to be counted
 * @return    Unsigned integer with the count of lines. Count is assumed to
 *            start at 1.
 */
unsigned int count_lines( FILE *fp ) {
	// Setup variables
	unsigned int count = 1;
	char character;

	// Iterate through characters
	while ( ( character = fgetc( fp ) ) ) {
		// Termination condition
		if ( character == EOF ) {
			break;
		}

		// Count characters
		if ( character == '\n' ) {
			count++;
		}
	}

	// Output
	return count;
}
