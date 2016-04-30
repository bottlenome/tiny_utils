abs_path=$(cd $(dirname $0) && pwd)
prefix_path=/root/work/deploy

OLDSERVERS="oldservers.txt"

#cat ${OLDSERVERS} | while read LINE
#do
#  echo "restarting..."${LINE}
#  ./04sub_restart_webnode.sh ${LINE}
#done
count=0
while read line; do
	LINES[$count]=$line
	count=$count+1
done < ${OLDSERVERS}

echo ${LINES[0]}
./04sub_restart_webnode.sh ${LINES[0]}
echo ${LINES[1]}
./04sub_restart_webnode.sh ${LINES[1]}


# settting for lb server
# update nginx
echo "start nginx"

# create nginx.conf
echo "create nginx.conf"
output_dir=/var/tmp
rm -f ${output_dir}/nginx/nginx.ipset
#cat ${OLDSERVERS} | while read LINE
#do
mco facts ipaddress -F fqdn=/^web.*${LINES[0]}/ -j | ${prefix_path}/bin/retrieve ip mco --format file >> ${output_dir}/nginx/nginx.ipset
mco facts ipaddress -F fqdn=/^web.*${LINES[1]}/ -j | ${prefix_path}/bin/retrieve ip mco --format file >> ${output_dir}/nginx/nginx.ipset
#done
echo "output file: ${output_dir}/nginx/nginx.ipset"

# refresh nginx.conf of lb server
echo "refresh nginx.conf"
mco puppetd runonce -I lb.nii.localdomain -v

# restart nginx proccess
echo "restart nginx"
mco service nginx restart -F fqdn=/^lb/ -v

echo "finish 04restart_webnode.sh"

