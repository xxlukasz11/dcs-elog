#!/bin/bash
cd "${0%/*}"
if [ -f "log.txt" ];
then
	rm log.txt
fi
../bin/e_log > ../build/log.txt &
echo "E-log started"
