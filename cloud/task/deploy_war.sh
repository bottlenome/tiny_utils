#!/bin/bash

svn export svn+ssh://localhost/root/work/repos/keijiban/keijiban.war /var/tmp/keijiban.war
/usr/bin/mco puppetd enable -F fqdn=/^web.$1/ -v
mco puppetd runonce -F fqdn=/^web.$1/ -v
/usr/bin/mco puppetd disable -F fqdn=/^web.$1/ -v
