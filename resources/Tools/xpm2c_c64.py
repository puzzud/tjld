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

	def GenerateFrameRowsFromXpm(self, xpmData, x, y):
		pixelX = (x * self.frameWidth) + x + 1
		pixelY = (y * self.frameHeight) + y + 1

		frameRows = []
		for rowIndex in range(0, self.frameHeight):
			rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + self.frameWidth]
			frameRows += self.GenerateRowFromRowString(rowString, xpmData.headerData.palette)

		if self.type == "spr":
			# 21 C64 sprite height.
			for rowIndex in range(21 - self.frameHeight):
				row = [0, 0, 0]
				frameRows += row
				
				frameRows.append(0) # Last unused byte.

		return frameRows

	def GenerateRowFromRowString(self, rowString, xpmPalette):
		row = []

		rowString += "".zfill(24 - len(rowString)).replace('0', 'o')

		while len(rowString) > 0:
			rowStringByte = rowString[0:8]
			row.append(self.GetByteFromRowStringByte(rowStringByte, xpmPalette))

			rowString = rowString[8:len(rowString)]

		return row

	def GetByteFromRowStringByte(self, rowStringByte, xpmPalette):
		byte = 0x00
		
		while len(rowStringByte) > 0:
			rowStringPair = rowStringByte[0:2]

			byte <<= 2
			byte += (self.GetByteFromRowStringPair(rowStringPair, xpmPalette))

			rowStringByte = rowStringByte[2:len(rowStringByte)]
		
		return byte

	def GetByteFromRowStringPair(self, rowStringPair, xpmPalette):
		colorCodeToCrumb = {
			0: 0x00,
			1: 0x01,
			2: 0x02,
			3: 0x03
		}

		colorSymbol = rowStringPair[0] # NOTE: Assuming first character is the same as the second.
		colorCode = xpmPalette.colorSymbols[colorSymbol]

		return colorCodeToCrumb[colorCode]

	def PrintNextFrame(self, frame):
		minimumLineLength = 16
		
		print("\t{")

		numberOfRows = self.frameHeight
		if self.type == "spr":
			numberOfRows = 21

		for i in range(0, numberOfRows):
			i = i # pylint.

			row = []
			if len(frame) > 0:
				row = frame[0:3]
				frame = frame[3:len(frame) - 1]
			else:
				row = [0, 0, 0]
			
			previewString = self.GetPreviewStringFromRow(row)

			sys.stdout.write("\t\t")

			line = str(row).replace('[', '').replace(']', '') + ","
			line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
			line += " // " + previewString
			print(line)

		sys.stdout.write("\t\t")

		if self.type == "spr":
			line = "0"
			line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
			line += " // " + "Unused byte."
			print(line)

			sys.stdout.write("\t}")

	def GetPreviewStringFromRow(self, row):
		previewString = ""
		
		for byte in row:
			previewString += '{0:08b}'.format(byte)

		previewString = ("|" + previewString + "|")

		return previewString
