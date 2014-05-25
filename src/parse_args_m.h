/*
 * parse_args.h
 *
 *  Created on: May 21, 2014
 *      Author: Volodymyr Varchuk
 */

#ifndef PARSE_ARGS_H_
#define PARSE_ARGS_H_

#include "list_func.h"

typedef enum { key_str, val_str, op_str } str_part;

typedef struct {
	kvo_list *list_kvo;
	s_list *bool_operators;
	char *db_name;
	char *table;
	char *SQL;
} opts_t;

extern char *legal_op_list[];
extern char *legal_bool_op_list[];


char *get_str_part ( char *, str_part );
opts_t *parse_args_m ( int, char ** );

#endif /* PARSE_ARGS_H_ */
