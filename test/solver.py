#!/usr/bin/env python

import numpy
from PIL import Image

class Point():
  def __init__(self, x, y, z):
    self.x = x
    self.y = y
    self.z = z

class Point2D():
  def __init__(self, x, y):
    self.x = x
    self.y = y
    self.np = numpy.array([self.x, self.y])

class World2D():
  def __init__(self, windowWidth, windowHeight):
    self.windowWidth = windowWidth
    self.windowHeight = windowHeight
    self.image = numpy.asarray([[[255, 255, 255, 1.0]] * windowWidth] * windowHeight);
    self.image.flags.writeable = True
    self.dim = 2

  def show(self):
    img = Image.fromarray(numpy.uint8(self.image))
    img.show()

class Solver():
  def __init__(self, world):
    self.world = world
    self.alpha = 0.01
    self.epsilon = 0.01
    self.PointDistance = 1
    self.pointNum = 3
    self.startPositions = None
    self.finalPositions = None

  def setPointNum(self, num):
    self.pointNum = num

  def setPointDistance(self, distance):
    self.PointDistance = distance

  def calcCost(self, expectation):
    cost = 0.0
    print "TODO"
    return cost

  #TODO remove or not
  def addStartPosition(self, point):
    self.startPosition = point
  
  #TODO remove or not
  def addStartPositionOfSomePoint(self, point):
    self.startPositions.append(point)
  
  #TODO remove or not
  def addFinalPosition(self, point):
    if self.pointNum < self.pointNum:
      self.finalPosition.append(point)
    else:
      print "err too match final position"

  def setStartPosition(self, startPosition):
    self.startPositions = startPosition

  def setFinalPosition(self, finalPosition):
    self.finalPositions = finalPosition

  def addFinalPositionOfSomePoint(self, point):
    self.startPositions.append(point)

  def solve(self):
    if self.startPositions == None:
      print "err before solve you must set startPositions"
      return 

    if self.finalPositions == None:
      print "err before solve you must set finalPositions"
      return 
    
    #set inital shape of final Position
    expectation = numpy.array([[None, None]] * self.pointNum)
    #TODO set finalPosition to expectation

    for i in range(self.pointNum):
      if i < len(self.finalPositions):
        expectation.append(self.finalPositions[i][:])
      else:
        expectation.append([None, None])
      for j in range(self.world.dim):
        if(expectation[i][j] == None):
          expectation[i][j] = numpy.random.random() * (self.pointNum -  1)
    print expectation
    #TODO
    cost = self.calcCost(expectation)
    while self.epsilon < cost:
      #reflesh
      for i in range(self.pointNum):
        for j in range(self.world.dim):
          expectation[i][j] += self.alpha * cost * expectation[i][j]
      cost = self.calcCost()

if __name__ == "__main__":
  world = World2D(512, 512)
  solver = Solver(world)
  solver.setStartPosition(numpy.array([Point2D(100, 0), Point2D(100, 1), Point2D(101, 0)]))
  solver.setFinalPosition(numpy.array([Point2D(100, 0), Point2D(100, 1), Point2D(101, 0)]))
  solver.solve()


