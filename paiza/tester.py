from subprocess import Popen, PIPE
import os


def execute(fileName):
  f = open(fileName)
  inputs = f.read()
  
  p = Popen(["python","submit.py"], stdin=PIPE)
  p.stdin.write(inputs)
  p.wait()
    
  f.close()

files = os.listdir("./")

for fileName in files:
  if(fileName.find("testcase") != -1):
    execute(fileName)


