# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.28.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.28.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/trung/cpp_vrml_utility

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/trung/cpp_vrml_utility

# Include any dependencies generated for this target.
include modules/cli11/tests/CMakeFiles/SimpleTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include modules/cli11/tests/CMakeFiles/SimpleTest.dir/compiler_depend.make

# Include the progress variables for this target.
include modules/cli11/tests/CMakeFiles/SimpleTest.dir/progress.make

# Include the compile flags for this target's objects.
include modules/cli11/tests/CMakeFiles/SimpleTest.dir/flags.make

modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o: modules/cli11/tests/CMakeFiles/SimpleTest.dir/flags.make
modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o: modules/cli11/tests/SimpleTest.cpp
modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o: modules/cli11/tests/CMakeFiles/SimpleTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/trung/cpp_vrml_utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o -MF CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o.d -o CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o -c /Users/trung/cpp_vrml_utility/modules/cli11/tests/SimpleTest.cpp

modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SimpleTest.dir/SimpleTest.cpp.i"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/trung/cpp_vrml_utility/modules/cli11/tests/SimpleTest.cpp > CMakeFiles/SimpleTest.dir/SimpleTest.cpp.i

modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SimpleTest.dir/SimpleTest.cpp.s"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/trung/cpp_vrml_utility/modules/cli11/tests/SimpleTest.cpp -o CMakeFiles/SimpleTest.dir/SimpleTest.cpp.s

# Object files for target SimpleTest
SimpleTest_OBJECTS = \
"CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o"

# External object files for target SimpleTest
SimpleTest_EXTERNAL_OBJECTS =

dist/SimpleTest: modules/cli11/tests/CMakeFiles/SimpleTest.dir/SimpleTest.cpp.o
dist/SimpleTest: modules/cli11/tests/CMakeFiles/SimpleTest.dir/build.make
dist/SimpleTest: modules/cli11/tests/libcatch_main.a
dist/SimpleTest: modules/cli11/tests/CMakeFiles/SimpleTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/trung/cpp_vrml_utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../dist/SimpleTest"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimpleTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/cli11/tests/CMakeFiles/SimpleTest.dir/build: dist/SimpleTest
.PHONY : modules/cli11/tests/CMakeFiles/SimpleTest.dir/build

modules/cli11/tests/CMakeFiles/SimpleTest.dir/clean:
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && $(CMAKE_COMMAND) -P CMakeFiles/SimpleTest.dir/cmake_clean.cmake
.PHONY : modules/cli11/tests/CMakeFiles/SimpleTest.dir/clean

modules/cli11/tests/CMakeFiles/SimpleTest.dir/depend:
	cd /Users/trung/cpp_vrml_utility && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/trung/cpp_vrml_utility /Users/trung/cpp_vrml_utility/modules/cli11/tests /Users/trung/cpp_vrml_utility /Users/trung/cpp_vrml_utility/modules/cli11/tests /Users/trung/cpp_vrml_utility/modules/cli11/tests/CMakeFiles/SimpleTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : modules/cli11/tests/CMakeFiles/SimpleTest.dir/depend

