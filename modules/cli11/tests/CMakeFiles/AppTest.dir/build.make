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
include modules/cli11/tests/CMakeFiles/AppTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include modules/cli11/tests/CMakeFiles/AppTest.dir/compiler_depend.make

# Include the progress variables for this target.
include modules/cli11/tests/CMakeFiles/AppTest.dir/progress.make

# Include the compile flags for this target's objects.
include modules/cli11/tests/CMakeFiles/AppTest.dir/flags.make

modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.o: modules/cli11/tests/CMakeFiles/AppTest.dir/flags.make
modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.o: modules/cli11/tests/AppTest.cpp
modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.o: modules/cli11/tests/CMakeFiles/AppTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/trung/cpp_vrml_utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.o"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.o -MF CMakeFiles/AppTest.dir/AppTest.cpp.o.d -o CMakeFiles/AppTest.dir/AppTest.cpp.o -c /Users/trung/cpp_vrml_utility/modules/cli11/tests/AppTest.cpp

modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AppTest.dir/AppTest.cpp.i"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/trung/cpp_vrml_utility/modules/cli11/tests/AppTest.cpp > CMakeFiles/AppTest.dir/AppTest.cpp.i

modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AppTest.dir/AppTest.cpp.s"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/trung/cpp_vrml_utility/modules/cli11/tests/AppTest.cpp -o CMakeFiles/AppTest.dir/AppTest.cpp.s

# Object files for target AppTest
AppTest_OBJECTS = \
"CMakeFiles/AppTest.dir/AppTest.cpp.o"

# External object files for target AppTest
AppTest_EXTERNAL_OBJECTS =

dist/AppTest: modules/cli11/tests/CMakeFiles/AppTest.dir/AppTest.cpp.o
dist/AppTest: modules/cli11/tests/CMakeFiles/AppTest.dir/build.make
dist/AppTest: modules/cli11/tests/libcatch_main.a
dist/AppTest: modules/cli11/tests/CMakeFiles/AppTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/trung/cpp_vrml_utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../dist/AppTest"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/cli11/tests/CMakeFiles/AppTest.dir/build: dist/AppTest
.PHONY : modules/cli11/tests/CMakeFiles/AppTest.dir/build

modules/cli11/tests/CMakeFiles/AppTest.dir/clean:
	cd /Users/trung/cpp_vrml_utility/modules/cli11/tests && $(CMAKE_COMMAND) -P CMakeFiles/AppTest.dir/cmake_clean.cmake
.PHONY : modules/cli11/tests/CMakeFiles/AppTest.dir/clean

modules/cli11/tests/CMakeFiles/AppTest.dir/depend:
	cd /Users/trung/cpp_vrml_utility && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/trung/cpp_vrml_utility /Users/trung/cpp_vrml_utility/modules/cli11/tests /Users/trung/cpp_vrml_utility /Users/trung/cpp_vrml_utility/modules/cli11/tests /Users/trung/cpp_vrml_utility/modules/cli11/tests/CMakeFiles/AppTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : modules/cli11/tests/CMakeFiles/AppTest.dir/depend

