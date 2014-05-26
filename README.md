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
`./m_search.sh "-kvo name:%index% size&gt;10000 -b and" "test.db"`	
this example return names of objects in container which name include "index" string and which size bigger than 10000 bytes.

`./m_search.sh "-kvo name:index% size&lt;10000 -b and" "test.db"`	
this example return names of objects in container which name start with "index" string and which size less than 10000 bytes.

`./m_search.sh "-s SELECT * FROM object WHERE name&gt;index" "test.db"`		
this example use sql syntax for query

###run on zebra:

	./zwift_m_search.sh {request} {container name} 

example:	
`./zwift_m_search.sh "-kvo size&gt;10000" "test"`	
this example return names of objects in container which size bigger than 10000 bytes

