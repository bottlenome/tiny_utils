import os

direct = "/result/"
dirNum = 0
result = ".res"
hoge = ".hoge"
dir = os.path.abspath("./")
ans = "./ans.txt"
probrems = "./problems.txt"
lineMax = 220

f = open(probrems, 'r')
line = f.readline()
LRUD = line.split(' ')
print "L:" + LRUD[0] + ",R:" + LRUD[1] + ",U:" + LRUD[2] + ",D:" + LRUD[3]
for i in range(len(LRUD)):
  LRUD[i] = int(LRUD[i])
f.close()

count = {"L":0, "R":0, "U":0, "D":0}
solved = 0
f = open(ans, 'w')
for i in range(1, 5001):
  if(i % 1000 == 1):
    dirNum += 1000
  filePath = dir + direct + str(dirNum) + "/" + str(i) + result
  if(os.path.exists(filePath)):
    af = open(filePath, 'r')
    line = af.readline()
    af.close()
    if(len(line) != 0):
      if(len(line) < lineMax):
        f.write(line)
        solved += 1
        for j in count:
          count[j] += line.count(j)
      else:
        os.rename(filePath, dir + direct + str(dirNum) + "/" + str(i) + ".hoge")
    else:
      f.write('\n')
  else:
    f.write('\n')  
f.close()

for i in count:
  print i,count[i]
print "solved:", solved

