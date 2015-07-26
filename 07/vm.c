#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"
#include "jg-string.h"

#define FILE_EXTENSION ".vm"

char *open_file( int, char**, FILE** );

int main( int argc, char *argv[] ) {
	FILE *file_pointer = NULL;
	char *filename = open_file( argc, argv, &file_pointer );

	if ( file_pointer == NULL ) {
		printf( "%s does not exist.", filename );
		return 0;
	} else {
		printf( "%s opened", filename );
	}

	fclose( file_pointer );
	return 0;
}

char *open_file( int argument_count, char **argument_value, FILE **fp ) {
	char *filename;
	const char default_filename[] = "test";

	if ( argument_count < 2 ) {
		filename = malloc( strlen( default_filename ) + 1 );
		strcpy( filename, default_filename );
	} else {
		filename = malloc( strlen( argument_value[ 1 ] ) + 1 );
		strcpy( filename, argument_value[ 1 ] );
	}

	*fp = fopen( filename, "r" );
	if ( *fp == NULL && ! strstr( filename, FILE_EXTENSION ) ) {
		filename = realloc( filename, strlen( filename ) + strlen( FILE_EXTENSION ) + 1 );
		strcat( filename, FILE_EXTENSION );

		*fp = fopen( filename, "r" );
	}

	return filename;
}
