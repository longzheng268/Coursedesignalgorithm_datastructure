# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_SOURCE_DIR = /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build

# Utility rule file for sync_resources.

# Include any custom commands dependencies for this target.
include CMakeFiles/sync_resources.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sync_resources.dir/progress.make

CMakeFiles/sync_resources:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Syncing resource files..."
	/usr/bin/cmake -E copy_directory /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/resources /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build/resources

sync_resources: CMakeFiles/sync_resources
sync_resources: CMakeFiles/sync_resources.dir/build.make
.PHONY : sync_resources

# Rule to build all files generated by this target.
CMakeFiles/sync_resources.dir/build: sync_resources
.PHONY : CMakeFiles/sync_resources.dir/build

CMakeFiles/sync_resources.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sync_resources.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sync_resources.dir/clean

CMakeFiles/sync_resources.dir/depend:
	cd /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build /media/LongZheng/Data_2/Code_Project/Coursedesignalgorithm_datastructure/build/CMakeFiles/sync_resources.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/sync_resources.dir/depend

