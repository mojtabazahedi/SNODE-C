# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/student/projects/snode.c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/student/projects/snode.c/build

# Include any dependencies generated for this target.
include apps/CMakeFiles/websocketparser.dir/depend.make

# Include the progress variables for this target.
include apps/CMakeFiles/websocketparser.dir/progress.make

# Include the compile flags for this target's objects.
include apps/CMakeFiles/websocketparser.dir/flags.make

apps/CMakeFiles/websocketparser.dir/websocketparser.cpp.o: apps/CMakeFiles/websocketparser.dir/flags.make
apps/CMakeFiles/websocketparser.dir/websocketparser.cpp.o: ../apps/websocketparser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/projects/snode.c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/CMakeFiles/websocketparser.dir/websocketparser.cpp.o"
	cd /home/student/projects/snode.c/build/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/websocketparser.dir/websocketparser.cpp.o -c /home/student/projects/snode.c/apps/websocketparser.cpp

apps/CMakeFiles/websocketparser.dir/websocketparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/websocketparser.dir/websocketparser.cpp.i"
	cd /home/student/projects/snode.c/build/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/student/projects/snode.c/apps/websocketparser.cpp > CMakeFiles/websocketparser.dir/websocketparser.cpp.i

apps/CMakeFiles/websocketparser.dir/websocketparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/websocketparser.dir/websocketparser.cpp.s"
	cd /home/student/projects/snode.c/build/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/student/projects/snode.c/apps/websocketparser.cpp -o CMakeFiles/websocketparser.dir/websocketparser.cpp.s

# Object files for target websocketparser
websocketparser_OBJECTS = \
"CMakeFiles/websocketparser.dir/websocketparser.cpp.o"

# External object files for target websocketparser
websocketparser_EXTERNAL_OBJECTS =

apps/websocketparser: apps/CMakeFiles/websocketparser.dir/websocketparser.cpp.o
apps/websocketparser: apps/CMakeFiles/websocketparser.dir/build.make
apps/websocketparser: express/libexpress.so.1
apps/websocketparser: web/http/libweb.so.1
apps/websocketparser: net/libnet.so.1
apps/websocketparser: utils/libutils.so.1
apps/websocketparser: log/liblogger.so.1
apps/websocketparser: /usr/lib/x86_64-linux-gnu/libssl.so
apps/websocketparser: /usr/lib/x86_64-linux-gnu/libcrypto.so
apps/websocketparser: /usr/lib/x86_64-linux-gnu/libmagic.so
apps/websocketparser: apps/CMakeFiles/websocketparser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/projects/snode.c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable websocketparser"
	cd /home/student/projects/snode.c/build/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/websocketparser.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/CMakeFiles/websocketparser.dir/build: apps/websocketparser

.PHONY : apps/CMakeFiles/websocketparser.dir/build

apps/CMakeFiles/websocketparser.dir/clean:
	cd /home/student/projects/snode.c/build/apps && $(CMAKE_COMMAND) -P CMakeFiles/websocketparser.dir/cmake_clean.cmake
.PHONY : apps/CMakeFiles/websocketparser.dir/clean

apps/CMakeFiles/websocketparser.dir/depend:
	cd /home/student/projects/snode.c/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/projects/snode.c /home/student/projects/snode.c/apps /home/student/projects/snode.c/build /home/student/projects/snode.c/build/apps /home/student/projects/snode.c/build/apps/CMakeFiles/websocketparser.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/CMakeFiles/websocketparser.dir/depend

