#! /usr/bin/env python3

import os
import sys

stream = os.popen('find src/ | grep "\.c" | wc -l')
filecount = int(stream.read())
stream = os.popen('norminette src | grep "OK" | wc -l')
norm_filecount = int(stream.read())
print(str(norm_filecount) + "/" + str(filecount) + " files are norminette compliant.")
#norm_filecount = 100
percentage = (float(norm_filecount) / float(filecount)) * 100.0
percentage = round(percentage, 2)
print(str(percentage) + "% done")
if (len (sys.argv) > 1) :
	if (sys.argv[1] == "-v") :
		stream = os.popen('norminette src | grep "Error!"')
		print("Naughty noncompliant files: \n")
		print((str)(stream.read()))
	else :
		print("Wrong argument '" + sys.argv[1] + "': use '-v' for verbose printing (prints noncompliant filenames)")
