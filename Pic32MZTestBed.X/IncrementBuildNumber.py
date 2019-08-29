# File:   IncrementBuildNumber.py
# Author: Taylor Robbins
# Date:   01\19\2018
# Description: 
# 	Opens a file, searches it for a define by name and increments it's numeric value 

import re
import os
import sys
import time

ColorBlack         = 0
ColorRed           = 1
ColorGreen         = 2
ColorYellow        = 3
ColorBlue          = 4
ColorMagenta       = 5
ColorCyan          = 6
ColorWhite         = 7
ColorBrightBlack   = 8
ColorBrightRed     = 9
ColorBrightGreen   = 10
ColorBrightYellow  = 11
ColorBrightBlue    = 12
ColorBrightMagenta = 13
ColorBrightCyan    = 14
ColorBrightWhite   = 15

def SetForegroundColor(colorCode):
#
	if (colorCode < 8):    sys.stdout.write("\x1B[%um" % (colorCode+30))
	elif (colorCode < 16): sys.stdout.write("\x1B[%um" % (colorCode+90))
#
def SetBackgroundColor(colorCode):
#
	if (colorCode < 8):    sys.stdout.write("\x1B[%um" % (colorCode+40))
	elif (colorCode < 16): sys.stdout.write("\x1B[%um" % (colorCode+100))
#
def SetDefaultColors():
#
	sys.stdout.write("\x1B[0m")
#
def PrintTitle(string):
#
	SetBackgroundColor(ColorWhite)
	SetForegroundColor(ColorBlack)
	print(string)
	SetDefaultColors()
#
def PrintSuccess(string):
#
	SetBackgroundColor(ColorGreen)
	SetForegroundColor(ColorBlack)
	print(string)
	SetDefaultColors()
#
def PrintError(string):
#
	SetBackgroundColor(ColorRed)
	SetForegroundColor(ColorRed)
	print(string)
	SetDefaultColors()
#

PrintTitle("===== Running IncrementVersionNumber.py =====")
# print("Passed %u arguments: %s" % (len(sys.argv), str(sys.argv)))

if (len(sys.argv) < 3 or len(sys.argv[1]) == 0 or len(sys.argv[2]) == 0):
#
	PrintError("Usage: IncrementBuildNumber.py \"path/to/version.h\" DEFINE_NAME")
	exit()
#

versionFilePath    = sys.argv[1]
defineName         = sys.argv[2]
tempFilePath       = versionFilePath + ".temp"
searchRegex        = '\\#define\\s+' + defineName + '\\s+([0-9]+)'
versionFile        = None
foundVersionNumber = False
newVersionNumber   = ""
fileContents = ""
newFileContents = ""

try:
#
	versionFile = open(versionFilePath, "r")
#
except IOError:
#
	PrintError("Could not open \"%s\" to increment build number" % (versionFilePath))
#

if (versionFile != None):
#
	fileContents = versionFile.read()
	versionFile.close()
#

if (len(fileContents) > 0):
#
	searchResult = re.search(searchRegex, fileContents);
	if (searchResult == None):
	#
		PrintError("Search failed with no matches using regex: \"%s\"" % (searchRegex))
	#
	elif (len(searchResult.groups()) < 1):
	#
		PrintError("Regex only returned %u groups" % (len(searchResult.groups())))
	#
	else:
	#
		currentVersionStr = searchResult.group(1)
		currentVersionNumber = int(currentVersionStr, 10)
		print("Current version is %u" % (currentVersionNumber))
		
		foundVersionNumber = True
		newVersionNumber = currentVersionNumber + 1
		newFileContents = fileContents[:searchResult.start(1)] + str(newVersionNumber) + fileContents[searchResult.end(1):]
		# print("New File Contents: \"%s\"" % (newFileContents))
	#
	
	tempFile = open(tempFilePath, "w")
	tempFile.write(newFileContents)
	tempFile.close()
	
	os.remove(versionFilePath)
	time.sleep(0.1)
	while (True):
	#
		try:
		#
			os.rename(tempFilePath, versionFilePath)
			break
		#
		except WindowsError as wErr:
		#
			print("Trying to rename temp file...")
			continue
		#
		except Exception as exc:
		#
			PrintError("Error while renaming temp file: %s" % (exc))
			break
		#
	#
#

if (foundVersionNumber):
#
	PrintSuccess("\"%s\" incremented to %u" % (defineName, newVersionNumber))
#
else:
#
	PrintError("Could not find \"%s\" define in \"%s\". Build number not incremented" % (defineName, versionFilePath))
#

PrintTitle("===== Finished IncrementVersionNumber.py =====")
