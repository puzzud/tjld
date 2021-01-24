#!/usr/bin/python

import sys

class Xpm2CFacility:
	def __init__(self, type):
		self.type = type

		if type == "chr":
			self.frameWidth = 8
			self.frameHeight = 8
			self.bytesPerRow = 1
		elif self.type == "spr":
			self.frameWidth = 16
			self.frameHeight = 16
			self.bytesPerRow = 3

	def GenerateFrameRowsFromXpm(self, xpmData, x, y):
		pixelX = (x * self.frameWidth) + x + 1
		pixelY = (y * self.frameHeight) + y + 1

		frameRows = []
		for rowIndex in range(0, self.frameHeight):
			rowString = xpmData.imageData.rows[pixelY + rowIndex][pixelX:pixelX + self.frameWidth]
			row = self.GenerateRowFromRowString(rowString, xpmData.headerData.palette)
			frameRows.append(row)

		if self.type == "spr":
			# 21 C64 sprite height.
			for rowIndex in range(21 - self.frameHeight):
				row = [0, 0, 0]
				frameRows.append(row)
				
			frameRows.append([0]) # Last unused byte.
		
		return frameRows

	def GenerateRowFromRowString(self, rowString, xpmPalette):
		row = []
		
		# TODO: Need to ensure '0' to 'o' mapping is correct.
		# Probably not always the case.
		rowString += "".zfill((self.bytesPerRow * 8) - len(rowString)).replace('0', 'o')
		
		while len(rowString) > 0:
			rowStringOctet = rowString[0:8]
			row.append(self.GetByteFromRowOctet(rowStringOctet, xpmPalette))

			rowString = rowString[8:len(rowString)]
		
		return row

	def GetByteFromRowOctet(self, rowStringOctet, xpmPalette):
		byte = 0x00
		
		if self.type == "chr":
			# NOTE: Assumes monochrome.
			byte = self.GetByteFromRowOctetMonochrome(rowStringOctet, xpmPalette)
		elif self.type == "spr":
			# NOTE: Assumes multicolor.
			while len(rowStringOctet) > 0:
				rowStringPair = rowStringOctet[0:2]

				byte <<= 2
				byte += self.GetByteFromRowStringPairMultiColor(rowStringPair, xpmPalette)

				rowStringOctet = rowStringOctet[2:len(rowStringOctet)]
		
		return byte

	def GetByteFromRowOctetMonochrome(self, rowStringOctet, xpmPalette):
		byte = 0x00
		
		for i in range(8):
			colorSymbol = rowStringOctet[i]
			colorCode = xpmPalette.colorSymbols[colorSymbol]
			if colorCode != 0:
				colorCode = 1
			
			byte <<= 1
			byte |= colorCode

		return byte

	def GetByteFromRowStringPairMultiColor(self, rowStringPair, xpmPalette):
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
			row = frame[i]

			previewString = self.GetPreviewStringFromRow(row)

			sys.stdout.write("\t\t")

			line = str(row).replace('[', '').replace(']', '') + ","
			line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
			line += " // " + previewString
			print(line)

		sys.stdout.write("\t")

		if self.type == "spr":
			sys.stdout.write("\t")

			line = "0"
			line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
			line += " // " + "Unused byte."
			print(line)

			sys.stdout.write("\t}")
		else:
			sys.stdout.write("}")

	def GetPreviewStringFromRow(self, row):
		previewString = ""
		
		for byte in row:
			previewString += '{0:08b}'.format(byte)

		previewString = ("|" + previewString + "|")

		return previewString
