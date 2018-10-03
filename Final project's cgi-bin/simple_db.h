#ifndef __SIMPLE_DB_H__
#define __SIMPLE_DB_H__


#include <stdio.h>
#include <stdlib.h>


#define DB		"db.txt"
#define	TMP		"tmp.txt"

#define MAX_RECORD_LEN	100000


int compare(const void *a, const void *b){
    char *x = *(char **)a;
    char *y = *(char **)b;
    int i, j;

    char to_compare[] = "msg:\"";
    int com_len = strlen(to_compare);

    for(i = 0; strncmp(to_compare, x + i, com_len) && i < strlen(x); i++);
    for(j = 0; strncmp(to_compare, y + j, com_len) && j < strlen(y); j++);
    return strcmp(x + i + com_len, y + j + com_len);
}

int db_show(){
    int MAX_LINES_COUNT = 1000; //最多可以有的字串數量

    char *lines[MAX_LINES_COUNT];
    int count = 0; // 目前的字串數量
    FILE *fin = fopen( DB, "r" );
    //輸入所有的database
    lines[0] = (char *)malloc(MAX_RECORD_LEN * sizeof(char));
    for(; fgets(lines[count], MAX_RECORD_LEN, fin); count++){


        lines[count+1] = (char *)malloc(MAX_RECORD_LEN * sizeof(char));
    }

    qsort(lines, count, sizeof(char *), compare);
    //lines[0] = lines[1];

    for(int i = 0; i < count; i++){
        printf( "#%d: %s<br>", i, lines[i] ) ;
    }

    fclose(fin);
    return count ;
}

void db_append( const char *record ){
	FILE *fout = fopen( DB, "a" ) ;
	fprintf( fout, "%s\n", record ) ;
	fclose( fout ) ;
}


void db_clear(){
	remove( DB ) ;
}


void db_del(int id){
	FILE *fin = fopen( DB, "r" ) ;
	if( fin ){
		FILE *fout = fopen( TMP, "w" ) ;

		char line[MAX_RECORD_LEN] ;
		for( int i=0 ; fgets(line, sizeof(line), fin ) ; i++ ){
			if( i != id )
				fprintf( fout, "%s", line ) ;
		}

		fclose( fin ) ;
		fclose( fout ) ;

		remove( DB ) ;
		rename( TMP, DB ) ;
	}
}

#endif
