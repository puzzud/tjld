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

def GenerateSpriteFrameRowsFromXpm(xpmData, x, y, spriteFrameWidth, spriteFrameHeight, type):
	spriteFrameRows = []
	
	if type == "base":
		spriteFrameRows = GenerateSpriteFrameRowsFromXpmBase(xpmData, x, y, spriteFrameWidth, spriteFrameHeight)
	elif type == "c64":
		spriteFrameRows = GenerateSpriteFrameRowsFromXpmC64(xpmData, x, y, spriteFrameWidth, spriteFrameHeight)

	return spriteFrameRows

def GenerateSpriteFrameRowsFromXpmBase(xpmData, x, y, spriteFrameWidth, spriteFrameHeight):
	pixelX = (x * spriteFrameWidth) + x + 1
	pixelY = (y * spriteFrameHeight) + y + 1

	spriteFrameRows = []
	for rowIndex in range(0, spriteFrameHeight):
		rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + spriteFrameWidth]
		spriteFrameRows.append(GenerateRowFromRowStringBase(rowString, xpmData.headerData.palette))

	return spriteFrameRows

def GenerateSpriteFrameRowsFromXpmC64(xpmData, x, y, spriteFrameWidth, spriteFrameHeight):
	pixelX = (x * spriteFrameWidth) + x + 1
	pixelY = (y * spriteFrameHeight) + y + 1

	spriteFrameRows = []
	for rowIndex in range(0, spriteFrameHeight):
		rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + spriteFrameWidth]
		spriteFrameRows += GenerateRowFromRowStringC64(rowString, xpmData.headerData.palette)

	# 21 C64 sprite height.
	for rowIndex in range(21 - spriteFrameHeight):
		row = [0, 0, 0]
		spriteFrameRows += row
	
	spriteFrameRows.append(0) # Last unused byte.

	return spriteFrameRows

def GenerateRowFromRowStringBase(rowString, xpmPalette):
	row = []

	for colorSymbol in rowString:
		row.append(xpmPalette.colorSymbols[colorSymbol])

	return row

def GenerateRowFromRowStringC64(rowString, xpmPalette):
	row = []

	rowString += "".zfill(24 - len(rowString)).replace('0', 'o')

	while len(rowString) > 0:
		rowStringByte = rowString[0:8]
		row.append(GetByteFromRowStringByteC64(rowStringByte, xpmPalette))

		rowString = rowString[8:len(rowString)]

	return row

def GetByteFromRowStringByteC64(rowStringByte, xpmPalette):
	byte = 0x00
	
	while len(rowStringByte) > 0:
		rowStringPair = rowStringByte[0:2]

		byte <<= 2
		byte += (GetByteFromRowStringPairC64(rowStringPair, xpmPalette))

		rowStringByte = rowStringByte[2:len(rowStringByte)]
	
	return byte

def GetByteFromRowStringPairC64(rowStringPair, xpmPalette):
	colorCodeToCrumb = {
		0: 0x00,
		1: 0x01,
		2: 0x03,
		3: 0x02
	}

	colorSymbol = rowStringPair[0] # NOTE: Assuming first character is the same as the second.
	colorCode = xpmPalette.colorSymbols[colorSymbol]

	return colorCodeToCrumb[colorCode]

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

def PrintNextSpriteFrameBase(spriteFrame):
	print("\t{")

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

def PrintNextSpriteFrameC64(spriteFrame):
	minimumLineLength = 16
	
	print("\t{")

	numberOfRows = 21

	for i in range(0, numberOfRows):
		row = []
		if len(spriteFrame) > 0:
			row = spriteFrame[0:3]
			spriteFrame = spriteFrame[3:len(spriteFrame) - 1]
		else:
			row = [0, 0, 0]
		
		previewString = GetPreviewStringFromRowC64(row)

		sys.stdout.write("\t\t")

		line = str(row).replace('[', '').replace(']', '') + ","
		line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
		line += " // " + previewString
		print(line)

	sys.stdout.write("\t\t")

	line = "0"
	line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
	line += " // " + "Unused byte."
	print(line)

	sys.stdout.write("\t}")

def GetPreviewStringFromRowC64(row):
	previewString = ""
	
	for byte in row:
		previewString += '{0:08b}'.format(byte)

	previewString = ("|" + previewString + "|")

	return previewString

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
