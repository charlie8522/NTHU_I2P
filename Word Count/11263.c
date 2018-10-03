#include "11263.h"
#include <stdio.h>

// fetch words from stdin
const char *fetch_word(){
	static char skip_symbol[] = " \t\n,.;:?()[]{}\"\"''" ;

	static char line_buffer[1024] ;
	static char *now = NULL ;

	// try to read a line from stdin
	for( ;; ){
		if( now == NULL)
			now = fgets( line_buffer, sizeof(line_buffer), stdin ) ;

		// End Of File?
		if( now == NULL )
			return NULL ;

		// skip symbols
		for( ; *now ; now++ ){
			int size = sizeof( skip_symbol ) ;
			int i ;
			for( i=0 ; i<size ; i++ ){
				if( *now == skip_symbol[i] )
					break ;
			}

			// if not match skip_symbol[]
			if( i >= size )
				break ;
		}

		// End Of Line?
		if( *now == '\0' ){
			now = NULL ;
			continue ;
		}

		char *word = now ;

		for( ; *now ; now++ ){
			int size = sizeof( skip_symbol ) ;
			int i ;
			for( i=0 ; i<size ; i++ ){
				if( *now == skip_symbol[i] )
					break ;
			}

			// if match skip_symbol[]
			if( i < size )
				break ;
		}

		if( *now ){
			*now = '\0' ;
			now ++ ;
		}

		return word ;
	}

	return NULL ;
}

