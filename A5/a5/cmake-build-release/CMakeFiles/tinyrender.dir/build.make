# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/tinyrender.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tinyrender.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tinyrender.dir/flags.make

CMakeFiles/tinyrender.dir/src/core/integrator.cpp.o: CMakeFiles/tinyrender.dir/flags.make
CMakeFiles/tinyrender.dir/src/core/integrator.cpp.o: ../src/core/integrator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tinyrender.dir/src/core/integrator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinyrender.dir/src/core/integrator.cpp.o -c /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/integrator.cpp

CMakeFiles/tinyrender.dir/src/core/integrator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyrender.dir/src/core/integrator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/integrator.cpp > CMakeFiles/tinyrender.dir/src/core/integrator.cpp.i

CMakeFiles/tinyrender.dir/src/core/integrator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyrender.dir/src/core/integrator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/integrator.cpp -o CMakeFiles/tinyrender.dir/src/core/integrator.cpp.s

CMakeFiles/tinyrender.dir/src/core/renderer.cpp.o: CMakeFiles/tinyrender.dir/flags.make
CMakeFiles/tinyrender.dir/src/core/renderer.cpp.o: ../src/core/renderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tinyrender.dir/src/core/renderer.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinyrender.dir/src/core/renderer.cpp.o -c /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/renderer.cpp

CMakeFiles/tinyrender.dir/src/core/renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyrender.dir/src/core/renderer.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/renderer.cpp > CMakeFiles/tinyrender.dir/src/core/renderer.cpp.i

CMakeFiles/tinyrender.dir/src/core/renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyrender.dir/src/core/renderer.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/renderer.cpp -o CMakeFiles/tinyrender.dir/src/core/renderer.cpp.s

CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.o: CMakeFiles/tinyrender.dir/flags.make
CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.o: ../src/core/renderpass.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.o -c /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/renderpass.cpp

CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/renderpass.cpp > CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.i

CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/core/renderpass.cpp -o CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.s

CMakeFiles/tinyrender.dir/src/main.cpp.o: CMakeFiles/tinyrender.dir/flags.make
CMakeFiles/tinyrender.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/tinyrender.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tinyrender.dir/src/main.cpp.o -c /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/main.cpp

CMakeFiles/tinyrender.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyrender.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/main.cpp > CMakeFiles/tinyrender.dir/src/main.cpp.i

CMakeFiles/tinyrender.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyrender.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/src/main.cpp -o CMakeFiles/tinyrender.dir/src/main.cpp.s

# Object files for target tinyrender
tinyrender_OBJECTS = \
"CMakeFiles/tinyrender.dir/src/core/integrator.cpp.o" \
"CMakeFiles/tinyrender.dir/src/core/renderer.cpp.o" \
"CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.o" \
"CMakeFiles/tinyrender.dir/src/main.cpp.o"

# External object files for target tinyrender
tinyrender_EXTERNAL_OBJECTS =

tinyrender: CMakeFiles/tinyrender.dir/src/core/integrator.cpp.o
tinyrender: CMakeFiles/tinyrender.dir/src/core/renderer.cpp.o
tinyrender: CMakeFiles/tinyrender.dir/src/core/renderpass.cpp.o
tinyrender: CMakeFiles/tinyrender.dir/src/main.cpp.o
tinyrender: CMakeFiles/tinyrender.dir/build.make
tinyrender: /usr/local/lib/libGLEW.dylib
tinyrender: CMakeFiles/tinyrender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable tinyrender"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinyrender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tinyrender.dir/build: tinyrender

.PHONY : CMakeFiles/tinyrender.dir/build

CMakeFiles/tinyrender.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tinyrender.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tinyrender.dir/clean

CMakeFiles/tinyrender.dir/depend:
	cd /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5 /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5 /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release /Users/j.li/School/U3_FALL/ECSE_446/Assignments/A5/a5/cmake-build-release/CMakeFiles/tinyrender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tinyrender.dir/depend

