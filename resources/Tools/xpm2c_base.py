#!/usr/bin/python

import sys

class Xpm2CFacility:
	def __init__(self, type):
		self.type = type

		if type == "chr":
			self.frameWidth = 8
			self.frameHeight = 8
		elif self.type == "spr":
			self.frameWidth = 16
			self.frameHeight = 16

	def GenerateFramesFromXpmAtPoint(self, xpmData, x, y):
		pixelX = (x * self.frameWidth) + x + 1
		pixelY = (y * self.frameHeight) + y + 1

		frameRows = []
		for rowIndex in range(0, self.frameHeight):
			rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + self.frameWidth]
			frameRows.append(self.GenerateRowFromRowString(rowString, xpmData.headerData.palette))

		return [frameRows]

	def GenerateRowFromRowString(self, rowString, xpmPalette):
		row = []

		for colorSymbol in rowString:
			row.append(xpmPalette.colorSymbols[colorSymbol])

		return row

	def PrintNextFrame(self, frame):
		print("\t{")

		numberOfRows = len(frame)

		for i in range(0, numberOfRows):
			row = frame[i]
			
			previewString = "|" + str(row).replace('[', '').replace(']', '').replace(", ", '').replace('0', '.') + "|"

			sys.stdout.write("\t\t")
			sys.stdout.write(str(row).replace('[', '{').replace(']', '}'))

			if i == numberOfRows - 1:
				sys.stdout.write("")
			else:
				sys.stdout.write(",")
			
			print("\t// " + previewString)

		sys.stdout.write("\t}")
