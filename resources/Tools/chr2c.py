#!/usr/bin/python

import sys

def PrintAllCharacters(inputFile):
	bytes = inputFile.read(8)
	while bytes:
		PrintNextCharacter(bytes)

		bytes = inputFile.read(8)
		if bytes:
			print(",")

def PrintNextCharacter(bytes):
	print("\t{")

	numberOfBytes = len(bytes)

	for i in range(0, numberOfBytes):
		byte = bytes[i]
		value = ord(byte)

		hexString = '0x{:02X}'.format(value)
		byteString = '|{0:08b}|'.format(value).replace('0', '.').replace('1', '*')

		sys.stdout.write("\t\t")
		sys.stdout.write(hexString)

		if i == numberOfBytes - 1:
			sys.stdout.write("")
		else:
			sys.stdout.write(",")
		
		print("\t// " + byteString)

	sys.stdout.write("\t}")

if len(sys.argv) != 2:
	print("Only supply a single parameter with path to a charset binary file.")
	sys.exit(1)

inputFileName = str(sys.argv[1])

# Read the entire file as a single byte string
inputFile = None

try:
	inputFile = open(inputFileName, 'rb')
except IOError:
	print("Input file does not exist or cannot be opened.")
	sys.exit(2)

if inputFile:
	print("{")
	PrintAllCharacters(inputFile)
	print("\n};")

	inputFile.close()
