set(CMAKE_CXX_COMPILER "/opt/Xilinx/SDK/2013.4/gnu/arm/lin/bin/arm-xilinx-linux-gnueabi-g++")
set(CMAKE_CXX_COMPILER_ARG1 "")
set(CMAKE_CXX_COMPILER_ID "GNU")
set(CMAKE_CXX_COMPILER_VERSION "4.7.3")
set(CMAKE_CXX_PLATFORM_ID "Linux")

set(CMAKE_AR "/opt/Xilinx/SDK/2013.4/gnu/arm/lin/bin/arm-xilinx-linux-gnueabi-ar")
set(CMAKE_RANLIB "/opt/Xilinx/SDK/2013.4/gnu/arm/lin/bin/arm-xilinx-linux-gnueabi-ranlib")
set(CMAKE_LINKER "/opt/Xilinx/SDK/2013.4/gnu/arm/lin/bin/arm-xilinx-linux-gnueabi-ld")
set(CMAKE_COMPILER_IS_GNUCXX 1)
set(CMAKE_CXX_COMPILER_LOADED 1)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_CXX_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_CXX_COMPILER_ENV_VAR "CXX")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_CXX_COMPILER_ID_RUN 1)
set(CMAKE_CXX_IGNORE_EXTENSIONS inl;h;hpp;HPP;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS C;M;c++;cc;cpp;cxx;m;mm;CPP)
set(CMAKE_CXX_LINKER_PREFERENCE 30)
set(CMAKE_CXX_LINKER_PREFERENCE_PROPAGATES 1)

# Save compiler ABI information.
set(CMAKE_CXX_SIZEOF_DATA_PTR "4")
set(CMAKE_CXX_COMPILER_ABI "ELF")
set(CMAKE_CXX_LIBRARY_ARCHITECTURE "")

if(CMAKE_CXX_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_CXX_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_CXX_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_CXX_COMPILER_ABI}")
endif()

if(CMAKE_CXX_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()




set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "stdc++;m;c")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "/opt/Xilinx/SDK/2013.4/gnu/arm/lin/lib/gcc/arm-xilinx-linux-gnueabi/4.7.3;/opt/Xilinx/SDK/2013.4/gnu/arm/lin/lib/gcc;/opt/Xilinx/SDK/2013.4/gnu/arm/lin/arm-xilinx-linux-gnueabi/lib;/opt/Xilinx/SDK/2013.4/gnu/arm/lin/arm-xilinx-linux-gnueabi/libc/lib;/opt/Xilinx/SDK/2013.4/gnu/arm/lin/arm-xilinx-linux-gnueabi/libc/usr/lib")
set(CMAKE_CXX_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")



