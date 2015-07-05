#include <string.h>
#include "jg-string.h"

/**
 * Strips double backslash comments from the given string
 *
 * Stripping is done by setting the first character of the // to '\0', so junk
 * data will exist after the string, and there will be some memory that has been
 * reserved but not allocated. For the bulk of strings, the amount of memory
 * wasted by this is minimal, as comments should not be very long.
 *
 * If the whole line is a comment, this function will return a string with only
 * a string termination character.
 *
 * @author Jonathan Gopel
 * @param  string String to check for comments
 * @return        String with comments removed
 */
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

/**
 * Removes leading and trailing spaces from a string
 *
 * This function is very close to a function from the Linux core. It is
 * transplanted here for the sake of portability.
 *
 * @author Jonathan Gopel
 * @param  string String to remove spaces from
 * @return        String without leading and trailing spaces
 */
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
