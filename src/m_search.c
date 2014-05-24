/*
 * m_search.c
 *
 *  Created on: May 20, 2014
 *      Author: Volodymyr Varchuk
 */

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "m_search.h"
#include "db_func.h"
#include "list_func.h"
#include "parse_args.h"

#undef TEST

char *create_SQL_str ( opts_t *opts )
{
	char *sql_ = "SELECT %s FROM %s WHERE %s";
	char *sql = NULL;
	char *sql_where = NULL;
	int i = 0;
	int where_len = 0;
	int total_sql_len = 0;

	for( i = 0; i < opts->list_kvo->count; i++ )
	{
		where_len +=
				strlen( opts->list_kvo->key[i] ) + strlen( opts->list_kvo->op[i] ) + strlen( opts->list_kvo->val[i] ) + ((i > 0) ?
						strlen( opts->bool_operators->str[i - 1] ) : 0) + 6;
	}
	total_sql_len = where_len + strlen ( opts->table ) + strlen ( sql_ );
	sql = malloc( total_sql_len );
	sql_where = malloc ( where_len );
	sql_where[0] = '\0';
	for ( i = 0; i < opts->list_kvo->count; i++ )
	{
		sprintf ( sql_where + strlen(sql_where), "%s %s\"%s\" %s ", opts->list_kvo->key[i], opts->list_kvo->op[i], opts->list_kvo->val[i], ( i < opts->bool_operators->count ) ? opts->bool_operators->str[i] : "" );
	}
	sprintf ( sql, sql_, "*", opts->table, sql_where );
	return sql;
}

int main ( int argc, char **argv )
{
	sqlite3 *db;
	s_list *table_list = init_list();
	opts_t *opt = NULL;
	int i = 0, j = 0;

	if ( (opt = parse_args( argc, argv )) == NULL )
		return -1;
#ifdef TEST
	printf ( "db_name %s\n", opt->db_name );
	printf ( "SQL     %s\n", opt->SQL );
	printf ( "table   %s\n", opt->table );

	printf ( "list key value operation\n" );
	for ( i = 0; i < opt->list_kvo->count; i++ )
		printf ( "%s, %s, %s\n", opt->list_kvo->key[i], opt->list_kvo->val[i], opt->list_kvo->op[i] );

	printf ( "list of bool operation\n" );
	for ( i = 0; i < opt->bool_operators->count; i++ )
		printf ( "%s\n", opt->bool_operators->str[i] );
#endif

	if (db_open( &db, opt->db_name ) != SQLITE_OK )
		return -1;
	get_tables_names(db, table_list);

	if ( opt->SQL )
	{
		if ( exec_SQL( db, opt->SQL ) != SQLITE_OK)
		{
			databaseError( db );
			return -1;
		}
	}
	else
	{
		if ( exec_SQL (db, create_SQL_str( opt )) != SQLITE_OK )
		{
			databaseError ( db );
			return -1;
		}
	}
#ifdef TEST
	for ( i = 0; i < table_list->count; i++)
		printf ( "%s\n", table_list->str[i] );

	printf ("\n\n\n");
#endif

#ifdef TEST
	s_list *field_list = NULL;
	field_list = get_fields_names (db, opt->table);
	for ( j = 0; j < field_list->count; j++ )
		printf ("\t%s\n", field_list->str[j] );

	printf ( "%s\n", create_SQL_str( opt ) );
#endif

	free_s_list( table_list );
	free_opts (opt);
	return 0;
}

