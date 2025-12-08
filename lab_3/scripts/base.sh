#!/bin/bash

cd $(dirname $0)/..

echo -n "0.0001, "
bin/main_base 100 0.0001 2> /dev/null
echo -n "0.0000316, "
bin/main_base 1000 0.0000316 2> /dev/null
echo -n "0.00001, "
bin/main_base 10000 0.00001 2> /dev/null
echo -n "0.00000316, "
bin/main_base 100000 0.00000316 2> /dev/null
echo -n "0.000001, "
bin/main_base 1000000 0.000001 2> /dev/null
