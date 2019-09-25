#!/bin/bash
cd "${0%/*}"
touch ../build/log.txt
../bin/e_log > ../build/log.txt &
echo "E-log started"
