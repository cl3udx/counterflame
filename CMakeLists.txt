cmake_minimum_required(VERSION 3.15)
project(CounterFlameDylib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)

add_library(CounterFlameDylib SHARED ${SOURCES})

target_include_directories(CounterFlameDylib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

target_link_libraries(CounterFlameDylib PRIVATE 
    "-framework Foundation"
    "-framework UIKit"
    "-framework OpenGLES"
)

set_target_properties(CounterFlameDylib PROPERTIES
    OUTPUT_NAME "CounterFlameDylib"
    PREFIX "lib"
    SUFFIX ".dylib"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
    XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
)
