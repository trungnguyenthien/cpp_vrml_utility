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
include modules/cli11/examples/CMakeFiles/nested.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include modules/cli11/examples/CMakeFiles/nested.dir/compiler_depend.make

# Include the progress variables for this target.
include modules/cli11/examples/CMakeFiles/nested.dir/progress.make

# Include the compile flags for this target's objects.
include modules/cli11/examples/CMakeFiles/nested.dir/flags.make

modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.o: modules/cli11/examples/CMakeFiles/nested.dir/flags.make
modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.o: modules/cli11/examples/nested.cpp
modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.o: modules/cli11/examples/CMakeFiles/nested.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/trung/cpp_vrml_utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.o"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.o -MF CMakeFiles/nested.dir/nested.cpp.o.d -o CMakeFiles/nested.dir/nested.cpp.o -c /Users/trung/cpp_vrml_utility/modules/cli11/examples/nested.cpp

modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/nested.dir/nested.cpp.i"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/trung/cpp_vrml_utility/modules/cli11/examples/nested.cpp > CMakeFiles/nested.dir/nested.cpp.i

modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/nested.dir/nested.cpp.s"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/trung/cpp_vrml_utility/modules/cli11/examples/nested.cpp -o CMakeFiles/nested.dir/nested.cpp.s

# Object files for target nested
nested_OBJECTS = \
"CMakeFiles/nested.dir/nested.cpp.o"

# External object files for target nested
nested_EXTERNAL_OBJECTS =

dist/nested: modules/cli11/examples/CMakeFiles/nested.dir/nested.cpp.o
dist/nested: modules/cli11/examples/CMakeFiles/nested.dir/build.make
dist/nested: modules/cli11/examples/CMakeFiles/nested.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/trung/cpp_vrml_utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../dist/nested"
	cd /Users/trung/cpp_vrml_utility/modules/cli11/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nested.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/cli11/examples/CMakeFiles/nested.dir/build: dist/nested
.PHONY : modules/cli11/examples/CMakeFiles/nested.dir/build

modules/cli11/examples/CMakeFiles/nested.dir/clean:
	cd /Users/trung/cpp_vrml_utility/modules/cli11/examples && $(CMAKE_COMMAND) -P CMakeFiles/nested.dir/cmake_clean.cmake
.PHONY : modules/cli11/examples/CMakeFiles/nested.dir/clean

modules/cli11/examples/CMakeFiles/nested.dir/depend:
	cd /Users/trung/cpp_vrml_utility && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/trung/cpp_vrml_utility /Users/trung/cpp_vrml_utility/modules/cli11/examples /Users/trung/cpp_vrml_utility /Users/trung/cpp_vrml_utility/modules/cli11/examples /Users/trung/cpp_vrml_utility/modules/cli11/examples/CMakeFiles/nested.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : modules/cli11/examples/CMakeFiles/nested.dir/depend

