
f = open('problems.txt', 'r')
i = 0
direct = "./result/"
dirNum = 1000
for line in f:
  i += 1
  if(i > 2):
    fileName=direct + str(dirNum) + "/" + str(i-2) + ".txt"
    gameFile = open(fileName,'w')
    gameFile.write(line)
    gameFile.close()
    if((i-2) % 1000 == 0):
      dirNum += 1000

f.close()

