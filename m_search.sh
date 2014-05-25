#!/bin/bash

if [ $# -eq 1 ]
then
	ARGS=$1
	DB_NAME_="test/test.db"
else 
	if [ $# -eq 2 ]
	then
		ARGS=$1
		DB_NAME_=$2
	else
		echo "Usage: `basename $0` {request} (for example ./m_search.sh \"-kvo name:%index%)\""
		echo "Usage: `basename $0` {request} {database} (for example ./m_search.sh \"-kvo name:%index%)\" \"test/test.db\""
 		exit $WRONG_ARGS
	fi
fi

SCRIPT=$(readlink -f "$0")
SCRIPT_PATH=`dirname "$SCRIPT"`
ABS_PATH=$SCRIPT_PATH

DB_NAME=$(readlink -f "$DB_NAME_")


sed s@{ARGS}@"$ARGS"@g test/m_search.nvram.template > test/m_search.nvram

sed s@{ARGS}@"$ARGS"@ test/m_search.manifest.template | \
sed s@{ABS_PATH}@"$ABS_PATH"@g | \
sed s@{DB_NAME}@"$DB_NAME"@g > test/m_search.manifest

time zerovm test/m_search.manifest

cat test/m_search_stdout.data
