import urllib2
import json

url = "https://api.github.com/users/%s/events"
username = "bottlenome"
response = urllib2.urlopen(url%username)
ret = response.read()
events = json.loads(ret)
if len(events) == 0:
    print "no pushs for past 90 days"
else:
    print "TODO"
