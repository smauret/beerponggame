# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /opt/clion-2017.3.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2017.3.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lucas/CLionProjects/beerponggame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lucas/CLionProjects/beerponggame/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/beerpongproject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/beerpongproject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/beerpongproject.dir/flags.make

CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o: CMakeFiles/beerpongproject.dir/flags.make
CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o: ../src/mainLucas.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o -c /home/lucas/CLionProjects/beerponggame/src/mainLucas.cpp

CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lucas/CLionProjects/beerponggame/src/mainLucas.cpp > CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.i

CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lucas/CLionProjects/beerponggame/src/mainLucas.cpp -o CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.s

CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.requires:

.PHONY : CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.requires

CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.provides: CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.requires
	$(MAKE) -f CMakeFiles/beerpongproject.dir/build.make CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.provides.build
.PHONY : CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.provides

CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.provides.build: CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o


CMakeFiles/beerpongproject.dir/src/Table.cpp.o: CMakeFiles/beerpongproject.dir/flags.make
CMakeFiles/beerpongproject.dir/src/Table.cpp.o: ../src/Table.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/beerpongproject.dir/src/Table.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/beerpongproject.dir/src/Table.cpp.o -c /home/lucas/CLionProjects/beerponggame/src/Table.cpp

CMakeFiles/beerpongproject.dir/src/Table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/beerpongproject.dir/src/Table.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lucas/CLionProjects/beerponggame/src/Table.cpp > CMakeFiles/beerpongproject.dir/src/Table.cpp.i

CMakeFiles/beerpongproject.dir/src/Table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/beerpongproject.dir/src/Table.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lucas/CLionProjects/beerponggame/src/Table.cpp -o CMakeFiles/beerpongproject.dir/src/Table.cpp.s

CMakeFiles/beerpongproject.dir/src/Table.cpp.o.requires:

.PHONY : CMakeFiles/beerpongproject.dir/src/Table.cpp.o.requires

CMakeFiles/beerpongproject.dir/src/Table.cpp.o.provides: CMakeFiles/beerpongproject.dir/src/Table.cpp.o.requires
	$(MAKE) -f CMakeFiles/beerpongproject.dir/build.make CMakeFiles/beerpongproject.dir/src/Table.cpp.o.provides.build
.PHONY : CMakeFiles/beerpongproject.dir/src/Table.cpp.o.provides

CMakeFiles/beerpongproject.dir/src/Table.cpp.o.provides.build: CMakeFiles/beerpongproject.dir/src/Table.cpp.o


CMakeFiles/beerpongproject.dir/src/Player.cpp.o: CMakeFiles/beerpongproject.dir/flags.make
CMakeFiles/beerpongproject.dir/src/Player.cpp.o: ../src/Player.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/beerpongproject.dir/src/Player.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/beerpongproject.dir/src/Player.cpp.o -c /home/lucas/CLionProjects/beerponggame/src/Player.cpp

CMakeFiles/beerpongproject.dir/src/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/beerpongproject.dir/src/Player.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lucas/CLionProjects/beerponggame/src/Player.cpp > CMakeFiles/beerpongproject.dir/src/Player.cpp.i

CMakeFiles/beerpongproject.dir/src/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/beerpongproject.dir/src/Player.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lucas/CLionProjects/beerponggame/src/Player.cpp -o CMakeFiles/beerpongproject.dir/src/Player.cpp.s

CMakeFiles/beerpongproject.dir/src/Player.cpp.o.requires:

.PHONY : CMakeFiles/beerpongproject.dir/src/Player.cpp.o.requires

CMakeFiles/beerpongproject.dir/src/Player.cpp.o.provides: CMakeFiles/beerpongproject.dir/src/Player.cpp.o.requires
	$(MAKE) -f CMakeFiles/beerpongproject.dir/build.make CMakeFiles/beerpongproject.dir/src/Player.cpp.o.provides.build
.PHONY : CMakeFiles/beerpongproject.dir/src/Player.cpp.o.provides

CMakeFiles/beerpongproject.dir/src/Player.cpp.o.provides.build: CMakeFiles/beerpongproject.dir/src/Player.cpp.o


CMakeFiles/beerpongproject.dir/src/Ball.cpp.o: CMakeFiles/beerpongproject.dir/flags.make
CMakeFiles/beerpongproject.dir/src/Ball.cpp.o: ../src/Ball.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/beerpongproject.dir/src/Ball.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/beerpongproject.dir/src/Ball.cpp.o -c /home/lucas/CLionProjects/beerponggame/src/Ball.cpp

