/*
 * m_search.h
 *
 *  Created on: May 20, 2014
 *      Author: Volodymyr Varchuk
 */

#ifndef M_SEARCH_H_
#define M_SEARCH_H_

#define DEFAULT_INPUT_DB_DEVICE "/dev/input"
#define DEFAULT_TABLE_NAME "object"
#define INIT_LIST_SIZE 10
#define MESS_ILLEGAL_OPERATOR "Illegal operator %s. Only \"<\" , \">\", \"=\", \":\", \"!=\", \"<=\", \">=\" allowed. \n"
#define MESS_ILLEGAL_LOG "Illegal operator %s. Only \"or\", \"and\" allowed. \n"
#define MESS_FEW_BOOL_OPERATROS "Too few logical operators.\n"
#define USAGE "USAGE [OPTIONS] <query>\n\
	-s <query>\t process query in sql format\n\
	-d <file name>\t database file\n\
	-t <table name>\t available only without option -s\n\
	-r [name][condition][value] or|and [name][condition][value]\t request\n\
		condition - :(like), >, <, >=, <=, != \n\
	example:\n\
	m_search -d ~/test.db -t object -r name:test% and size>20000\n\
	m_search -d ~/test.db -s SELECT * FROM object\n"
#define TEST


#endif /* M_SEARCH_H_ */
