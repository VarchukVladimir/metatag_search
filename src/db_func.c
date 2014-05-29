/*
 * db_func.c
 *
 *  Created on: May 20, 2014
 *      Author: Volodymyr Varchuk
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include "db_func.h"

#undef TEST

int db_open ( sqlite3 **db, char *db_file_name )
{
	int result = 0;
	result = sqlite3_open( db_file_name, db );
	if ( result != SQLITE_OK )
	{
		databaseError( *db );
		return -1;
	}
	else
	{
		/*enable extended sqlite error code*/
		int result = sqlite3_extended_result_codes( *db, 1 );
		if ( result )
		{
			databaseError( *db );
			return -1;
		}

		/*disable journaling*/
		result = sqlite_pragma( *db, "PRAGMA journal_mode=MEMORY;" );
		if ( result )
		{
			databaseError( *db );
			return -1;
		}
		/*disable using of synchronisation, because it's not supported by ZRT FS*/
		result = sqlite_pragma( *db, "PRAGMA synchronous=OFF;" );
		if ( result )
		{
			databaseError( *db );
			return -1;
		}
		/*exclusive access to DB by single process*/
		result = sqlite_pragma( *db, "PRAGMA locking_mode=EXCLUSIVE;" );
		if ( result )
		{
			databaseError( *db );
			return -1;
		}
	}
	return result;
}

int exec_SQL ( sqlite3* db, const char* SQL )
{
	sqlite3_stmt *pStmt = 0;
	int result = 0;
	int i = 0;

	if ( !SQL )
		return -1;
#ifdef TEST
	printf ( "exec SQL %s\n", SQL );
#endif
	result = sqlite3_prepare( db, SQL, -1, &pStmt, 0 );
	if ( result == SQLITE_OK && pStmt != NULL )
	{
		while(( result = sqlite3_step( pStmt ) ) == SQLITE_ROW)
		{
			for( i = 0; i < sqlite3_column_count( pStmt ); ++i )
				print_col( pStmt, i );
			printf( "\n" );
		}
		result = sqlite3_finalize( pStmt );
	}

	return result;
}

int sqlite_pragma ( sqlite3* db, const char* request )
{
	// get current database version of schema
	static sqlite3_stmt *stmt_pragma;
	int rc = 0;

	if ( ( rc = sqlite3_prepare_v2( db, request, -1, &stmt_pragma, NULL ) ) == SQLITE_OK )
	{
		while(sqlite3_step( stmt_pragma ) == SQLITE_ROW)
			;
	}
	else
	{
		rc = sqlite3_errcode( db );
	}
	sqlite3_finalize( stmt_pragma );
	return rc;
}

int get_tables_names ( sqlite3 *db, s_list *list )
{
	int table_count = 0;
	int result = 0;
	char * SQL = "SELECT name FROM sqlite_master WHERE type='table'";
	sqlite3_stmt *pStmt;
	do
	{
		result = sqlite3_prepare( db, SQL, -1, &pStmt, 0 );
		if ( result != SQLITE_OK )
		{
			return result;
		}
		while(( result = sqlite3_step( pStmt ) ) == SQLITE_ROW)
		{
			add_to_list( sqlite3_column_text( pStmt, 0 ), list );
		}
		result = sqlite3_finalize( pStmt );
	} while(result == SQLITE_SCHEMA);
	return table_count;
}

s_list *get_fields_names ( sqlite3 *db, char *table_name )
{
	s_list *list = NULL;

	int table_count = 0;
	int i = 0;
	int result = 0;
	char *SQL_ = "PRAGMA table_info(%s);";
	char *SQL = NULL;
	int SQL_len = 0;
	sqlite3_stmt *pStmt;
	if ( !table_name )
		return NULL;
	if ( !( list = init_list( ) ) )
		return NULL;
	SQL_len = strlen( SQL_ ) + strlen( table_name );
	SQL = malloc( sizeof(char) * ( SQL_len ) );
	snprintf( SQL, SQL_len, SQL_, table_name );

	do
	{
		result = sqlite3_prepare( db, SQL, -1, &pStmt, 0 );
		if ( result != SQLITE_OK )
		{
			sqlite3_finalize( pStmt );
			return NULL;
		}
		while(( result = sqlite3_step( pStmt ) ) == SQLITE_ROW)
		{
			add_to_list( sqlite3_column_text( pStmt, 1 ), list );
		}
		result = sqlite3_finalize( pStmt );
	} while(result == SQLITE_SCHEMA);
	return list;
}

int print_col ( sqlite3_stmt * pStmt, int col )
{
	switch (sqlite3_column_type( pStmt, col ))
	{
	case SQLITE_INTEGER:
		printf( "%d; ", sqlite3_column_int( pStmt, col ) );
		break;
	case SQLITE_FLOAT:
		printf( "%f; ", sqlite3_column_double( pStmt, col ) );
		break;
	case SQLITE_TEXT:
		if ( strcasecmp( sqlite3_column_name( pStmt, col ), "created_at" ) == 0 )
		{
			time_t time_field = 0, *ptime_field = &time_field;
			time_field = atoi( sqlite3_column_text( pStmt, col ) );
			char *buff = malloc( sizeof(char) * 256 );
			buff = ctime( ptime_field );
			if ( buff != NULL )
				buff[strlen( buff ) - 1] = '\0';
			printf( "%s; ", buff );
		}
		else
		{
			printf( "%s; ", sqlite3_column_text( pStmt, col ) );
		}
		break;
	case SQLITE_BLOB:
		printf( "BLOB; " );
		break;
	case SQLITE_NULL:
		printf( "Null; " );
		break;
	default:
		printf( " *Cannot determine SQLITE TYPE* col=%d ", col );
	}

	return 0;
}

void databaseError ( sqlite3* db )
{
	int errcode = sqlite3_errcode( db );
	const char *errmsg = sqlite3_errmsg( db );
	fprintf( stderr, "Database error %d: %s\n", errcode, errmsg );
}
