#!/bin/bash
# -*- coding: utf-8 -*-
abs_path=$(cd $(dirname $0) && pwd)
prefix_path=/root/work/deploy

OLDSERVERS="oldservers.txt"

## get running web servers information
#instance_ids=(`mco facts fqdn -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve instance_id mco`)
#echo ${instance_ids[@]}


echo "start"


# read old server server id

<< coment
cat ${OLDSERVERS} | while read LINE
do
  echo "1_stop webnode ${LINE} start "
  ./02ensyu_sub.sh ${LINE}
  echo "2_stop webnode ${LINE} end "
done

do
  echo "1_stop webnode ${LINE} start "	
#  ./02ensyu_sub.sh ${LINE}
  echo "2_stop webnode ${LINE} end "
done

while read LINE
do
  echo "1_stop webnode ${LINE} start "
  ./02ensyu_sub.sh ${LINE}
  echo "2_stop webnode ${LINE} end "
done < <(cat ./oldservers.txt)

for LINE in ${OLDSERVERS}
  do
  echo "1_stop webnode ${LINE} start "
  ./02ensyu_sub.sh ${LINE}
  echo "2_stop webnode ${LINE} end "
  done
coment

count=0
while read line; do
	LINES[$count]=$line
	count=$count+1
done < ${OLDSERVERS}

echo ${LINES[0]}
echo ${LINES[1]}

./02ensyu_sub.sh ${LINES[@]}

echo "finish"

