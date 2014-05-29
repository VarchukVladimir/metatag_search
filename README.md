# Metatag_search
==============

## Prerequisites

Install 
[sqlite](https://github.com/zerovm/zerovm-ports/blob/master/sqlite/README.md)

## Building

	make

## Usage



###local run with test database:
	./m_search.sh {request} {test database}

example:	
`./m_search.sh "-r name:%index% and size>10000" "~/test.db"`
this example return records about objects in container which name include "index" string and which size bigger than 10000 bytes.

`./m_search.sh "-t name:index% and size<10000 and created_at>15.05.2014" "test.db"`	
this example return names of objects in container which name start with "index" string, which size less than 10000 bytes and which cteated after date 15.05.2014.

`./m_search.sh "-s SELECT * FROM object WHERE name>index" "~/test.db"`	
this example use sql syntax for query

###run on zebra:

	./zwift_m_search.sh {request} {container name} 

example:	
`./zwift_m_search.sh "-r size>10000" "test"`	
this example return names of objects in container which size bigger than 10000 bytes

