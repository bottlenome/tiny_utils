#!/bin/bash
# -*- coding: utf-8 -*-

abs_path=$(cd $(dirname $0) && pwd)
prefix_path=$(cd ${abs_path}/../ && pwd)

instance_ids=$*
ipaddrs=

if [ ! "${instance_ids}" ]; then
    echo "usage ./delete_webnode.sh web_instance_id"
    exit 1
fi

# stop mcollective
echo "stop mcollective"
for instance_id in $instance_ids; do
  ipaddr=$(mco facts ipaddress -F fqdn=/^web.${instance_id}/ -v | ${prefix_path}/bin/retrieve ip mco)
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective stop"
  ipaddrs="${ipaddrs} ${ipaddr}"
done

# update nagios
echo "update nagios"

output_dir=/var/tmp
# create config nagios
echo "create web.ipset"
mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --output file > ${output_dir}/monitor/web.ipset
echo "output file: ${output_dir}/monitor/web.ipset"

# refresh nagios config
echo "refresh nagios config"
mco puppetd runonce -F fqdn=/^monitor/ -v

# stop ganglia-monitor
for ipaddr in $ipaddrs; do
  echo "stop ganglia-monitor ${ipaddr}"
  ${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/ganglia-monitor stop"
done

# restart ganglia-monitor
echo "restart ganglia-monitor"
mco service ganglia-monitor restart -F fqdn=/^web/ -v

# restart gmetad
echo "restart gmetad"
mco service gmetad restart -F fqdn=/^monitor/ -v

# restart nagios
echo "restart nagios"
monitor_ip=$(mco facts ipaddress -F fqdn=/^monitor/ -v | ${prefix_path}/bin/retrieve ip mco)
${prefix_path}/bin/deploy ssh exec ${monitor_ip} "/etc/init.d/nagios3 stop"
${prefix_path}/bin/deploy ssh exec ${monitor_ip} "/etc/init.d/nagios3 start"

# update nginx
echo "udpate nginx"

# create nginx.conf
echo "create nginx.conf"
mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --output file > ${output_dir}/nginx/nginx.ipset
echo "output file: ${output_dir}/nginx/nginx.ipset"

# refresh nginx.conf of lb server
echo "refresh nginx.conf"
mco puppetd runonce -I lb.nii.localdomain -v

# restart nginx proccess
echo "restart nginx"
mco service nginx restart -F fqdn=/^lb/ -v

# terminate instances
echo "terminate instance"
for instance_id in $instance_ids; do
  ${prefix_path}/bin/deploy instances terminate ${instance_id}
done

# work around reflesh ganglia.
# restart ganglia-monitor
echo "restart ganglia-monitor"
mco service ganglia-monitor restart -F fqdn=/^web/ -v

# restart gmetad
echo "restart gmetad"
mco service gmetad restart -F fqdn=/^monitor/ -v
