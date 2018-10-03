#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_db.h"
#include "utility.h"


int get_del_id(){
	int id = -1 ;

	// get query
	int query_len = atoi( getenv("CONTENT_LENGTH") ) ;
	char *query = (char*) malloc( query_len ) ;
	memset( query, 0, query_len ) ;

	char scanf_format[8] ;
	sprintf( scanf_format, "%%%dc", query_len ) ;
	scanf( scanf_format, query ) ;

	// print query
	printf( "%s=\"%s\"<br><br>\n", "QUERY_STRING", query ) ;

	// print query detail
	printf( "query_len: %d<br>", query_len ) ;
	for( int i=0 ; i<query_len ; i++ )
		printf( "ascii of query[%d]: %d<br>", i, (unsigned)query[i] ) ;

	// parse query
	char *seek = query ;
	while( *seek ){
		char *key, *value ;
		seek = parse_query( seek, &key, &value ) ;

		if( !strcmp( key, "id" ) ){
			sscanf( value, "%d", &id ) ;
			break ;
		}
	}

	free( query ) ;
	return id ;
}


void web_out(){
	printf("<html><meta http-equiv=\"refresh\" content=\"15; url=/cgi-bin/index.exe?lastpage=del_refresh\" />\n<body>\n") ;

	int id = get_del_id() ;
	db_del( id ) ;

	printf("<br><b>DELETE MESSAGE #%d.</b><br>", id) ;
	printf( "<br><a href=\"/cgi-bin/index.exe?lastpage=del_hyperlink\">Redirect to index.exe</a>, or wait for 15 seconds.<br>\n" ) ;

	printf("</body></html>\n") ;
}


int main(){
	web_out() ;

	return 0 ;
}
