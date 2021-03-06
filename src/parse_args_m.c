/*
 * parse_args.c
 *
 *  Created on: May 21, 2014
 *      Author: Volodymyr Varchuk
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parse_args_m.h"
#include "list_func.h"
#include "m_search.h"

char *legal_op_list[] = { "!=", "<=", ">=", "=", "<", ">", "like" };
char *legal_bool_op_list[] = { "or", "and" };
char *date_format[] = { "%d.%m.%Y", "%d.%m.%Y %H:%M:%S", "%m/%d/%Y", "%m/%d/%Y %H:%M:%S", };
char *date_field[] = { "created_at" };

extern char *strptime ( __const char *__restrict __s, __const char *__restrict __fmt, struct tm *__tp );

int convert_date_fields ( opts_t *opt )
{
	int i = 0, j = 0, format_index = 0;
	int field_list_size = sizeof( date_field ) / sizeof( date_field[0] );
	int format_list_size = sizeof( date_format ) / sizeof( date_format[0] );

	if ( opt != NULL )
		for( i = 0; i < opt->list_kvo->count; i++ )
		{
			for( j = 0; j < field_list_size; j++ )
			{
				if ( opt->list_kvo->val[i] != NULL && strcasecmp( date_field[j], opt->list_kvo->key[i] ) == 0 )
				{
					char *temp_buf = NULL;
					int k = 0;
					for( k = 0; k < format_list_size; k++ )
					{
						struct tm *ltm = malloc( sizeof(struct tm) );
						char *p;
						p = strptime( opt->list_kvo->val[i], date_format[k], ltm );
						if ( p != NULL )
						{
							temp_buf = malloc( sizeof( 256 ) );
							time_t t = mktime( ltm );
							snprintf( temp_buf, 256, "%zu", t );
							free( opt->list_kvo->val[i] );
							opt->list_kvo->val[i] = temp_buf;
						}
					}
				}
			}
		}

	return 0;
}

int check_legal_op ( char *op )
{
	int i = 0;
	int list_len = sizeof( legal_op_list ) / sizeof( legal_op_list[0] );

	if ( !op )
		return -1;

	for( i = 0; i < list_len; i++ )
	{
		if ( strncasecmp( op, legal_op_list[i], strlen( op ) ) == 0 )
		{
			return 0;
		}
	}
	return -1;
}

int check_legal_bool_op ( char *op )
{
	int i = 0;
	int list_len = sizeof( legal_bool_op_list ) / sizeof( legal_bool_op_list[0] );

	if ( !op )
		return -1;

	for( i = 0; i < list_len; i++ )
	{
		if ( strncasecmp( op, legal_bool_op_list[i], strlen( op ) ) == 0 )
		{
			return 0;
		}
	}
	return -1;
}

opts_t *parse_args_m ( int argc, char **argv )
{
	int i = 0;
	kvo_list *list_kvo = init_list_kvo( );
	s_list *list_ops = init_list( );

	opts_t *opts = NULL;
	int kvo = 0;
	int not_sql = 0;
	opts = malloc( sizeof(opts_t) );

	opts->db_name = strdup( DEFAULT_INPUT_DB_DEVICE );
	opts->table = strdup( DEFAULT_TABLE_NAME );

	if ( argc < 2 )
		return NULL;

	for( i = 1; i < argc; i++ )
	{
		if ( ( strcasecmp( argv[i], "-d" ) == 0 ) && ( i + 1 < argc ) )
		{
			opts->db_name = malloc( strlen( argv[i + 1] ) + 1 );
			memcpy( opts->db_name, argv[i + 1], strlen( argv[i + 1] ) + 1 );
			kvo = 0;
			i++;
			continue;
		}
		if ( ( strcasecmp( argv[i], "-t" ) == 0 ) && ( i + 1 < argc ) )
		{
			char *buff = NULL;
			buff = malloc( strlen( argv[i + 1] ) + 1 );
			memcpy( buff, argv[i + 1], strlen( argv[i + 1] ) + 1 );
			opts->table = buff;
			kvo = 0;
			i++;
			not_sql = 1;
			continue;
		}

		if ( not_sql == 0 && ( strcasecmp( argv[i], "-s" ) == 0 ) && ( i + 1 < argc ) )
		{
			char *buff = NULL;
			int sql_len = 0;
			int j = 0;
			for( j = i + 1; j < argc; j++ )
				sql_len += ( strlen( argv[j] ) + 2 );
			buff = malloc( sql_len );
			for( j = i + 1; j < argc; j++ )
				snprintf( buff + strlen( buff ), sql_len - strlen( buff ), "%s ", argv[j] );

			opts->SQL = buff;
			free( opts->table );
			opts->table = NULL;
			opts->list_kvo = init_list_kvo( );
			opts->bool_operators = init_list( );
			return opts;
		}

		char *pkey = get_str_part( argv[i], key_str );
		char *pop = get_str_part( argv[i], op_str );
		char *pval = get_str_part( argv[i], val_str );
		int add_op = 0;
		if ( ( kvo == 1 ) && ( strcasecmp( argv[i], "or" ) != 0 ) && ( strcasecmp( argv[i], "and" ) != 0 ) )
		{
			if ( add_to_list_kvo( pkey, pval, pop, list_kvo ) != 0 )
				return NULL;
		}
		else if ( kvo == 1 && ( ( strcasecmp( argv[i], "or" ) == 0 ) || ( strcasecmp( argv[i], "and" ) == 0 ) ) )
		{
			add_op = 1;
			add_to_list( argv[i], list_ops );
		}
		else if ( kvo == 1 )
		{
			return NULL;
		}

		if ( ( strcasecmp( argv[i], "-r" ) == 0 ) && ( i + 1 < argc ) )
		{
			kvo = 1;
			not_sql = 1;
		}
	}
	for( i = 0; i < list_kvo->count; i++ )
	{
		if ( check_legal_op( list_kvo->op[i] ) != 0 )
		{
			fprintf( stderr, MESS_ILLEGAL_OPERATOR, list_kvo->op[i] );
			return NULL;
		}
	}
	for( i = 0; i < list_ops->count; i++ )
	{
		if ( check_legal_bool_op( list_ops->str[i] ) != 0 )
		{
			fprintf( stderr, MESS_ILLEGAL_LOG, list_ops->str[i] );
			return NULL;
		}
	}

	if ( not_sql == 1 )
		opts->SQL = NULL;

	if ( list_ops->count < ( list_kvo->count - 1 ) )
	{
		fprintf( stderr, MESS_FEW_BOOL_OPERATROS );
		return NULL;
	}

	opts->bool_operators = list_ops;
	opts->list_kvo = list_kvo;

	convert_date_fields( opts );

	return opts;
}

char *get_str_part ( char *str, str_part opt )
{
	int i = 0, pos_op = 0, len_op = 0;
	int str_len = 0;
	char *buff = NULL;

	if ( !str )
		return NULL;

	str_len = strlen( str );

	for( i = 0; i < str_len; i++ )
	{
		if ( ( !pos_op ) && ( i != 0 ) && ( str[i] == '!' || str[i] == '>' || str[i] == '<' || str[i] == '=' || str[i] == ':' ) )
		{
			pos_op = i;
		}
		if ( pos_op && ( pos_op != i ) && !( str[i] == '!' || str[i] == '>' || str[i] == '<' || str[i] == '=' || str[i] == ':' ) )
		{
			len_op = i - pos_op;
			break;
		}

	}

	if ( pos_op == 0 || len_op == 0 )
		return NULL;

	switch (opt)
	{
	case key_str:
		buff = malloc( sizeof(char) * ( pos_op + 1 ) );
		strncpy( buff, str, pos_op );
		buff[pos_op] = '\0';
		break;
	case val_str:
		buff = malloc( sizeof(char) * ( str_len - len_op - pos_op + 1 ) );
		strncpy( buff, str + i, str_len - i );
		buff[str_len - i] = '\0';
		break;
	case op_str:
		buff = malloc( sizeof(char) * ( len_op ) );
		strncpy( buff, str + pos_op, len_op );
		buff[len_op] = '\0';
		if ( strcasecmp( buff, ":" ) == 0 )
		{
			free( buff );
			buff = strdup( "like" );
		}
		break;
	}

	return buff;
}

void free_opts ( opts_t *opt )
{
	if ( opt == NULL )
		return;

	if ( opt->bool_operators )
		free_s_list( opt->bool_operators );
	if ( opt->list_kvo )
		free_kvo_list( opt->list_kvo );
	if ( opt->SQL )
		free( opt->SQL );
	if ( opt->db_name )
		free( opt->db_name );
	if ( opt->table )
		free( opt->table );
	return;
}
