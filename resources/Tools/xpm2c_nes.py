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

		frames = []

		if self.type == "chr":
			frames = [self.GenerateFrameFromXpmAtPoint(xpmData, pixelX, pixelY)]
		elif self.type == "spr":
			frames = [
				self.GenerateFrameFromXpmAtPoint(xpmData, pixelX, pixelY),
				self.GenerateFrameFromXpmAtPoint(xpmData, pixelX + 8, pixelY),
				self.GenerateFrameFromXpmAtPoint(xpmData, pixelX, pixelY + 8),
				self.GenerateFrameFromXpmAtPoint(xpmData, pixelX + 8, pixelY + 8)
			]

		return frames

	def GenerateFrameFromXpmAtPoint(self, xpmData, x, y):
		frameRows = []
		for rowIndex in range(0, self.frameHeight):
			rowString = xpmData.imageData.rows[y + rowIndex][x:x + self.frameWidth]
			row = self.GenerateRowFromRowString(rowString, xpmData.headerData.palette)
			frameRows.append(row)
		
		return frameRows

	def GenerateRowFromRowString(self, rowString, xpmPalette):
		row = []
		
		while len(rowString) > 0:
			rowStringOctet = rowString[0:8]

			# Row consists of 1 array and 2 separate bytes:
			#  1) Array of color codes of source frame row.
			#  2) Byte representing first bit plane of row.
			#  3) Byte representing second bit plane of row.
			colorCodeRow = self.GetColorCodeRowFromOctet(rowStringOctet, xpmPalette)
			row.append(colorCodeRow)
			row.append(self.GetByteFromColorCodeRow(colorCodeRow, 0))
			row.append(self.GetByteFromColorCodeRow(colorCodeRow, 1))

			rowString = rowString[8:len(rowString)]
		
		return row

	def GetColorCodeRowFromOctet(self, rowStringOctet, xpmPalette):
		colorCodeRow = []
		
		for colorSymbol in rowStringOctet:
			colorCode = xpmPalette.colorSymbols[colorSymbol]
			colorCodeRow.append(colorCode)
		
		return colorCodeRow

	def GetByteFromColorCodeRow(self, colorCodeRow, bitPlaneIndex):
		byte = 0x00

		for colorCode in colorCodeRow:
			byte <<= 1

			if (colorCode & (bitPlaneIndex + 1)) != 0:
				byte |= 0x01
		
		return byte

	def PrintNextFrame(self, frame):
		minimumLineLength = 16
		
		print("\t{")

		numberOfRows = 8 # NOTE: Always 8 for nes.

		for i in range(0, numberOfRows):
			row = frame[i]

			previewString = self.GetPreviewStringFromRow(row[0])

			sys.stdout.write("\t\t")

			line = str(row[1]).replace('[', '').replace(']', '') + ","
			line += "".zfill(minimumLineLength - len(line)).replace('0', ' ')
			line += " // " + previewString
			print(line)
		
		for i in range(0, numberOfRows):
			row = frame[i]

			sys.stdout.write("\t\t")

			line = str(row[2]).replace('[', '').replace(']', '') + ","
			print(line)

		sys.stdout.write("\t")
		sys.stdout.write("}")

	# NOTE: Same as xpm2c_base.
	def GetPreviewStringFromRow(self, row):
		previewString = "|" + str(row).replace('[', '').replace(']', '').replace(", ", '').replace('0', '.') + "|"
		return previewString
