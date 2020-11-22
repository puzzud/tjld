#!/usr/bin/python

import sys

ColorIdNames = [
	"None",
	"white",
	"gray50",
	"#3F3F3F"
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

def GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight):
	pixelX = (x * spriteFrameWidth) + x + 1
	pixelY = (y * spriteFrameHeight) + y + 1

	spriteFrameRows = []
	for rowIndex in range(0, spriteFrameHeight):
		rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + spriteFrameWidth]
		spriteFrameRows.append(GenerateRowFromRowString(rowString, xpmData.headerData.palette))

	return spriteFrameRows

def GenerateRowFromRowString(rowString, xpmPalette):
	row = []

	for colorSymbol in rowString:
		row.append(xpmPalette.colorSymbols[colorSymbol])

	return row

def PrintAllSpriteFrames(xpmData, spriteFrameWidth, spriteFrameHeight):
	spriteSheetFrameWidth = int((xpmData.headerData.width - 2) / spriteFrameWidth) # 2 for grid.
	spriteSheetFrameHeight = int((xpmData.headerData.height - 2) / spriteFrameHeight)

	spriteFrames = []

	for y in range(spriteSheetFrameHeight):
		for x in range(spriteSheetFrameWidth):
			spriteFrames.append(GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight))
	
	for i in range(0, len(spriteFrames)):
		PrintNextSpriteFrame(spriteFrames[i])
		if i != len(spriteFrames) - 1:
			print(",")

def PrintNextSpriteFrame(spriteFrame):
	print "\t{"

	numberOfRows = len(spriteFrame)

	for i in range(0, numberOfRows):
		row = spriteFrame[i]
		
		previewString = "|" + str(row).replace('[', '').replace(']', '').replace(", ", '').replace('0', '.') + "|"

		sys.stdout.write("\t\t")
		sys.stdout.write(str(row).replace('[', '{').replace(']', '}'))

		if i == numberOfRows - 1:
			sys.stdout.write("")
		else:
			sys.stdout.write(",")
		
		print("\t// " + previewString)

	sys.stdout.write("\t}")

if len(sys.argv) != 2:
	print "Only supply a single parameter with path to a sprite sheet XPM file."
	sys.exit(1)

inputFileName = str(sys.argv[1])

# Read the entire file as a single byte string
inputFile = None

try:
	inputFile = open(inputFileName, 'rb')
except IOError:
	print "Input file does not exist or cannot be opened."
	sys.exit(2)

if inputFile:
	print "{"

	xpmData = XpmData()
	xpmData.headerData = ReadXpmHeaderData(inputFile)
	xpmData.imageData = ReadXpmImageData(inputFile, xpmData.headerData)

	PrintAllSpriteFrames(xpmData, 16, 16)

	print "\n};"

	inputFile.close()
