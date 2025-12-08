#!/bin/bash

cd $(dirname $0)/..

echo -n "100, "
bin/main_base 100 0.0001 2> /dev/null
echo -n "1000, "
bin/main_base 1000 0.0000316 2> /dev/null
echo -n "10000, "
bin/main_base 10000 0.00001 2> /dev/null
echo -n "100000, "
bin/main_base 100000 0.00000316 2> /dev/null
echo -n "1000000, "
bin/main_base 1000000 0.000001 2> /dev/null
