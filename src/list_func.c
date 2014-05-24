/*
 * list_func.c
 *
 *  Created on: May 20, 2014
 *      Author: Volodymyr Varchuk
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_func.h"
#include "m_search.h"

s_list *init_list ()
{
	s_list *list = malloc( sizeof(s_list) );
	int i = 0;

	if ( !list )
	{
		MEM_ERROR(NULL);
	}
	if ( !( list->str = malloc( sizeof(char *) * INIT_LIST_SIZE ) ) )
	{
		MEM_ERROR(NULL);
	}
	list->count = 0;
	list->max_count = INIT_LIST_SIZE;
	for( i = 0; i < INIT_LIST_SIZE; i++ )
		list->str[i] = NULL;
	return (s_list *) list;
}

kvo_list *init_list_kvo ()
{
	kvo_list *list = malloc( sizeof(kvo_list) );
	int i = 0;

	if ( !list )
	{
		MEM_ERROR(NULL);
	}
	if ( !( list->key = malloc( sizeof(char *) * INIT_LIST_SIZE ) ) )
	{
		free( list );
		MEM_ERROR(NULL);
	}
	if ( !( list->val = malloc( sizeof(char *) * INIT_LIST_SIZE ) ) )
	{
		free( list->key );
		free( list );
		MEM_ERROR(NULL);
	}
	if ( !( list->op = malloc( sizeof(char *) * INIT_LIST_SIZE ) ) )
	{
		free( list->key );
		free( list->val );
		free( list );
		MEM_ERROR(NULL);
	}
	list->count = 0;
	list->max_count = INIT_LIST_SIZE;
	for( i = 0; i < INIT_LIST_SIZE; i++ )
		list->key[i] = list->val[i] = list->op[i] = NULL;
	return list;
}

int check_list_size ( s_list *list )
{
	if ( !list )
		return -1;

	if ( list->count >= ( list->max_count - 1 ) )
	{
		char **temp_list = NULL;
		temp_list = (char **) realloc( list->str, sizeof(char *) * list->max_count * 2 + 1 );
		if ( temp_list != NULL )
		{
			list->str = temp_list;
			list->max_count *= 2;
		}
		else
		{
			MEM_ERROR();
		}
	}
	return 0;
}

int add_to_list ( const char *str, s_list *list )
{
	int resize = 0;
	int max_size = list->max_count;

	if ( list->count >= ( list->max_count - 1 ) )
	{
		char **temp_list = NULL;
		temp_list = (char **) realloc( list->str, sizeof(char *) * list->max_count * 2 + 1 );
		if ( temp_list != NULL )
		{
			list->str = temp_list;
			list->max_count *= 2;
		}
		else
		{
			MEM_ERROR();
		}
	}

	if ( str != NULL )
	{
		list->str[list->count] = (char *) malloc( sizeof(char) * ( strlen( str ) + 1 ) );
		if ( !list->str[list->count] )
		{
			MEM_ERROR(-1);
		}
		strncpy( list->str[list->count], str, strlen( str ) + 1 );
	}
	else
	{
		list->str[list->count] = NULL;
	}
	list->count++;

	return 0;
}

int add_to_list_kvo ( const char *key, const char *val, const char *op, kvo_list *list )
{

	if ( !list )
		return -1;

	if ( list->count >= ( list->max_count - 1 ) )
	{
		char **temp_list = NULL;
		// key realloc
		temp_list = (char **) realloc( list->key, sizeof(char *) * list->max_count * 2 + 1 );
		if ( temp_list != NULL )
		{
			list->key = temp_list;
		}
		else
		{
			MEM_ERROR(-1);
		}
		// val realloc
		temp_list = (char **) realloc( list->val, sizeof(char *) * list->max_count * 2 + 1 );
		if ( temp_list != NULL )
		{
			list->val = temp_list;
		}
		else
		{
			MEM_ERROR(-1);
		}
		// op realloc
		temp_list = (char **) realloc( list->op, sizeof(char *) * list->max_count * 2 + 1 );
		if ( temp_list != NULL )
		{
			list->op = temp_list;
		}
		else
		{
			MEM_ERROR(-1);
		}
		list->max_count *= 2;
	}


	if ( key != NULL )
	{
		list->key[list->count] = (char *) malloc( sizeof(char) * ( strlen( key ) + 1 ) );
		if ( !list->key[list->count] )
		{
			MEM_ERROR(-1);
		}
		strncpy( list->key[list->count], key, strlen( key ) + 1 );
	}
	else
	{
		list->key[list->count] = NULL;
	}

	if ( val != NULL )
	{
		list->val[list->count] = (char *) malloc( sizeof(char) * ( strlen( val ) + 1 ) );
		if ( !list->val[list->count] )
		{
			MEM_ERROR(-1);
		}
		strncpy( list->val[list->count], val, strlen( val ) + 1 );
	}
	else
	{
		list->val[list->count] = NULL;
	}

	if ( op != NULL )
	{
		list->op[list->count] = (char *) malloc( sizeof(char) * ( strlen( op ) + 1 ) );
		if ( !list->op[list->count] )
		{
			MEM_ERROR(-1);
		}
		strncpy( list->op[list->count], op, strlen( op ) + 1 );
	}
	else
	{
		list->op[list->count] = NULL;
	}
	list->count++;

	return 0;
}

void free_s_list ( s_list *list )
{
	int i = 0;
	if ( !list )
		return;
	for ( i = 0; i < list->count; i++ )
		free(list->str[i]);
	free (list);
	return;
}

void free_kvo_list ( kvo_list *list )
{
	int i = 0;
	if ( !list )
		return;
	for ( i = 0; i < list->count; i++ )
	{
		free(list->key[i]);
		free(list->val[i]);
		free(list->op[i]);
	}
	free (list);
	return;
}
