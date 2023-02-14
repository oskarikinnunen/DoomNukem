#! /usr/bin/env python3

import sys
import shutil

#print ('Number of arguments:', len(sys.argv), 'arguments.')
#print ('Argument List:', str(sys.argv))


if len(sys.argv) != 2:
	print("Usage:")
	print("  ./createcomponent.py newcomponentname")
	exit()

def backup():
	shutil.copyfile("src/entity/components/component_definitions.c", ".createcomponent_data/bu/component_definitions.c.backup")
	shutil.copyfile("include/components.h", ".createcomponent_data/bu/components.h.backup")
	shutil.copyfile("Makefile", ".createcomponent_data/bu/Makefile.backup")
	#f = open('src/entity/components/component_definitions.c')

header_str="typedef struct	s_COMPONENTNAME\n{\n\tfloat	TESTVARIABLE;\n}\tt_COMPONENTNAME;\n"
func_str="void	assign_component_COMPONENTNAME(t_component *component);\n"

def createheader(componentname):
	newfile_lines = []
	prevline = ""
	f = open("include/components.h")
	for line in f:
		if "\tCOMP_" in line and "," not in line:
			newfile_lines.append(line.replace("\n", ",\n"))
			newfile_lines.append("\tCOMP_" + componentname.upper() + "\n")
		elif "STRUCTDEF END" in line:
			newfile_lines.append(header_str.replace("COMPONENTNAME", componentname))
			newfile_lines.append(line)
		elif "ASSIGNFUNC END" in line:
			newfile_lines.append(func_str.replace("COMPONENTNAME", componentname))
			newfile_lines.append(line)
		else:
			newfile_lines.append(line)
	f.close()
	f = open(".createcomponent_data/temp_components.h", "w")
	for newline in newfile_lines:
		f.write(newline)
	f.close()

def createcomponentfile(componentname):
	newfile_lines = []
	f = open('.createcomponent_data/comp_placeholder.c')
	for line in f:
		newfile_lines.append(line.replace("COMPONENTNAME", componentname))
	f.close()
	f = open(".createcomponent_data/temp_comp_" + componentname + ".c", "w")
	for newline in newfile_lines:
		f.write(newline)
	f.close()
	f = open("src/entity/components/comp_" + componentname + ".c", "w")
	for newline in newfile_lines:
		f.write(newline)
	f.close()

compdefstr = "\t\t(t_componentdefinition){.type = COMP_COMPONENTNAME, .name = \"COMPONENTNAME\",\n\t\t\t.func_assign_component = assign_component_COMPONENTNAME}\n"

makefile_marker = "#.ENDSRC. CREATECOMPONENT SCRIPT DEPENDS ON THIS SO DONT REMOVE"

def createcomponentdefinition(componentname):
	newfile_lines = []
	f = open('src/entity/components/component_definitions.c')
	found_arraystart = False
	inserted = False
	for line in f:
		modifiedline = line
		
		if "static	t_componentdefinition editables[32] =" in line:
			found_arraystart = True
		if found_arraystart and "}" in line and "," not in line and not inserted:
			#print(line)
			modifiedline = line.replace('\n', ',\n')
			newfile_lines.append(modifiedline)
			newcompdef = compdefstr.replace('.type = COMP_COMPONENTNAME', ".type = COMP_" + componentname.upper())
			newcompdef = newcompdef.replace('.name = \"COMPONENTNAME\"', ".name = \"" + componentname.capitalize()+"\"")
			newcompdef = newcompdef.replace('COMPONENTNAME', componentname.lower())
			newfile_lines.append(newcompdef)
			inserted = True
		else:
			newfile_lines.append(line)
	f.close()
	f = open(".createcomponent_data/temp_component_definitions.c", "w")
	for newline in newfile_lines:
		f.write(newline)
	f.close()

def createmakefile(componentname):
	newfile_lines = []
	f = open('Makefile')
	for line in f:
		if "#.ENDSRC." in line:
			newfile_lines.append(line.split(" #.ENDSRC.")[0] + " " + ("\ \n").replace(" ", ""))
			newfile_lines.append("\t\tentity/components/comp_" + componentname + ".c " + makefile_marker + "\n")
		else:
			newfile_lines.append(line)
	f.close()
	f = open(".createcomponent_data/temp_Makefile", "w")
	for newline in newfile_lines:
		f.write(newline)
	f.close()


if len(sys.argv) == 2:
	componentname = sys.argv[1].lower()
	backup()
	print("Create component: ", componentname)
	createheader(componentname)
	createcomponentfile(componentname)
	createcomponentdefinition(componentname)
	createmakefile(componentname)
	shutil.copyfile(".createcomponent_data/temp_component_definitions.c", "src/entity/components/component_definitions.c")
	shutil.copyfile(".createcomponent_data/temp_components.h", "include/components.h")
	shutil.copyfile(".createcomponent_data/temp_Makefile", "Makefile")


