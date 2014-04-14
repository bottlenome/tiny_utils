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
    self.startPositions = []
    self.finalPositions = []
    self.world = world
    self.alpha = 0.01
    self.epsilon = 0.01
    self.PointDistance = 1
    self.finalPosition = []
    self.pointNum = 3

  def setPointNum(self, num):
    self.pointNum = num
  
  def setPointDistance(self, distance):
    self.PointDistance = distance

  def calcCost(self, expectation):
    print "TODO"

  #TODO x,y to Point
  def addStartPosition(self, point):
    self.startPosition = point
  
  def addStartPositionOfSomePoint(self, point):
    self.startPositions.append(point)
  
  def addFinalPosition(self, point):
    if self.pointNum < len(self.finalPosition):
      self.finalPosition.append(point)
    else:
      print "err too match final position"
  
  def addFinalPositionOfSomePoint(self, point):
    self.startPositions.append(point)

  def solve(self):
    #set inital shape of final Position
    expectation = []
    for i in range(self.pointNum):
      if i < len(self.finalPosition):
        expectation.append(self.finalPosition[i][:])
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
  world.show()
  solver = Solver(world)
  solver.solve()


