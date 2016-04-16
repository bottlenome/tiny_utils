#!/bin/bash
# -*- coding: utf-8 -*-

OLDSERVERS="oldservers.txt"

prefix_path=/root/work/deploy

echo "terminate instance"

cat ${OLDSERVERS} | while read LINE
do
    echo "terminateing..."${LINE}
    ${prefix_path}/bin/deploy instances terminate ${LINE}
done
