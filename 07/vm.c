#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-files.h"
#include "jg-string.h"

#define INPUT_EXTENSION ".vm"
#define OUTPUT_EXTENSION ".asm"

char* open_file_from_args( const int, char* * const, FILE* * const );

int main( int argc, char* argv[] ) {
	FILE* file_pointer = NULL;
	char* input_filename = open_file_from_args( argc, argv, &file_pointer );

	if ( file_pointer == NULL ) {
		printf( "%s does not exist.\n", input_filename );
		return 0;
	} else {
		printf( "%s opened\n", input_filename );
	}

	fclose( file_pointer );

	char* output_filename = replace_extension( input_filename, OUTPUT_EXTENSION );
	free( input_filename );
	file_pointer = fopen( output_filename, "w" );

	if ( file_pointer == NULL ) {
		printf( "%s could not be created\n", output_filename );
	} else {
		printf( "%s created\n", output_filename );
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

	char* output = open_file_with_extension( filename, INPUT_EXTENSION, "r", fp );
	free( filename );

	return output;
}
