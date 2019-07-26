list(APPEND ${TEVIAN_BASE_LIB}_SOURCE_FILES
     ${TEVIAN_SOURCE_DIR}/Commons.cpp
     ${TEVIAN_SOURCE_DIR}/FaceDetector.cpp
     ${TEVIAN_SOURCE_DIR}/Settings.cpp
     ${TEVIAN_SOURCE_DIR}/BaseApi.cpp
     ${TEVIAN_SOURCE_DIR}/FaceApi.cpp
     ${TEVIAN_SOURCE_DIR}/AbstractApi.cpp
     ${TEVIAN_SOURCE_DIR}/AuthorizationHandler.cpp
     )

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)


# Build it as a shared only
add_library(${TEVIAN_BASE_LIB} SHARED ${${TEVIAN_BASE_LIB}_SOURCE_FILES} FaceData.hpp)

# Default include dir
target_include_directories(${TEVIAN_BASE_LIB} PUBLIC ${TEVIAN_SOURCE_DIR})

# Link all needed Qt libraries to base library and us the base as a link target
# to gui, client and cli libraries
target_link_libraries(${TEVIAN_BASE_LIB}
                      PUBLIC
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets
                      Qt5::Network
                      )

# Library Properties
set_target_properties(
        ${TEVIAN_BASE_LIB}
        PROPERTIES
        # Enable moc compilation for base library.
        AUTOMOC ON
        # Build all moc generated files in Build/<CONFIG>/Autogen directory.
        AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/Autogen
        # Put all built runtime files to exe's build directory.
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

if(WIN32)
    # Windows requires all libraries to be put on exe's directory
    # Before building we copy libQt5<MODULE>.dll files to base libary's
    # build directory, use them to build target library
    add_custom_command(
            TARGET ${TEVIAN_BASE_LIB} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::Core>
            $<TARGET_FILE:Qt5::Gui>
            $<TARGET_FILE:Qt5::Widgets>
            $<TARGET_FILE:Qt5::Network>
            $<TARGET_FILE_DIR:${TEVIAN_BASE_LIB}>
    
    )
endif(WIN32)

# After building we want to copy base library to exe's output directory
add_custom_command(
        TARGET ${TEVIAN_BASE_LIB} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TEVIAN_BASE_LIB}>
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)