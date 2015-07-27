#ifndef JG_FILES_H_ // Prevent including twice
#define JG_FILES_H_

FILE* open_specified_file();
unsigned int count_lines( FILE* );
char* open_file_with_extension( char*, char* const, char* const, FILE* * const );

#endif
