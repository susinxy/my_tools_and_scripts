#!/bin/sh
gengen -i struct_def.txt --file-name struct_def.h --gen-name struct_def
gengen -i members_gen.txt --file-name members_gen.h --gen-name members_gen 
gcc SCMPrint.cpp -o SCMPrint -lxerces-c -lstdc++
