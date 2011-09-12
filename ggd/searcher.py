import subprocess,os,time

waitTime = 2 * 60 * 10
waitCount = 0
i = 0
direct = "/result/"
dirNum = 5000
txt = ".txt"
result = ".res"
limit = 36
under = 30
DEAPTH = 15
CUTSIZE = 10

progname = "/wfs"
dir = os.path.abspath("./")
progname = dir + progname

def returnSize(filePath):
  f = open(filePath, 'r')
  str = f.readline()
  f.close()
  return int(str[0]) * int(str[2])

for i in range(4500, 5001):
  depth = DEAPTH
  cutSize = CUTSIZE
  if(i % 1000 == 1):
    dirNum += 1000 
  filePath = dir + direct + str(dirNum) + "/" + str(i) 
  loopCount = 0;
  size = returnSize(filePath + txt)
  while((under <= size and size <= limit) and (not os.path.exists(filePath + result))):
    waitCount = 0
    print i,"start depth=",depth,"cutSize=",cutSize
    p = subprocess.Popen([progname, filePath + txt , filePath + result, str(4000)])
    while p.poll() == None:
      if(waitCount > waitTime):
        print i,"kill"
        p.kill()
      time.sleep(0.5)
      waitCount += 1
    if(loopCount >= 20 or cutSize <= 2):
      break
    print "status:",p.poll()
    break;
    """
    if(p.poll() == 255):
      #noway
      if(waitCount < 5):
        depth += 5 - waitCount
      else:
        cutSize -= 1
      loopCount += 1
    else:
      #timeout
      break;
      #depth += 1
      #cutSize -= 1
    """

