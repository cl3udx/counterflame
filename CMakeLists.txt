cmake_minimum_required(VERSION 3.15)
project(CounterFlameDylib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB IMGUI_SOURCES "imgui/*.cpp")

set(SOURCES
    main.cpp
    ${IMGUI_SOURCES}
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
