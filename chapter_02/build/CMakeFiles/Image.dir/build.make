# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build

# Include any dependencies generated for this target.
include CMakeFiles/Image.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Image.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Image.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Image.dir/flags.make

CMakeFiles/Image.dir/main.cpp.o: CMakeFiles/Image.dir/flags.make
CMakeFiles/Image.dir/main.cpp.o: /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/main.cpp
CMakeFiles/Image.dir/main.cpp.o: CMakeFiles/Image.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Image.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Image.dir/main.cpp.o -MF CMakeFiles/Image.dir/main.cpp.o.d -o CMakeFiles/Image.dir/main.cpp.o -c /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/main.cpp

CMakeFiles/Image.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Image.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/main.cpp > CMakeFiles/Image.dir/main.cpp.i

CMakeFiles/Image.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Image.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/main.cpp -o CMakeFiles/Image.dir/main.cpp.s

CMakeFiles/Image.dir/src/color.cpp.o: CMakeFiles/Image.dir/flags.make
CMakeFiles/Image.dir/src/color.cpp.o: /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/color.cpp
CMakeFiles/Image.dir/src/color.cpp.o: CMakeFiles/Image.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Image.dir/src/color.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Image.dir/src/color.cpp.o -MF CMakeFiles/Image.dir/src/color.cpp.o.d -o CMakeFiles/Image.dir/src/color.cpp.o -c /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/color.cpp

CMakeFiles/Image.dir/src/color.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Image.dir/src/color.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/color.cpp > CMakeFiles/Image.dir/src/color.cpp.i

CMakeFiles/Image.dir/src/color.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Image.dir/src/color.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/color.cpp -o CMakeFiles/Image.dir/src/color.cpp.s

CMakeFiles/Image.dir/src/vec3.cpp.o: CMakeFiles/Image.dir/flags.make
CMakeFiles/Image.dir/src/vec3.cpp.o: /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/vec3.cpp
CMakeFiles/Image.dir/src/vec3.cpp.o: CMakeFiles/Image.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Image.dir/src/vec3.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Image.dir/src/vec3.cpp.o -MF CMakeFiles/Image.dir/src/vec3.cpp.o.d -o CMakeFiles/Image.dir/src/vec3.cpp.o -c /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/vec3.cpp

CMakeFiles/Image.dir/src/vec3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Image.dir/src/vec3.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/vec3.cpp > CMakeFiles/Image.dir/src/vec3.cpp.i

CMakeFiles/Image.dir/src/vec3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Image.dir/src/vec3.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/src/vec3.cpp -o CMakeFiles/Image.dir/src/vec3.cpp.s

# Object files for target Image
Image_OBJECTS = \
"CMakeFiles/Image.dir/main.cpp.o" \
"CMakeFiles/Image.dir/src/color.cpp.o" \
"CMakeFiles/Image.dir/src/vec3.cpp.o"

# External object files for target Image
Image_EXTERNAL_OBJECTS =

Image: CMakeFiles/Image.dir/main.cpp.o
Image: CMakeFiles/Image.dir/src/color.cpp.o
Image: CMakeFiles/Image.dir/src/vec3.cpp.o
Image: CMakeFiles/Image.dir/build.make
Image: CMakeFiles/Image.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Image"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Image.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Image.dir/build: Image
.PHONY : CMakeFiles/Image.dir/build

CMakeFiles/Image.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Image.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Image.dir/clean

CMakeFiles/Image.dir/depend:
	cd /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02 /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02 /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build /Users/uncled/Documents/CFiles/Ray_Tracing/in_one_weekend/RayTracing-InOneWeekend/chapter_02/build/CMakeFiles/Image.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Image.dir/depend
