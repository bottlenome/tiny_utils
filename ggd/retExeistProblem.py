import subprocess,os,time

waitTime = 2 * 60 * 10
waitCount = 0
i = 0
direct = "/result/"
dirNum = 0
txt = ".txt"
result = ".res"
DEAPTH = 15
CUTSIZE = 10
count = {"9":0, "12":0, "15":0, "18":0, "16":0, "20":0, "24":0, "25":0, "30":0, "36":0} 
answered = {"9":0, "12":0, "15":0, "18":0, "16":0, "20":0, "24":0, "25":0, "30":0, "36":0} 
under = 0
up = 0

progname = "/search"
dir = os.path.abspath("./")
progname = dir + progname

def returnSize(filePath):
  f = open(filePath, 'r')
  str = f.readline()
  f.close()
  return int(str[0]) * int(str[2])

for i in range(1, 5001):
  depth = DEAPTH
  cutSize = CUTSIZE
  if(i % 1000 == 1):
    dirNum += 1000 
  filePath = dir + direct + str(dirNum) + "/" + str(i) 
  loopCount = 0;
  num = returnSize(filePath + txt)
  count[str(num)] += 1
  if(os.path.exists(filePath + result)):
    answered[str(num)] += 1
  else:
    if(num <= 36):
      under += 1
      print num,i
    else:
      up += 1

print count
print answered

print "under",under
print "up",up

