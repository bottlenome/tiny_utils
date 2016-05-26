import random

def createNumbers():
  numbers = []
  for i in range(12):
    numbers.append(random.randint(0, 9))
  return numbers

def isMyNumber(numbers):
  total = 0.0
  for (n, r) in enumerate(numbers[:-1], start=1):
    if 1 <= n and n <= 6:
      total += r * (n + 1)
    elif 7 <= n and n <= 11:
      total += r * (n - 5)
    else:
      print n
      assert(False)
  if (11 - int(total)%11) == numbers[-1]:
    return True
  else:
    return False

count = 0
trial = 100000
for i in range(trial):
  numbers = createNumbers()
  if isMyNumber(numbers):
    #print numbers
    count += 1

print "result %f"%(count/float(trial)*100.0)
