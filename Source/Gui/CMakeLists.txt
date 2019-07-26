set(${TEVIAN_GUI_LIB}_SOURCE_FILES
    ${TEVIAN_SOURCE_DIR}/Gui/ViewPort.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/RenderWidget.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/Controls.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/DetectionRenderer.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/ImageViewTab.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/Scaling.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/ImageBook.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/Window.cpp
    ${TEVIAN_SOURCE_DIR}/Gui/PreferenceDialog.cpp
    )

add_library(${TEVIAN_GUI_LIB} STATIC ${${TEVIAN_GUI_LIB}_SOURCE_FILES})
target_include_directories(
        ${TEVIAN_GUI_LIB}
        PUBLIC ${TEVIAN_SOURCE_DIR})

target_link_libraries(
        ${TEVIAN_GUI_LIB}
        PUBLIC ${TEVIAN_BASE_LIB}
)

set_target_properties(
        ${TEVIAN_GUI_LIB}
        PROPERTIES
        AUTOMOC ON
        # Make source directory to visible for moc compiler
        AUTOMOC_OPTIONS "-I ${TEVIAN_SOURCE_DIR}"
        AUTOGEN_BUILD_DIR ${CMAKE_BINARY_DIR}/Autogen
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

if(WIN32)
    add_custom_command(
            TARGET ${TEVIAN_GUI_LIB} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${TEVIAN_GUI_LIB}>
            $<TARGET_DIR:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>
    )
endif()
