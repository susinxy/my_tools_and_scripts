# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zby/my_tools_and_scripts/log

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zby/my_tools_and_scripts/log/build

# Include any dependencies generated for this target.
include recorder/CMakeFiles/recorder.dir/depend.make

# Include the progress variables for this target.
include recorder/CMakeFiles/recorder.dir/progress.make

# Include the compile flags for this target's objects.
include recorder/CMakeFiles/recorder.dir/flags.make

recorder/CMakeFiles/recorder.dir/linkedlist.c.o: recorder/CMakeFiles/recorder.dir/flags.make
recorder/CMakeFiles/recorder.dir/linkedlist.c.o: ../recorder/linkedlist.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zby/my_tools_and_scripts/log/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object recorder/CMakeFiles/recorder.dir/linkedlist.c.o"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/recorder.dir/linkedlist.c.o   -c /home/zby/my_tools_and_scripts/log/recorder/linkedlist.c

recorder/CMakeFiles/recorder.dir/linkedlist.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/recorder.dir/linkedlist.c.i"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zby/my_tools_and_scripts/log/recorder/linkedlist.c > CMakeFiles/recorder.dir/linkedlist.c.i

recorder/CMakeFiles/recorder.dir/linkedlist.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/recorder.dir/linkedlist.c.s"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zby/my_tools_and_scripts/log/recorder/linkedlist.c -o CMakeFiles/recorder.dir/linkedlist.c.s

recorder/CMakeFiles/recorder.dir/linkedlist.c.o.requires:

.PHONY : recorder/CMakeFiles/recorder.dir/linkedlist.c.o.requires

recorder/CMakeFiles/recorder.dir/linkedlist.c.o.provides: recorder/CMakeFiles/recorder.dir/linkedlist.c.o.requires
	$(MAKE) -f recorder/CMakeFiles/recorder.dir/build.make recorder/CMakeFiles/recorder.dir/linkedlist.c.o.provides.build
.PHONY : recorder/CMakeFiles/recorder.dir/linkedlist.c.o.provides

recorder/CMakeFiles/recorder.dir/linkedlist.c.o.provides.build: recorder/CMakeFiles/recorder.dir/linkedlist.c.o


recorder/CMakeFiles/recorder.dir/recorder.c.o: recorder/CMakeFiles/recorder.dir/flags.make
recorder/CMakeFiles/recorder.dir/recorder.c.o: ../recorder/recorder.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zby/my_tools_and_scripts/log/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object recorder/CMakeFiles/recorder.dir/recorder.c.o"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/recorder.dir/recorder.c.o   -c /home/zby/my_tools_and_scripts/log/recorder/recorder.c

recorder/CMakeFiles/recorder.dir/recorder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/recorder.dir/recorder.c.i"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zby/my_tools_and_scripts/log/recorder/recorder.c > CMakeFiles/recorder.dir/recorder.c.i

recorder/CMakeFiles/recorder.dir/recorder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/recorder.dir/recorder.c.s"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zby/my_tools_and_scripts/log/recorder/recorder.c -o CMakeFiles/recorder.dir/recorder.c.s

recorder/CMakeFiles/recorder.dir/recorder.c.o.requires:

.PHONY : recorder/CMakeFiles/recorder.dir/recorder.c.o.requires

recorder/CMakeFiles/recorder.dir/recorder.c.o.provides: recorder/CMakeFiles/recorder.dir/recorder.c.o.requires
	$(MAKE) -f recorder/CMakeFiles/recorder.dir/build.make recorder/CMakeFiles/recorder.dir/recorder.c.o.provides.build
.PHONY : recorder/CMakeFiles/recorder.dir/recorder.c.o.provides

recorder/CMakeFiles/recorder.dir/recorder.c.o.provides.build: recorder/CMakeFiles/recorder.dir/recorder.c.o


# Object files for target recorder
recorder_OBJECTS = \
"CMakeFiles/recorder.dir/linkedlist.c.o" \
"CMakeFiles/recorder.dir/recorder.c.o"

# External object files for target recorder
recorder_EXTERNAL_OBJECTS =

recorder/librecorder.a: recorder/CMakeFiles/recorder.dir/linkedlist.c.o
recorder/librecorder.a: recorder/CMakeFiles/recorder.dir/recorder.c.o
recorder/librecorder.a: recorder/CMakeFiles/recorder.dir/build.make
recorder/librecorder.a: recorder/CMakeFiles/recorder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zby/my_tools_and_scripts/log/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library librecorder.a"
	cd /home/zby/my_tools_and_scripts/log/build/recorder && $(CMAKE_COMMAND) -P CMakeFiles/recorder.dir/cmake_clean_target.cmake
	cd /home/zby/my_tools_and_scripts/log/build/recorder && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/recorder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
recorder/CMakeFiles/recorder.dir/build: recorder/librecorder.a

.PHONY : recorder/CMakeFiles/recorder.dir/build

recorder/CMakeFiles/recorder.dir/requires: recorder/CMakeFiles/recorder.dir/linkedlist.c.o.requires
recorder/CMakeFiles/recorder.dir/requires: recorder/CMakeFiles/recorder.dir/recorder.c.o.requires

.PHONY : recorder/CMakeFiles/recorder.dir/requires

recorder/CMakeFiles/recorder.dir/clean:
	cd /home/zby/my_tools_and_scripts/log/build/recorder && $(CMAKE_COMMAND) -P CMakeFiles/recorder.dir/cmake_clean.cmake
.PHONY : recorder/CMakeFiles/recorder.dir/clean

recorder/CMakeFiles/recorder.dir/depend:
	cd /home/zby/my_tools_and_scripts/log/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zby/my_tools_and_scripts/log /home/zby/my_tools_and_scripts/log/recorder /home/zby/my_tools_and_scripts/log/build /home/zby/my_tools_and_scripts/log/build/recorder /home/zby/my_tools_and_scripts/log/build/recorder/CMakeFiles/recorder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : recorder/CMakeFiles/recorder.dir/depend

