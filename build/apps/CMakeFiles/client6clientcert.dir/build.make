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
include apps/CMakeFiles/client6clientcert.dir/depend.make

# Include the progress variables for this target.
include apps/CMakeFiles/client6clientcert.dir/progress.make

# Include the compile flags for this target's objects.
include apps/CMakeFiles/client6clientcert.dir/flags.make

apps/CMakeFiles/client6clientcert.dir/client6clientcert.cpp.o: apps/CMakeFiles/client6clientcert.dir/flags.make
apps/CMakeFiles/client6clientcert.dir/client6clientcert.cpp.o: ../apps/client6clientcert.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/projects/snode.c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/CMakeFiles/client6clientcert.dir/client6clientcert.cpp.o"
	cd /home/student/projects/snode.c/build/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client6clientcert.dir/client6clientcert.cpp.o -c /home/student/projects/snode.c/apps/client6clientcert.cpp

apps/CMakeFiles/client6clientcert.dir/client6clientcert.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client6clientcert.dir/client6clientcert.cpp.i"
	cd /home/student/projects/snode.c/build/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/student/projects/snode.c/apps/client6clientcert.cpp > CMakeFiles/client6clientcert.dir/client6clientcert.cpp.i

apps/CMakeFiles/client6clientcert.dir/client6clientcert.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client6clientcert.dir/client6clientcert.cpp.s"
	cd /home/student/projects/snode.c/build/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/student/projects/snode.c/apps/client6clientcert.cpp -o CMakeFiles/client6clientcert.dir/client6clientcert.cpp.s

# Object files for target client6clientcert
client6clientcert_OBJECTS = \
"CMakeFiles/client6clientcert.dir/client6clientcert.cpp.o"

# External object files for target client6clientcert
client6clientcert_EXTERNAL_OBJECTS =

apps/client6clientcert: apps/CMakeFiles/client6clientcert.dir/client6clientcert.cpp.o
apps/client6clientcert: apps/CMakeFiles/client6clientcert.dir/build.make
apps/client6clientcert: web/http/libweb.so.1
apps/client6clientcert: net/libnet.so.1
apps/client6clientcert: utils/libutils.so.1
apps/client6clientcert: log/liblogger.so.1
apps/client6clientcert: /usr/lib/x86_64-linux-gnu/libssl.so
apps/client6clientcert: /usr/lib/x86_64-linux-gnu/libcrypto.so
apps/client6clientcert: /usr/lib/x86_64-linux-gnu/libmagic.so
apps/client6clientcert: apps/CMakeFiles/client6clientcert.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/projects/snode.c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client6clientcert"
	cd /home/student/projects/snode.c/build/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client6clientcert.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/CMakeFiles/client6clientcert.dir/build: apps/client6clientcert

.PHONY : apps/CMakeFiles/client6clientcert.dir/build

apps/CMakeFiles/client6clientcert.dir/clean:
	cd /home/student/projects/snode.c/build/apps && $(CMAKE_COMMAND) -P CMakeFiles/client6clientcert.dir/cmake_clean.cmake
.PHONY : apps/CMakeFiles/client6clientcert.dir/clean

apps/CMakeFiles/client6clientcert.dir/depend:
	cd /home/student/projects/snode.c/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/projects/snode.c /home/student/projects/snode.c/apps /home/student/projects/snode.c/build /home/student/projects/snode.c/build/apps /home/student/projects/snode.c/build/apps/CMakeFiles/client6clientcert.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/CMakeFiles/client6clientcert.dir/depend

