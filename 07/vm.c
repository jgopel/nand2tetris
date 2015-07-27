#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-files.h"

#define FILE_EXTENSION ".vm"

char* open_file_from_args( const int, char* * const, FILE* * const );

int main( int argc, char* argv[] ) {
	FILE* file_pointer = NULL;
	char* filename = open_file_from_args( argc, argv, &file_pointer );

	if ( file_pointer == NULL ) {
		printf( "%s does not exist.", filename );
		return 0;
	} else {
		printf( "%s opened", filename );
	}

	fclose( file_pointer );
	return 0;
}

char* open_file_from_args( const int argument_count, char ** const argument_value, FILE ** const fp ) {
	char* filename;
	const char default_filename[] = "test";

	if ( argument_count < 2 ) {
		filename = malloc( strlen( default_filename ) + 1 );
		strcpy( filename, default_filename );
	} else {
		filename = malloc( strlen( argument_value[ 1 ] ) + 1 );
		strcpy( filename, argument_value[ 1 ] );
	}

	filename = open_file_with_extension( filename, FILE_EXTENSION, fp, "r" );

	return filename;
}
