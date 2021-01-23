#!/usr/bin/python

import sys

import xpm
import xpm2c_base
import xpm2c_c64

def GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight, type):
	spriteFrameRows = []
	
	if type == "base":
		spriteFrameRows = xpm2c_base.GenerateSpriteFrameRowsFromXpmBase(xpmData, x, y, spriteFrameWidth, spriteFrameHeight)
	elif type == "c64":
		spriteFrameRows = xpm2c_c64.GenerateSpriteFrameRowsFromXpmC64(xpmData, x, y, spriteFrameWidth, spriteFrameHeight)

	return spriteFrameRows

def PrintAllSpriteFrames(xpmData, spriteFrameWidth, spriteFrameHeight, type):
	spriteSheetFrameWidth = int((xpmData.headerData.width - 2) / spriteFrameWidth) # 2 for grid.
	spriteSheetFrameHeight = int((xpmData.headerData.height - 2) / spriteFrameHeight)

	spriteFrames = []

	for y in range(spriteSheetFrameHeight):
		for x in range(spriteSheetFrameWidth):
			spriteFrames.append(GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight, type))
	
	for i in range(0, len(spriteFrames)):
		PrintNextSpriteFrame(spriteFrames[i], type)
		if i != len(spriteFrames) - 1:
			print(",")

def PrintNextSpriteFrame(spriteFrame, type):
	if type == "base":
		xpm2c_base.PrintNextSpriteFrameBase(spriteFrame)
	elif type == "c64":
		xpm2c_c64.PrintNextSpriteFrameC64(spriteFrame)

###################################################################
# Main
###################################################################

if len(sys.argv) != 3:
	print("xpm2c.py <input file> <type>")
	sys.exit(1)

inputFileName = str(sys.argv[1])
type = str(sys.argv[2])

# Read the entire file as a single byte string
inputFile = None

try:
	inputFile = open(inputFileName, 'r')
except IOError:
	print("Input file does not exist or cannot be opened.")
	sys.exit(2)

if inputFile:
	print("{")

	xpmData = xpm.XpmData()
	xpmData.headerData = xpm.ReadXpmHeaderData(inputFile)
	xpmData.imageData = xpm.ReadXpmImageData(inputFile, xpmData.headerData)

	PrintAllSpriteFrames(xpmData, 16, 16, type)

	print("\n};")

	inputFile.close()
