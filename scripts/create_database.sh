#!/bin/bash
# 1st parameter - database file name
# sqlite3 must be installed

if [ "$#" -ne 1 ]
then
	echo "Wrong number of arguments provided"
else
	scripts_folder="${0%/*}"
	setup_script_path="$scripts_folder/../sql/setup_database.sql"
	sqlite3 $1 < $setup_script_path
fi
