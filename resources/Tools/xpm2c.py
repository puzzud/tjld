#!/usr/bin/python

import sys

import xpm
import xpm2c_base
import xpm2c_c64

def PrintAllSpriteFrames(xpmData, spriteFrameWidth, spriteFrameHeight, xpm2CFacility):
	spriteSheetFrameWidth = int((xpmData.headerData.width - 2) / spriteFrameWidth) # 2 for grid.
	spriteSheetFrameHeight = int((xpmData.headerData.height - 2) / spriteFrameHeight)

	spriteFrames = []

	for y in range(spriteSheetFrameHeight):
		for x in range(spriteSheetFrameWidth):
			spriteFrames.append(xpm2CFacility.GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight))
	
	for i in range(0, len(spriteFrames)):
		xpm2CFacility.PrintNextSpriteFrame(spriteFrames[i])
		if i != len(spriteFrames) - 1:
			print(",")

###################################################################
# Main
###################################################################

if len(sys.argv) != 3:
	print("xpm2c.py <input file> <target>")
	sys.exit(1)

inputFileName = str(sys.argv[1])
target = str(sys.argv[2])

# Read the entire file as a single byte string
inputFile = None

try:
	inputFile = open(inputFileName, 'r')
except IOError:
	print("Input file does not exist or cannot be opened.")
	sys.exit(2)

if inputFile:
	xpm2CFacility = None
	if target == "base":
		xpm2CFacility = xpm2c_base.Xpm2CFacility()
	elif target == "c64":
		xpm2CFacility = xpm2c_c64.Xpm2CFacility()
	
	if xpm2CFacility == None:
		print("Did not supply a valid target: " + target)
		sys.exit(3)

	xpmData = xpm.XpmData()
	xpmData.headerData = xpm.ReadXpmHeaderData(inputFile)
	xpmData.imageData = xpm.ReadXpmImageData(inputFile, xpmData.headerData)

	print("{")
	PrintAllSpriteFrames(xpmData, 16, 16, xpm2CFacility)
	print("\n};")

	inputFile.close()
