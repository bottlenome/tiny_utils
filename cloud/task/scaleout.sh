#!/bin/bash
# -*- coding: utf-8 -*-

abs_path=$(cd $(dirname $0) && pwd)
prefix_path=$(cd ${abs_path}/../ && pwd)

count=$1

if [ ! "${count}" ]; then
    echo "usage: ./scaleout.sh num :num is number"
    exit 1
fi

# get running web servers information
instance_ids=(`mco facts fqdn -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve instance_id mco`)
echo ${instance_ids[@]}

# count up running web servers
countup=`expr ${count} - ${#instance_ids[*]}`
if [ ${countup} -gt 0 ]; then
    echo "add webnode ${countup}"
    ${prefix_path}/task/add_webnode_count.sh ${countup}
else
    for ((i=0; $i < ${count}; ++i)); do
	unset instance_ids[$i]
    done
    if [ ${#instance_ids[*]} -eq 0 ]; then
      echo "There is no instance to be deleted."
      exit 0
    fi
    echo "delete webnode ${instance_ids[@]}"
    ${prefix_path}/task/delete_webnode.sh ${instance_ids[@]}
fi
