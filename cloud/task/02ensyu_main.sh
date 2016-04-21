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
cat ${OLDSERVERS} | while read LINE
do
  echo "1_stop webnode ${LINE} start "
  ./02ensyu_sub.sh ${LINE}
  echo "2_stop webnode ${LINE} end "
done

echo "finish"

