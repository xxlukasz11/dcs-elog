#!/bin/bash
cd "${0%/*}"
../bin/e_log > ../build/log.txt &
echo "E-log started"