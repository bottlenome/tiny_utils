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
    push_event_num = 0
    commits_num = 0
    for i in events:
        if i["type"] == "PushEvent":
            push_event_num += 1
            commits_num += len(i["payload"]["commits"])
    print "push events:%d"%push_event_num
    print "commits :%d"%commits_num
    print events
