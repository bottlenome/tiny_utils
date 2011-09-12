import subprocess,os,time

waitTime = 2 * 60 * 5
waitCount = 0
i = 0
direct = "/result/"
dirNum = 0
txt = ".txt"
result = ".res"
old = ".old"

progname = "/ansCheck"
dir = os.path.abspath("./")
progname = dir + progname

for i in range(1, 5001):
  if(i % 1000 == 1):
    dirNum += 1000 
  filePath = dir + direct + str(dirNum) + "/" + str(i) 
  if os.path.exists(filePath + result):
    f = open(filePath + result)
    line = f.readline()
    f.close()
    os.system(progname + " " + filePath+txt + " " + filePath+result + " " + filePath + old)

