#!/usr/bin/python

import sys

import xpm2c_base
import xpm2c_c64

# TODO: Be able to specify this order in the future.
ColorIdNames = [
	"None",
	"white",
	"#3F3F3F",
	"gray50"
]

class XpmPalette:
	def __init__(self):
		self.colorSymbols = {}

class XpmHeaderData: 
	def __init__(self): 
		self.width = 0
		self.height = 0
		self.palette = XpmPalette()

class XpmImageData:
	def __init__(self):
		self.rows = []

class XpmData:
	def __init__(self):
		self.headerData = XpmHeaderData()
		self.imageData = XpmImageData()

def GetXpmQuotedLineString(line):
	firstQuoteIndex = line.index('\"')
	if firstQuoteIndex < 0:
		return ""
	
	return line[firstQuoteIndex + 1:line.rindex('\"') + 1 - 1]

def ReadXpmHeaderData(inputFile):
	inputFile.readline() # /* XPM */
	inputFile.readline() # static char *dwarf_base[] = {
	inputFile.readline() # /* columns rows colors chars-per-pixel */
	
	infoString = GetXpmQuotedLineString(inputFile.readline()) # "128 128 4 1 ",
	info = infoString.split()

	xpmHeaderData = XpmHeaderData()
	xpmHeaderData.width = int(info[0])
	xpmHeaderData.height = int(info[1])
	
	numberOfColors = int(info[2])
	for i in range(numberOfColors):
		paletteEntryString = GetXpmQuotedLineString(inputFile.readline()) # "  c #3F3F3F",
		paletteEntry = paletteEntryString.split(" c ")
		colorSymbol = paletteEntry[0]
		colorIdName = paletteEntry[1]
		xpmHeaderData.palette.colorSymbols[colorSymbol] = ColorIdNames.index(colorIdName)
	
	return xpmHeaderData

def ReadXpmImageData(inputFile, xpmHeaderData):
	xpmImageData = XpmImageData()
	
	inputFile.readline() # /* pixels */

	for _i in range(xpmHeaderData.height):
		line = inputFile.readline() # "ooo",
		if line == None:
			break
		xpmImageData.rows.append(GetXpmQuotedLineString(line))
	
	return xpmImageData

def GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight, type):
	spriteFrameRows = []
	
	if type == "base":
		spriteFrameRows = GenerateSpriteFrameRowsFromXpmBase(xpmData, x, y, spriteFrameWidth, spriteFrameHeight)
	elif type == "c64":
		spriteFrameRows = GenerateSpriteFrameRowsFromXpmC64(xpmData, x, y, spriteFrameWidth, spriteFrameHeight)

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
		PrintNextSpriteFrameBase(spriteFrame)
	elif type == "c64":
		PrintNextSpriteFrameC64(spriteFrame)

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

	xpmData = XpmData()
	xpmData.headerData = ReadXpmHeaderData(inputFile)
	xpmData.imageData = ReadXpmImageData(inputFile, xpmData.headerData)

	PrintAllSpriteFrames(xpmData, 16, 16, type)

	print("\n};")

	inputFile.close()
