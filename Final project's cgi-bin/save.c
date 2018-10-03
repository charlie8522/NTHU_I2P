#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

typedef struct
    {
        int year;
        int month;
        int date;
        char id[1000];

    } Struct;



void save(char *query)
{

    Struct struct1;
    // parse query
	char *seek = query ;
	while( *seek )
    {
		// get each (key, value) pair
		char *key, *value ;
		seek = parse_query( seek, &key, &value ) ;

        int val;

		// if key is "lastpage"
		if( strcmp(key, "year") == 0 )
        {
            sscanf(value, "%d",&val);
			struct1.year = val;
        }
        else if( strcmp(key, "month") == 0 )
        {
            sscanf(value, "%d",&val);
			struct1.month = val;
        }
        else if( strcmp(key, "date") == 0 )
        {
            sscanf(value, "%d",&val);
			struct1.date = val;
        }
        else if( strcmp(key, "id") == 0 )
        {
            sscanf(value, "%s", struct1.id);
        }

	}

	FILE *fout = fopen( "save_db.txt", "a" ) ;
	fprintf( fout, "%d\t%d\t%d\t%s\t\n", struct1.year,struct1.month,struct1.date,struct1.id) ;
	fclose( fout ) ;

}
int main(){
	int query_len = atoi( getenv("CONTENT_LENGTH") ) ;
	char *query = (char*) malloc( query_len ) ;
	memset( query, 0, query_len ) ;

	char scanf_format[8] ;
	sprintf( scanf_format, "%%%dc", query_len ) ;
	scanf( scanf_format, query ) ;

//	printf("%s",query);
    save(query) ;
    printf("<html><meta http-equiv=\"refresh\" content=\"0; url=/cgi-bin/index.exe\" />\n</html>\n") ;
	return 0 ;
}
