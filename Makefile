#CC = x86_64-nacl-gcc
CC = gcc
CCFLAGS = -g -O0
LIBS = -lsqlite3

APP_NAME = m_search

OBJS = obj/m_search.o obj/db_func.o obj/list_func.o obj/parse_args.o 
SRC = src/m_search.c src/db_func.c src/list_func.c src/parse_args.c

all: create_dirs $(APP_NAME)

create_dirs:
	@mkdir obj -p 

$(APP_NAME): $(OBJS)	
	$(CC) -o $(APP_NAME) $(OBJS) $(CCFLAGS) $(LIBS)
	
obj/m_search.o: src/m_search.c  
	$(CC) -c -o $@ $^ $(CCFLAGS) 
	
obj/db_func.o: src/db_func.c
	$(CC) -c -o $@ $^ $(CCFLAGS)

obj/list_func.o: src/list_func.c
	$(CC) -c -o $@ $^ $(CCFLAGS)

obj/parse_args.o: src/parse_args.c
	$(CC) -c -o $@ $^ $(CCFLAGS)

clean:
	rm -rf obj/* $(APP_NAME) 

