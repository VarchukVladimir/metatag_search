#!/bin/bash

if [ $# -eq 2 ]
then
	ARGS=$1
	CONTAINER_NAME=$2
else
	echo "Usage: `basename $0` {request} (for example ./m_search.sh \"-kvo name:%index%)\""
	echo "Usage: `basename $0` {request} {container name} (for example ./zwift_m_search.sh \"-kvo name:%index%)\" \"gutenberg_index\""
	exit $WRONG_ARGS
fi

sed s@{ARGS}@"$ARGS"@ zwift/m_search.json.template | \
sed s@{CONTAINER_NAME}@"$CONTAINER_NAME"@g > zwift/m_search.json

zwift exec zwift/m_search.json


