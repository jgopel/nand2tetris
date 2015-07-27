#ifndef JG_STRING_H_ // Prevent including twice
#define JG_STRING_H_

char *strip_comments( char* );
char *trim_spaces( char* );
void int_to_binary_string( unsigned int, char*, int );
char* replace_extension( char* const, char* const );

#endif
