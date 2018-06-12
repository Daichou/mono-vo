# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/opencv-3.1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/opencv-3.1.0/build

# Utility rule file for pch_Generate_opencv_objdetect.

# Include the progress variables for this target.
include modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/progress.make

modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect: modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch

modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch: ../modules/objdetect/src/precomp.hpp
modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch: modules/objdetect/precomp.hpp
modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch: lib/libopencv_objdetect_pch_dephelp.a
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ubuntu/opencv-3.1.0/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp.gch/opencv_objdetect_Release.gch"
	cd /home/ubuntu/opencv-3.1.0/build/modules/objdetect && /usr/bin/cmake -E make_directory /home/ubuntu/opencv-3.1.0/build/modules/objdetect/precomp.hpp.gch
	cd /home/ubuntu/opencv-3.1.0/build/modules/objdetect && /opt/Xilinx/SDK/2013.4/gnu/arm/lin/bin/arm-xilinx-linux-gnueabi-g++ -O3 -DNDEBUG -DNDEBUG -fPIC "-D__OPENCV_BUILD=1" "-DCVAPI_EXPORTS" -isystem"/home/ubuntu/opencv-3.1.0/build" -isystem"/home/ubuntu/opencv-3.1.0/build" -I"/home/ubuntu/opencv-3.1.0/modules/objdetect/include" -I"/home/ubuntu/opencv-3.1.0/modules/objdetect/src" -isystem"/home/ubuntu/opencv-3.1.0/build/modules/objdetect" -I"/home/ubuntu/opencv-3.1.0/modules/core/include" -I"/home/ubuntu/opencv-3.1.0/modules/imgproc/include" -I"/home/ubuntu/opencv-3.1.0/modules/ml/include" -I"/home/ubuntu/opencv-3.1.0/modules/imgcodecs/include" -I"/home/ubuntu/opencv-3.1.0/modules/videoio/include" -I"/home/ubuntu/opencv-3.1.0/modules/highgui/include" -fsigned-char -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wno-narrowing -Wno-delete-non-virtual-dtor -fdiagnostics-show-option -pthread -fomit-frame-pointer -mfpu=neon -ffunction-sections -fvisibility=hidden -fvisibility-inlines-hidden -DCVAPI_EXPORTS -x c++-header -o /home/ubuntu/opencv-3.1.0/build/modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch /home/ubuntu/opencv-3.1.0/build/modules/objdetect/precomp.hpp

modules/objdetect/precomp.hpp: ../modules/objdetect/src/precomp.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ubuntu/opencv-3.1.0/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp"
	cd /home/ubuntu/opencv-3.1.0/build/modules/objdetect && /usr/bin/cmake -E copy_if_different /home/ubuntu/opencv-3.1.0/modules/objdetect/src/precomp.hpp /home/ubuntu/opencv-3.1.0/build/modules/objdetect/precomp.hpp

pch_Generate_opencv_objdetect: modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect
pch_Generate_opencv_objdetect: modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch
pch_Generate_opencv_objdetect: modules/objdetect/precomp.hpp
pch_Generate_opencv_objdetect: modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/build.make
.PHONY : pch_Generate_opencv_objdetect

# Rule to build all files generated by this target.
modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/build: pch_Generate_opencv_objdetect
.PHONY : modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/build

modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/clean:
	cd /home/ubuntu/opencv-3.1.0/build/modules/objdetect && $(CMAKE_COMMAND) -P CMakeFiles/pch_Generate_opencv_objdetect.dir/cmake_clean.cmake
.PHONY : modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/clean

modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/depend:
	cd /home/ubuntu/opencv-3.1.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/opencv-3.1.0 /home/ubuntu/opencv-3.1.0/modules/objdetect /home/ubuntu/opencv-3.1.0/build /home/ubuntu/opencv-3.1.0/build/modules/objdetect /home/ubuntu/opencv-3.1.0/build/modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/depend

