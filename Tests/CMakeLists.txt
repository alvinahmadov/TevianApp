set(EXE_SOURCE_FILES    Tevian.cpp)
set(EXE_LINK_LIBRARIES  ${TEVIAN_GUI_LIB} ${TEVIAN_BASE_LIB} ${TEVIAN_CLIENT_LIB})

add_executable(TevianTask ${NATIVE} ${EXE_SOURCE_FILES} ../app.qrc)

target_include_directories(TevianTask PUBLIC            ${TEVIAN_SOURCE_DIR})

target_link_libraries(TevianTask PUBLIC                 ${EXE_LINK_LIBRARIES})

set_target_properties(
        TevianTask
        PROPERTIES
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY                        ${EXECUTABLE_OUTPUT_PATH}
)

message(STATUS "Testing ${EXE_SOURCE_FILES} libraries")
