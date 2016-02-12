import urllib2
import json
import datetime

url = "https://api.github.com/users/%s/events"
username = "bottlenome"
mail_addr = "bottlenome@gmail.com"

def get_json(url):
    response = urllib2.urlopen(url)
    ret = response.read()
    json_object = json.loads(ret)
    return json_object

events = get_json(url%username)

log = ""

if len(events) == 0:
    log += "no pushs for past 90 days\n"
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
    log += "90days summary\n"
    log += "push events:%d\n"%push_event_num
    log += "commits :%d\n"%commits_num
    log += "total lines :%d, %.1f lines / month\n"%(changed_lines, changed_lines/3.0)

def get_mail_format(log):
    template = """From: urota@mcmp.dip.jp
To: %s
Subject: git hub status %s

%s
."""
    day = datetime.datetime.now()
    return template%(mail_addr, day.strftime("%Y/%m/%d %H:%M"), log)

print get_mail_format(log)

