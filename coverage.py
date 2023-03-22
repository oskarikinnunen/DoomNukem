#! /usr/bin/env python3

import os

stream = os.popen('find src/ | grep "\.c" | wc -l')
filecount = int(stream.read())
stream = os.popen('norminette src | grep "OK" | wc -l')
norm_filecount = int(stream.read())
print(str(norm_filecount) + "/" + str(filecount) + " files are norminette compliant.")
#norm_filecount = 100
percentage = (float(norm_filecount) / float(filecount)) * 100.0
percentage = round(percentage, 2)
print(str(percentage) + "% done")