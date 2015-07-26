#include <string.h>
#include <ctype.h>
#include <math.h>
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
 * transplanted here for the sake of portability. Because of the use of
 * isspace(), this function is capable of trimming elements that are not just
 * spaces (including tabs and newlines).
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
	while ( end >= string && isspace( (unsigned char) *end ) ) {
		end--;
	}
	*( end + 1 ) = '\0';

	while ( *string && isspace( (unsigned char) *string ) ) {
		string++;
	}

	return string;
}

/**
 * Turns an integer into a binary string
 *
 * Number of bits should be equal to the size of output.
 *
 * @author Jonathan Gopel
 * @param input          Integer
 * @param output         String to store binary in
 * @param number_of_bits Number of leading 0s to use
 */
void int_to_binary_string( unsigned int input, char* output, int number_of_bits ) {
	// Setup variables
	int value;
	int element;

	// Make sure value will fit in output
	if ( input >= pow( 2, number_of_bits ) ) {
		return;
	}

	// Set null terminator
	output[ number_of_bits ] = '\0';

	// Iterate through number
	for ( int i = ( number_of_bits - 1 ); i >= 0; --i ) {
		// Set value for current iteration
		value = input >> i;

		// Calculate array element to change
		element = number_of_bits - i - 1;

		// Add value to array
		output[ element ] = ( value & 1 ) + '0';
	}
}
