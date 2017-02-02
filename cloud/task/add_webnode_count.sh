#!/bin/bash
# -*- coding: utf-8 -*-

abs_path=$(cd $(dirname $0) && pwd)
prefix_path=$(cd ${abs_path}/../ && pwd)

count=$1

if [ ! "${count}" ]; then
    echo "usage: ./add_webnode_count.sh num :num is number"
    exit 1
fi

# launch instance
instance_ids=
for ((i=0; $i < ${count}; i++)); do
  instance_id="$(${prefix_path}/bin/deploy instances launch web)"
  instance_ids="${instance_ids} ${instance_id}"
done
echo "launch instance: ${instance_ids}"

# wait instance state running
for instance_id in ${instance_ids}; do
  ${prefix_path}/bin/deploy instances wait ${instance_id}
done
echo "running instances"

# add deploy server ipaddr to webnode
mail_ipaddr=$(mco facts ipaddress -F fqdn=/^mail/ -j | ${prefix_path}/bin/retrieve ip mco)
echo "mail server: $mail_ipaddr"

db_ipaddr=$(mco facts ipaddress -F fqdn=/^db/ -j | ${prefix_path}/bin/retrieve ip mco)
echo "db server: $db_ipaddr"

deploy_ipaddr=$(/sbin/ip route get 8.8.8.8 |head -1 |awk '{print $7}')
for instance_id in ${instance_ids}; do
  ipaddr="$(${prefix_path}/bin/deploy instances describe ${instance_id})"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${deploy_ipaddr} deploy.nii.localdomain\" >> /etc/hosts"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${mail_ipaddr} mail.nii.localdomain\" >> /etc/hosts"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${db_ipaddr} db.nii.localdomain\" >> /etc/hosts"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/puppet stop"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective stop"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "rm -rf /var/lib/puppet/ssl/" 
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/puppet start"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective start"
done

# add puppetca to puppetmaster
echo 'start puppetca'

while [[ "$thecerts" != *web*localdomain* ]]; do
   sleep 5
   thecerts="$( puppet cert list )"
done
puppet cert sign --all

echo 'end puppetca'

# wait for it to finish the execution of the puppet agent
for instance_id in ${instance_ids}; do
  fqdn="$(${prefix_path}/bin/deploy ssh exec ${ipaddr} "hostname")"
  max=30
  while :
  do
    ip=$(mco facts ipaddress -I ${fqdn} | ${prefix_path}/bin/retrieve ip mco)
    if [ $ip ]; then
      break
    fi
    sleep 10
  done
done

# restart the tomcat to run war file that was distributed
for instance_id in ${instance_ids}; do
  ipaddr="$(${prefix_path}/bin/deploy instances describe ${instance_id})"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/tomcat6 stop"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/tomcat6 start"
done


# update nginx
echo "start nginx"

# create nginx.conf
echo "create nginx.conf"
output_dir=/var/tmp
mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/nginx/nginx.ipset
echo "output file: ${output_dir}/nginx/nginx.ipset"

# refresh nginx.conf of lb server
echo "refresh nginx.conf"
mco puppetd runonce -I lb.nii.localdomain -v

# restart nginx proccess
echo "restart nginx"
mco service nginx restart -F fqdn=/^lb/ -v

# restart ganglia-monitor
echo 'restart ganglia-monitor'
yes | mco service ganglia-monitor restart -v

echo 'start nagios'
# create config nagios
mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/monitor/web.ipset

head ${output_dir}/monitor/*.ipset

# refresh nagios config
mco puppetd runonce -F fqdn=/^monitor/ -v

# restart nagios
mco service gmetad restart -F fqdn=/^monitor/ -v

monitor_ip=$(mco facts ipaddress -F fqdn=/^monitor/ -v | ${prefix_path}/bin/retrieve ip mco)
${prefix_path}/bin/deploy ssh exec ${monitor_ip} "/etc/init.d/nagios3 stop"
${prefix_path}/bin/deploy ssh exec ${monitor_ip} "/etc/init.d/nagios3 start"

# prevent the automatic update ot puppet agent
mco puppetd disable -F fqdn=/^web/ -v
