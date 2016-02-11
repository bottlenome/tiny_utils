import urllib2
import json


def get_json(url):
    response = urllib2.urlopen(url)
    ret = response.read()
    json_object = json.loads(ret)
    return json_object

url = "https://api.github.com/users/%s/events"
username = "bottlenome"
events = get_json(url%username)

if len(events) == 0:
    print "no pushs for past 90 days"
else:
    push_event_num = 0
    commits_num = 0
    changed_lines = 0
    for i in events:
        if i["type"] == "PushEvent":
            push_event_num += 1
            commits_num += len(i["payload"]["commits"])
            for j in i["payload"]["commits"]:
                commit_detail = get_json(j["url"])
                changed_lines += commit_detail["stats"]["total"]
    print "push events:%d"%push_event_num
    print "commits :%d"%commits_num
    print "lines :%d, average per 90 days :%f"%(changed_lines, changed_lines/90.0)
