#!/bin/bash
# -*- coding: utf-8 -*-
abs_path=$(cd $(dirname $0) && pwd)
prefix_path=/root/work/deploy

NEWSERVERS="newservers.txt"

./new_add_webnode_count.sh 2
cat ${NEWSERVERS} | while read LINE
do
  echo "deploying..."${LINE}
  ./deploy_war.sh ${LINE}
done

# update nginx
echo "start nginx"

# create nginx.conf
echo "create nginx.conf"
output_dir=/var/tmp
cat ${NEWSERVERS} | while read LINE
do
mco facts ipaddress -F fqdn=/^web.$LINE/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/nginx/nginx.ipset
done
echo "output file: ${output_dir}/nginx/nginx.ipset"

# refresh nginx.conf of lb server
echo "refresh nginx.conf"
mco puppetd runonce -I lb.nii.localdomain -v

# restart nginx proccess
echo "restart nginx"
mco service nginx restart -F fqdn=/^lb/ -v

