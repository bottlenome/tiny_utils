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

#update monitoring condition
output_dir=/var/tmp
mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --output file > ${output_dir}/monitor/web.ipset

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


