#!/usr/bin/python

import sys

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
		2: 0x02,
		3: 0x03
	}

	colorSymbol = rowStringPair[0] # NOTE: Assuming first character is the same as the second.
	colorCode = xpmPalette.colorSymbols[colorSymbol]

	return colorCodeToCrumb[colorCode]

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
