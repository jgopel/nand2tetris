#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jg-input.h"
#include "jg-string.h"

#define FILE_EXTENSION ".vm"

int main( int argc, char *argv[] ) {
	char *filename;
	const char default_filename[] = "test";

	if ( argc < 2 ) {
		filename = malloc( strlen( default_filename ) + 1 );
		strcpy( filename, default_filename );
	} else {
		filename = malloc( strlen( argv[ 1 ] ) + 1 );
		strcpy( filename, argv[ 1 ] );
	}

	FILE *file_pointer = fopen( filename, "r" );
	if ( file_pointer == NULL && ! strstr( filename, FILE_EXTENSION ) ) {
		filename = realloc( filename, strlen( filename ) + strlen( FILE_EXTENSION ) + 1 );
		strcat( filename, FILE_EXTENSION );

		file_pointer = fopen( filename, "r" );
	}

	if ( file_pointer == NULL ) {
		printf( "%s does not exist.", filename );
		return 0;
	}

	return 0;
}
