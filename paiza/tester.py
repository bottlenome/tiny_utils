from subprocess import Popen, PIPE

f = open("testcase")

for line in f:
  p = Popen(["python","submit.py"], stdin=PIPE)
  p.stdin.write(line)
  p.wait()
  
f.close()

