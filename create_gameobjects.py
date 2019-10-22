#
# Create the an gameobjects definition file from the objects/directory structure.
# 
# Organizes the gameobject definitions into a single file, with declarations at the top,
# then structure definitions,
# then function definitions.

import os
import re

data_file_name = "gameobject_definitions"

gameobject_files = []
for filename in os.listdir("gameobjects"):
    if filename.endswith(".h"):
        gameobject_files.append((filename[:-2], open("gameobjects/" + filename, "r")))

declarations = ""
structure_definitions = ""
definitions = ""

for name, f in gameobject_files:
    lines = [line.strip() for line in f.readlines()]
    for line in lines:
        if re.match(r'void .*_init', line) or re.match(r'void .*_update', line):
            declarations += "{};\n".format(line)
        properties_struct_match = re.match(r'typedef struct (.*)_s', line)
        if properties_struct_match:
            declarations += "typedef struct {0}_s {0};\n".format(properties_struct_match.group(1))
    getting_structure_definition = False
    structure_definition = ""
    for line in lines:
        if not getting_structure_definition:
            if line == "// BEGIN_PROPERTIES":
                getting_structure_definition = True
            else:
                definitions += line + "\n"
        elif line == "// END_PROPERTIES":
            getting_structure_definition = False
        else:
            structure_definition += line + "\n"
    if structure_definition != "":
        structure_definitions += structure_definition
            
            
print(declarations)
print(structure_definitions)
print(definitions)
data_file = open(data_file_name, "w+")
data_file.write(declarations)
data_file.write(structure_definitions)
data_file.write(definitions)
