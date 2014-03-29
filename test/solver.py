import numpy

class Solver():
  def __init__(self):
    self.startPositions = []
    self.finalPositions = []
    self.DIM = 2
    self.alpha = 0.01
    self.epsilon = 0.01

  def setPointNum(self, num):
    self.pointNum = num
  
  def setPointDistance(self, distance):
    self.PointDistance = distance

  #TODO x,y to Point
  def addStartPosition(self, x, y):
    self.startPosition = [x, y]
  
  def addStartPositionOfSomePoint(self, x, y):
    self.startPositions.append([x, y])
  
  def addFinalPosition(self, x, y):
    self.finalPosition = [x, y]
  
  def addFinalPositionOfSomePoint(self, x, y):
    self.startPositions.append([x,y])

  def solve(self):
    #set inital position of final Position
    expectation = []
    for i in range(self.pointNum):
      if i < len(self.finalPosition):
        expectation.append(self.finalPosition[i][:]):
      else:
        expectation.append([None, None])
      for j in range(self.DIM):
        if(expectation[i][j] == None):
          expectation[i][j] = numpy.random.random()
    
    #cost = calcCost()
    while self.epsilon < cost:
      #reflesh
      #expectation += self.alpha * cost * expectation
      cost = calcCost()

      



