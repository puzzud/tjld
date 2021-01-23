#!/usr/bin/python

import sys

import xpm
import xpm2c_base
import xpm2c_c64

def PrintAllFrames(xpmData, frameWidth, frameHeight, xpm2CFacility):
	sheetFrameWidth = int((xpmData.headerData.width - 2) / frameWidth) # 2 for grid.
	sheetFrameHeight = int((xpmData.headerData.height - 2) / frameHeight)

	frames = []

	for y in range(sheetFrameHeight):
		for x in range(sheetFrameWidth):
			frames.append(xpm2CFacility.GenerateFrameRowsFromXpm(xpmData, x, y, frameWidth, frameHeight))
	
	for i in range(0, len(frames)):
		xpm2CFacility.PrintNextFrame(frames[i])
		if i != len(frames) - 1:
			print(",")

###################################################################
# Main
###################################################################

if len(sys.argv) != 4:
	print("xpm2c.py <input file> <target> <type>")
	sys.exit(1)

inputFileName = str(sys.argv[1])
target = str(sys.argv[2])
type = str(sys.argv[3])

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
		xpm2CFacility = xpm2c_base.Xpm2CFacility(type)
	elif target == "c64":
		xpm2CFacility = xpm2c_c64.Xpm2CFacility(type)
	
	if xpm2CFacility == None:
		print("Did not supply a valid target: " + target)
		sys.exit(3)

	xpmData = xpm.XpmData()
	xpmData.headerData = xpm.ReadXpmHeaderData(inputFile)
	xpmData.imageData = xpm.ReadXpmImageData(inputFile, xpmData.headerData)

	print("{")
	PrintAllFrames(xpmData, 16, 16, xpm2CFacility)
	print("\n};")

	inputFile.close()
