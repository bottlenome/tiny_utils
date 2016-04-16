#!/bin/bash
# -*- coding: utf-8 -*-

OLDSERVERS="oldservers.txt"

abs_path=$(cd $(dirname $0) && pwd)
prefix_path=$(cd ${abs_path}/../ && pwd)

echo "terminate instance"

cat ${OLDSERVERS} | while read LINE
do
    echo "terminateing..."${LINE}
    ${prefix_path}/bin/deploy instances terminate ${LINE}
done
