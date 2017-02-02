#!/bin/bash
# -*- coding: utf-8 -*-

set -e

abs_path=$(cd $(dirname $0) && pwd)
prefix_path=$(cd ${abs_path}/../ && pwd)

# get Web servers information

old_instance_ids=($( mco facts ipaddress -F fqdn=/^web/ -v -j | ${prefix_path}/bin/retrieve instance_id mco ))
instance_num=${#old_instance_ids[*]}
echo "old_instances: ${old_instance_ids[@]}"

if [ ${instance_num} -eq 0 ] ; then
  echo "ERROR: No web instance found."
  exit 1
fi

# launch instance

echo "launch web instances"
instance_ids=
for ((i=1; i<=instance_num; i++)); do
  instance_id=$(${prefix_path}/bin/deploy instances launch web | ${prefix_path}/bin/retrieve instance_id cloud)
  if [ ${i} -eq 1 ]; then
    instance_ids="${instance_id}"
  else
    instance_ids="${instance_id} ${instance_ids}"
  fi
done
echo "instance ids: ${instance_ids}"

# wait instance state running
for instance_id in ${instance_ids}; do
  echo "wait until ${instance_id} launched"
  ${prefix_path}/bin/deploy instances wait ${instance_id}
done
echo "running instance"

# add deploy server ipaddr to webnode
mail_ipaddr=`mco facts ipaddress -F fqdn=/^mail/ -j | ${prefix_path}/bin/retrieve ip mco`
echo "mail server: ${mail_ipaddr}"

db_ipaddr=`mco facts ipaddress -F fqdn=/^db/ -j | ${prefix_path}/bin/retrieve ip mco`
echo "db server: ${db_ipaddr}"

deploy_ipaddr=`/sbin/ip route get 8.8.8.8 |head -1 |awk '{print $7}'`

for instance_id in ${instance_ids}; do
  ipaddr=`${prefix_path}/bin/deploy instances describe --instanceids=${instance_id} --key=ipaddr | ${prefix_path}/bin/retrieve ip cloud`
  echo "deploy ${instance_id}:${ipaddr}"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${deploy_ipaddr} deploy.nii.localdomain\" >> /etc/hosts"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${mail_ipaddr} mail.nii.localdomain\" >> /etc/hosts"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${db_ipaddr} db.nii.localdomain\" >> /etc/hosts"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/puppet stop"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective stop"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "rm -rf /var/lib/puppet/ssl"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/puppet start"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective start"
done

# add puppetca to puppetmaster
echo 'start puppet cert'

while [[ "${certnum}" != "${instance_num}" ]]; do
  sleep 5
  certnum="$( puppet cert list | grep web | wc -l )"
done
puppet cert sign --all

echo 'accepted cert list'
puppet cert list --all

# wait for it to finish the execution of the puppet agent
for instance_id in ${instance_ids}; do
  fqdn="web.${instance_id}.nii.localdomain"
  echo "[${fqdn}]"
  max=30

  while :
  do
    for ((i=1; i<=max; i++)); do
      ipaddr=$( mco facts ipaddress -I ${fqdn} | ${prefix_path}/bin/retrieve ip mco )
      echo "[${fqdn}] ip: ${ipaddr}"
      if [ ${ipaddr} ]; then
        echo "[${fqdn}] Retrieved IP address by mco"
        break
      fi
      sleep 10
    done

    if [ ${ipaddr} ]; then
      break
    fi

    web_node_ipaddress=`${prefix_path}/bin/deploy instances describe --instanceids=${instance_id} --key=ipaddr | ${prefix_path}/bin/retrieve ip cloud`
    ${prefix_path}/bin/deploy ssh exec ${web_node_ipaddress} "/etc/init.d/mcollective stop"
    ${prefix_path}/bin/deploy ssh exec ${web_node_ipaddress} "/etc/init.d/mcollective start"

    sleep 10
  done

  echo "facts created: ${instance_id}"

  # restart the tomcat to run war file that was distributed
  # ${prefix_path}/bin/deploy ssh exec ${ipaddr} "service tomcat6 stop"
  # ${prefix_path}/bin/deploy ssh exec ${ipaddr} "service tomcat6 start"
  mco service tomcat6 restart -I ${fqdn} -v || :

  # prevent the automatic update ot puppet agent
  mco puppetd disable -F fqdn=${fqdn}
done

# shutdown old instances
echo "stop old webnode"
${prefix_path}/task/delete_webnode.sh ${old_instance_ids[@]}
