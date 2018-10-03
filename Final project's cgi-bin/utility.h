#ifndef __UTILITY_H__
#define __UTILITY_H__


char *parse_query( char *seek, char **key, char **value ){
	*key = seek ;
	for( ; *seek && *seek!='=' ; seek++ ) ;
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	*value = seek ;
	for( ; *seek && /* *seek!='&' &&*/ *seek!=10 ; seek++ ) ;
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	return seek ;
}


void gen_token( char *token, int len_token, const char *user, const char *pass ){
	int i;
	for( i=0 ; i<len_token ; ){
		token[i++] = *(user++) ;
		if( i<len_token )
			token[i++] = *(pass++) ;
	}
	token[len_token-1] = '\0' ;
}
#endif
