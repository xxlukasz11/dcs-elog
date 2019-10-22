#!/bin/bash
cd "${0%/*}"
../bin/e_log ../resources/parameters.conf 2>&1 &
