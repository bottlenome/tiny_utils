#!/usr/bin/env python

import numpy
from PIL import Image

class Position():
  def __init__(self, x, y, z):
    self.x = x
    self.y = y
    self.z = z

class Position2D():
  def __init__(self, x, y):
    self.x = x
    self.y = y
    self.np = numpy.array([self.x, self.y])

class Point():
  def __init__(self, momentOfInertia = None):
    self.momentOfInertia = momentOfInertia 
    self.position = None
  
  def setPosition(self, position):
    self.position = position 

  def getPosition(self):
    return self.position

class Structure():
  def __init__(self):
    self.pointDistance = 1
    self.pointNum = 0
    self.points = []

  def addPoint(self, point):
    self.points.append(point)
    self.pointNum += 1

  def removePoint(self, point, index):
    self.points.pop(index)
    self.pointNum -= 1

  def getPoints(self):
    return self.points

  def getPoint(self, index):
    return self.points[index]

class World2D():
  def __init__(self, windowWidth, windowHeight):
    self.windowWidth = windowWidth
    self.windowHeight = windowHeight
    self.image = numpy.asarray([[[255, 255, 255, 1.0]] * windowWidth] * windowHeight);
    self.image.flags.writeable = True
    self.dim = 2
    self.structures = []

  def show(self):
    img = Image.fromarray(numpy.uint8(self.image))
    #TODO null structures
    print self.structures
    for i in self.structures:
      drawStructure(i)
    img.show()

  def addStructure(self, structure):
    self.structures.append(structure)

  def drawStructure(self, structure):
    dr = imageDraw.Draw(self.image)
    for i in structure.getPoints():
      position = i.getPosition()
      if position != None:
        x = position.x
        y = position.y
        dr.ellipse(((x - 10, y - 10), (x + 10, y + 10)), outline=(0, 0, 0), fill = (0, 0, 0))

class Solver():
  def __init__(self, world):
    self.world = world
    self.alpha = 0.01
    self.epsilon = 0.01
    self.PositionDistance = 1
    self.pointNum = 3
    self.startPositions = None
    self.finalPositions = None
    self.vector = numpy.pi / 180

  def setPositionNum(self, num):
    self.pointNum = num

  def setPositionDistance(self, distance):
    self.PositionDistance = distance

  def calcCost(self, expectation):
    cost = 0.0
    print "TODO"
    return cost

  #TODO remove or not
  def addStartPosition(self, point):
    self.startPosition = point
  
  #TODO remove or not
  def addStartPosition(self, position):
    self.startPositions.append(position)
  
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

  def addFinalPosition(self, position):
    self.startPositions.append(position)

  def solve(self):
    if self.startPositions == None:
      print "err before solve you must set startPositions"
      return 

    if self.finalPositions == None:
      print "err before solve you must set finalPositions"
      return 
    
    """
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
    """
    #set time of finalPosition
    time = 1.0
    dtime = 0.1
    #TODO
    cost = self.calcCost(expectation)
    while self.epsilon < cost:
      #reflesh
      for i in range(self.pointNum):
        for j in range(self.world.dim):
          expectation[i][j] += self.alpha * cost * expectation[i][j]
      cost = self.calcCost()

if __name__ == "__main__":
  structure = Structure()
  structure.addPoint(Point())
  structure.addPoint(Point())
  structure.addPoint(Point())
  structure.getPoint(0).setPosition(Position2D(100, 100))
  
  world = World2D(512, 512)
  world.show()
  solver = Solver(world)
  solver.setStartPosition(numpy.array([Position2D(100, 0), Position2D(100, 1), Position2D(101, 0)]))
  solver.setFinalPosition(numpy.array([Position2D(100, 0), Position2D(99, 0), Position2D(101, 0)]))
  solver.solve()


