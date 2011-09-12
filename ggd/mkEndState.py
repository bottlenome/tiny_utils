def insert(src, ins, pos):
  top = src[:pos]
  back = src[pos:]
  return top + [ins] + back

i = 0
direct = "./result/"
dirNum = 0
#for i in range(1, 5001):
for i in range(1, 2):
  if(i % 1000 == 1):
    dirNum += 1000
  fileName = direct + str(dirNum) + "/" + str(i)
  fileNameE = fileName + "E.txt"
  fileName = fileName + ".txt"
  f = open(fileName, 'r')
  line = f.readline()
  line = line[4:-1]
  j = 0
  equalPlace = []
  print list(line)
  for j in range(len(line)):
    if(line[j] == '='):
      equalPlace.append(j)
    j += 1
  nums = list(line.replace('=',''))
  nums.sort()
  nums = nums[1:] + [nums[0]]
  for j in equalPlace:
    nums.insert(j,"=")
  print nums
  #fe = open(fileNameE, 'w')
  f.close()


