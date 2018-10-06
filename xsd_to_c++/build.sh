#!/bin/sh
script_dir=$(cd `dirname $0`;pwd)
gengen -i $script_dir/struct_def.txt --output-dir $script_dir --file-name struct_def.h --gen-name struct_def
gengen -i $script_dir/members_gen.txt --output-dir $script_dir --file-name members_gen.h --gen-name members_gen 
gcc $script_dir/SCMPrint.cpp -o $script_dir/SCMPrint -lxerces-c -lstdc++
