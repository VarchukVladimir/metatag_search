/*
 * list_func.h
 *
 *  Created on: May 20, 2014
 *      Author: Volodymyr Varchuk
 */

#ifndef LIST_FUNC_H_
#define LIST_FUNC_H_

#define MEM_ERROR(A) fprintf ( stderr, "Error. Cannot allocate memory.\n" ); return A

typedef struct {
	char **key;
	char **val;
	char **op;
	int count;
	int max_count;
} kvo_list;

typedef struct s_list{
	char **str;
	int count;
	int max_count;
} s_list;

s_list *init_list ();
int add_to_list (const char *, s_list *);
void free_s_list ( s_list * );

kvo_list *init_list_kvo ();
int add_to_list_kvo ( const char *, const char *, const char *, kvo_list * );
void free_kvo_list ( kvo_list * );

#endif /* LIST_FUNC_H_ */
