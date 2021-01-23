#!/usr/bin/python

import sys

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
