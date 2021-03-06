cmake_minimum_required(VERSION 3.10)

## set appropriately the following line to create a Double Precision version
set(DOUBLE_PRECISION ON)

project(createNnzPartition C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

## adding the following lines while cleaning the code
if (CMAKE_C_COMPILER_ID STREQUAL "GNU" )
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -ftree-vectorize -fopt-info-vec-optimized -ffast-math -mavx2 ")
    #set(CMAKE_EXE_LINKER_FLAGS "-lm" )
elseif ( CMAKE_C_COMPILER_ID STREQUAL "Intel" )
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -qopt-report=3 -qopt-report-phase=vec -xCORE-AVX2")
elseif ( CMAKE_C_COMPILER_ID STREQUAL "PGI" )
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Minform=inform -Minfo=all -Mvect=simd -DPGI")
endif ( )
## end of adding the following lines while cleaning the code



if (DOUBLE_PRECISION)
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DDOUBLE")
endif()


include_directories("src/include")
set (nnzPar "src/nnzPar.c")
add_executable(nnzPar ${nnzPar})

set (rowPar "src/rowPar.c")
add_executable(rowPar ${rowPar})

