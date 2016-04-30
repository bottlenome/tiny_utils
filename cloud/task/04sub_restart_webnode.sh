2>> /tmp/add_webnode.debug
set -x
date >&2
whoami >&2
abs_path=$(cd $(dirname $0) && pwd)
prefix_path=/root/work/deploy

######### fuji_test #######
## # launch instance
## instance_id="$(${prefix_path}/bin/deploy instances launch web)"
## echo "launch instance: ${instance_id}"
instance_id=$1
echo "instance_id : ${instance_id}"
## 
## # wait instance state running
## ${prefix_path}/bin/deploy instances wait ${instance_id}
## echo "running instance"
######### fuji_test #######

# add deploy server ipaddr to webnode
mail_ipaddr=$(mco facts ipaddress -F fqdn=/^mail/ -j | ruby ${prefix_path}/bin/retrieve ip mco)
echo "mail server: $mail_ipaddr"

db_ipaddr="$(mco facts ipaddress -F fqdn=/^db/ -j | ${prefix_path}/bin/retrieve ip mco)"
echo "db server: $db_ipaddr"

deploy_ipaddr=`/sbin/ip route get 8.8.8.8 |head -1 |awk '{print $7}'`
ipaddr="$(${prefix_path}/bin/deploy instances describe ${instance_id})"

${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${deploy_ipaddr} deploy.nii.localdomain\" >> /etc/hosts"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${mail_ipaddr} mail.nii.localdomain\" >> /etc/hosts"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "echo \"${db_ipaddr} db.nii.localdomain\" >> /etc/hosts"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/puppet stop"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective stop"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "rm -fr /var/lib/puppet/ssl/"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/puppet start"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/mcollective start"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/tomcat6 start"
echo 'start puppetca'

######### fuji_test #######
## 引数に渡されたoldserverのインスタンスIDを対象にする
#while [[ "$thecerts" != *web*localdomain* ]]; do
##while [[ "$thecerts" != *web*${instance_id}*localdomain* ]]; do
######### fuji_test #######
#   sleep 5
#   thecerts="$( puppet cert list )"
#done
#puppet cert sign --all

echo 'end puppetca'

# wait for it to finish the execution of the puppet agent
fqdn="$(${prefix_path}/bin/deploy ssh exec ${ipaddr} "hostname")"
max=30
while :
do
    ip=`mco facts ipaddress -I ${fqdn} | ${prefix_path}/bin/retrieve ip mco`
    if [ $ip ] || [ "$max" = "0" ] ; then
        break
    fi
    max=$(( max - 1 ))
    sleep 10
done

# restart the tomcat to run war file that was distributed
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/tomcat6 stop"
${prefix_path}/bin/deploy ssh exec ${ipaddr} "/etc/init.d/tomcat6 start"

# update nginx
echo "start nginx"

# create nginx.conf
echo "create nginx.conf"
output_dir=/var/tmp

######### fuji_test #######
## 引数に渡されたoldserverのインスタンスIDを対象にする
## mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/nginx/nginx.ipset
mco facts ipaddress -F fqdn=/*web*${instance_id}*/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/nginx/nginx.ipset
echo "output file: ${output_dir}/nginx/nginx.ipset"

# refresh nginx.conf of lb server
echo "refresh nginx.conf"
mco puppetd runonce -I lb.nii.localdomain -v

# restart nginx proccess
echo "restart nginx"
mco service nginx restart -F fqdn=/^lb/ -v

echo 'restart ganglia-monitor'
yes | mco service ganglia-monitor restart -v

echo 'start nagios'
# create config nagios

######### fuji_test #######
## 引数に渡されたoldserverのインスタンスIDを対象にする
## mco facts ipaddress -F fqdn=/^web/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/monitor/web.ipset
mco facts ipaddress -F fqdn=/*web*${instance_id}*/ -j | ${prefix_path}/bin/retrieve ip mco --format file > ${output_dir}/monitor/web.ipset

head ${output_dir}/monitor/web.ipset

# refresh nagios config
mco puppetd runonce -F fqdn=/^monitor/ -v

# restart nagios
mco service gmetad restart -F fqdn=/^monitor/ -v

# next line does not work, so stopping and starting explicitly
# mco service nagios3 restart -F fqdn=/^monitor/ -v
${prefix_path}/bin/deploy ssh exec monitor.nii.localdomain "service nagios3 stop"
${prefix_path}/bin/deploy ssh exec monitor.nii.localdomain "service nagios3 start"

# prevent the automatic update to puppet agent

######### fuji_test #######
## テキストを読み込んでoldserverを対象とする
## mco puppetd disable -F fqdn=/^web/ -v
mco puppetd disable -F fqdn=/*web*${instance_id}*/ -v


