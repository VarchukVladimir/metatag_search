/*
 * db_func.h
 *
 *  Created on: May 20, 2014
 *      Author: Volodymyr Varchuk
 */

#ifndef DB_FUNC_H_
#define DB_FUNC_H_

#include "list_func.h"

int db_open ( sqlite3 **, char * );
void databaseError ( sqlite3 * );
int get_tables_names ( sqlite3 *db, s_list * );
s_list *get_fields_names ( sqlite3 *, char * );

#endif /* DB_FUNC_H_ */
