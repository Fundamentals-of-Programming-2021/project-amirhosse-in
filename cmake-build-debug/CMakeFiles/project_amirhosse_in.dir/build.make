# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/amirhossein/Downloads/clion-2019.3.6/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/amirhossein/Downloads/clion-2019.3.6/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/amirhossein/Documents/project-amirhosse-in

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/project_amirhosse_in.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/project_amirhosse_in.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/project_amirhosse_in.dir/flags.make

CMakeFiles/project_amirhosse_in.dir/main.c.o: CMakeFiles/project_amirhosse_in.dir/flags.make
CMakeFiles/project_amirhosse_in.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/project_amirhosse_in.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/project_amirhosse_in.dir/main.c.o   -c /home/amirhossein/Documents/project-amirhosse-in/main.c

CMakeFiles/project_amirhosse_in.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/project_amirhosse_in.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/amirhossein/Documents/project-amirhosse-in/main.c > CMakeFiles/project_amirhosse_in.dir/main.c.i

CMakeFiles/project_amirhosse_in.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/project_amirhosse_in.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/amirhossein/Documents/project-amirhosse-in/main.c -o CMakeFiles/project_amirhosse_in.dir/main.c.s

# Object files for target project_amirhosse_in
project_amirhosse_in_OBJECTS = \
"CMakeFiles/project_amirhosse_in.dir/main.c.o"

# External object files for target project_amirhosse_in
project_amirhosse_in_EXTERNAL_OBJECTS =

project_amirhosse_in: CMakeFiles/project_amirhosse_in.dir/main.c.o
project_amirhosse_in: CMakeFiles/project_amirhosse_in.dir/build.make
project_amirhosse_in: CMakeFiles/project_amirhosse_in.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable project_amirhosse_in"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/project_amirhosse_in.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/project_amirhosse_in.dir/build: project_amirhosse_in

.PHONY : CMakeFiles/project_amirhosse_in.dir/build

CMakeFiles/project_amirhosse_in.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/project_amirhosse_in.dir/cmake_clean.cmake
.PHONY : CMakeFiles/project_amirhosse_in.dir/clean

CMakeFiles/project_amirhosse_in.dir/depend:
	cd /home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amirhossein/Documents/project-amirhosse-in /home/amirhossein/Documents/project-amirhosse-in /home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug /home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug /home/amirhossein/Documents/project-amirhosse-in/cmake-build-debug/CMakeFiles/project_amirhosse_in.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/project_amirhosse_in.dir/depend

