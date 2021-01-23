#!/usr/bin/python

import sys

def GenerateSpriteFrameRowsFromXpmBase(xpmData, x, y, spriteFrameWidth, spriteFrameHeight):
	pixelX = (x * spriteFrameWidth) + x + 1
	pixelY = (y * spriteFrameHeight) + y + 1

	spriteFrameRows = []
	for rowIndex in range(0, spriteFrameHeight):
		rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + spriteFrameWidth]
		spriteFrameRows.append(GenerateRowFromRowStringBase(rowString, xpmData.headerData.palette))

	return spriteFrameRows

def GenerateRowFromRowStringBase(rowString, xpmPalette):
	row = []

	for colorSymbol in rowString:
		row.append(xpmPalette.colorSymbols[colorSymbol])

	return row

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
