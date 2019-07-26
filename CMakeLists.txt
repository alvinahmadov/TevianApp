cmake_minimum_required(VERSION 3.10)
project(TevianTask)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_definitions(${Qt5Widgets_DEFINITIONS})

set(POSITION_INDEPENDENT_CODE ON)
# Globally enable automoc
# Automoc needs to find an include of <moc_(file).cpp> in <file.cpp>
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(UNIX)
    # Build/(Windows|Linux)/(Release|Debig)
    set(TEVIAN_BUILD_DIR ${PROJECT_SOURCE_DIR}/Build/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})
endif(UNIX)

# All header and source files getPath
set(TEVIAN_SOURCE_DIR ${PROJECT_SOURCE_DIR}/Source)
set(TEVIAN_TESTS_DIR ${PROJECT_SOURCE_DIR}/Tests)

if(WIN32)
    set(CXX_FLAGS "-EHsc")
    set(CXX_FLAGS_DEBUG "-Wall -Od")
    # Warning level 2, Optimization level 2
    set(CXX_FLAGS_RELEASE "-O2 -W2")
    # On windows target exe file needs to be build as Windows' native executable
    set(NATIVE WIN32)
elseif(UNIX)
    set(CXX_FLAGS "-Wno-return-type -Wno-ignored-qualifiers -Wno-delete-non-virtual-dtor")
    set(CXX_FLAGS_DEBUG "-Wall -Wextra -g -O0 ")
    set(CXX_FLAGS_RELEASE "-O3 -ffp-contract=off -fno-expensive-optimizations -march=native")
    # On linux target exe file doesn't need to be build as Windows' native executable
    set(NATIVE)
endif(WIN32)


# Enable testing for project modules separately. If all values false it builds main.
# Test Gui module
set(TEVIAN_GUI       FALSE)

# Base library. Contains link to Qt5 libraries needed for modules
# Linked against every module's static library
set(TEVIAN_BASE_LIB base)
# Gui module library. Contains application's gui components
set(TEVIAN_GUI_LIB gui)

# Set CXX flags
set(CMAKE_CXX_FLAGS ${CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${CXX_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${CXX_FLAGS_RELEASE})

# Needed to include here to preoperly run AUTOMOC precompiling
# Reason: All needed Qt libraries are linked against project's
# base library and not visible for module libraries, make it global
find_package(Qt5Core REQUIRED)


add_subdirectory(${TEVIAN_SOURCE_DIR})
add_subdirectory(${TEVIAN_SOURCE_DIR}/Gui)
add_subdirectory(${TEVIAN_TESTS_DIR})