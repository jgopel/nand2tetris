#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"
#include "jg-files.h"

/**
 * Opens the file specified by the user via stdin
 *
 * @author Jonathan Gopel
 * @return Pointer to the opened file
 */
FILE *open_specified_file() {
	// Setup variables
	FILE *fp = NULL;
	char *string;

	// Iterate through loop until valid file input is given
	while ( fp == NULL ) {
		printf( "\nFile to use: " );
		string = read_line_io();
		fp = fopen( string, "r" );
	}

	return fp;
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

/**
 * Opens a file with a given extension and stores it in a given file pointer
 *
 * @author Jonathan Gopel
 * @param  filename       Name of the file to open
 * @param  file_extension Name of extension to try
 * @param  open_mode      Mode to open the file in
 * @param  fp             Pointer to store the result in
 * @return                Filename that was opened
 */
char* open_file_with_extension( char* const filename, char* const file_extension, char* const open_mode, FILE* * const fp ) {
	char* file_used = malloc( strlen( filename ) + 1 );
	strcpy( file_used, filename );

	*fp = fopen( file_used, open_mode );
	if ( *fp == NULL && ! strstr( file_used, file_extension ) ) {
		file_used = realloc( file_used, strlen( file_used ) + strlen( file_extension ) + 1 );
		strcat( file_used, file_extension );

		*fp = fopen( file_used, open_mode );
	}

	return file_used;
}