CMakeFiles/beerpongproject.dir/src/Ball.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/beerpongproject.dir/src/Ball.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lucas/CLionProjects/beerponggame/src/Ball.cpp > CMakeFiles/beerpongproject.dir/src/Ball.cpp.i

CMakeFiles/beerpongproject.dir/src/Ball.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/beerpongproject.dir/src/Ball.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lucas/CLionProjects/beerponggame/src/Ball.cpp -o CMakeFiles/beerpongproject.dir/src/Ball.cpp.s

CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.requires:

.PHONY : CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.requires

CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.provides: CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.requires
	$(MAKE) -f CMakeFiles/beerpongproject.dir/build.make CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.provides.build
.PHONY : CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.provides

CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.provides.build: CMakeFiles/beerpongproject.dir/src/Ball.cpp.o


CMakeFiles/beerpongproject.dir/src/Cup.cpp.o: CMakeFiles/beerpongproject.dir/flags.make
CMakeFiles/beerpongproject.dir/src/Cup.cpp.o: ../src/Cup.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/beerpongproject.dir/src/Cup.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/beerpongproject.dir/src/Cup.cpp.o -c /home/lucas/CLionProjects/beerponggame/src/Cup.cpp

CMakeFiles/beerpongproject.dir/src/Cup.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/beerpongproject.dir/src/Cup.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lucas/CLionProjects/beerponggame/src/Cup.cpp > CMakeFiles/beerpongproject.dir/src/Cup.cpp.i

CMakeFiles/beerpongproject.dir/src/Cup.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/beerpongproject.dir/src/Cup.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lucas/CLionProjects/beerponggame/src/Cup.cpp -o CMakeFiles/beerpongproject.dir/src/Cup.cpp.s

CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.requires:

.PHONY : CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.requires

CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.provides: CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.requires
	$(MAKE) -f CMakeFiles/beerpongproject.dir/build.make CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.provides.build
.PHONY : CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.provides

CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.provides.build: CMakeFiles/beerpongproject.dir/src/Cup.cpp.o


# Object files for target beerpongproject
beerpongproject_OBJECTS = \
"CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o" \
"CMakeFiles/beerpongproject.dir/src/Table.cpp.o" \
"CMakeFiles/beerpongproject.dir/src/Player.cpp.o" \
"CMakeFiles/beerpongproject.dir/src/Ball.cpp.o" \
"CMakeFiles/beerpongproject.dir/src/Cup.cpp.o"

# External object files for target beerpongproject
beerpongproject_EXTERNAL_OBJECTS =

beerpongproject: CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o
beerpongproject: CMakeFiles/beerpongproject.dir/src/Table.cpp.o
beerpongproject: CMakeFiles/beerpongproject.dir/src/Player.cpp.o
beerpongproject: CMakeFiles/beerpongproject.dir/src/Ball.cpp.o
beerpongproject: CMakeFiles/beerpongproject.dir/src/Cup.cpp.o
beerpongproject: CMakeFiles/beerpongproject.dir/build.make
beerpongproject: CMakeFiles/beerpongproject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable beerpongproject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/beerpongproject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/beerpongproject.dir/build: beerpongproject

.PHONY : CMakeFiles/beerpongproject.dir/build

CMakeFiles/beerpongproject.dir/requires: CMakeFiles/beerpongproject.dir/src/mainLucas.cpp.o.requires
CMakeFiles/beerpongproject.dir/requires: CMakeFiles/beerpongproject.dir/src/Table.cpp.o.requires
CMakeFiles/beerpongproject.dir/requires: CMakeFiles/beerpongproject.dir/src/Player.cpp.o.requires
CMakeFiles/beerpongproject.dir/requires: CMakeFiles/beerpongproject.dir/src/Ball.cpp.o.requires
CMakeFiles/beerpongproject.dir/requires: CMakeFiles/beerpongproject.dir/src/Cup.cpp.o.requires

.PHONY : CMakeFiles/beerpongproject.dir/requires

CMakeFiles/beerpongproject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/beerpongproject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/beerpongproject.dir/clean

CMakeFiles/beerpongproject.dir/depend:
	cd /home/lucas/CLionProjects/beerponggame/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lucas/CLionProjects/beerponggame /home/lucas/CLionProjects/beerponggame /home/lucas/CLionProjects/beerponggame/cmake-build-debug /home/lucas/CLionProjects/beerponggame/cmake-build-debug /home/lucas/CLionProjects/beerponggame/cmake-build-debug/CMakeFiles/beerpongproject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/beerpongproject.dir/depend

