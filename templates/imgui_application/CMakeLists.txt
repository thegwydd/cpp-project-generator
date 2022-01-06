cmake_minimum_required(VERSION 3.10)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/project.info" project_info)
string(REGEX MATCH "project_name:[ \t]*([^ \t\r\n]*)" _ ${project_info})
set(PARSED_PROJECT_NAME ${CMAKE_MATCH_1})
string(REGEX MATCH "project_version:[ \t]*([0-9.]*)" _ ${project_info})
set(PARSED_PROJECT_VERSION ${CMAKE_MATCH_1})

string(TOUPPER ${PARSED_PROJECT_NAME} UPPERCASE_PROJECT_NAME)
string(REGEX REPLACE "-" "_" UPPERCASE_PROJECT_NAME ${UPPERCASE_PROJECT_NAME})

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

#Set up project
project(
    ${PARSED_PROJECT_NAME} 
    VERSION ${PARSED_PROJECT_VERSION}
)

conan_cmake_run(
    BASIC_SETUP ${PROJECT_NAME}
    BUILD missing
    CONANFILE conanfile.py
    )

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/${PARSED_PROJECT_NAME}_version.h
    @ONLY
    )

find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)

set(IMGUI_BINDINGS 
    ${CMAKE_CURRENT_BINARY_DIR}/bindings/src/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/bindings/src/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/bindings/include/imgui_impl_glfw.h
    ${CMAKE_CURRENT_BINARY_DIR}/bindings/include/imgui_impl_opengl3.h
    )

set(IMGUI_UTILS
    src/imgui_utils/imgui_stdlib.cpp
    src/imgui_utils/imgui_stdlib.h
    src/imgui_utils/imgui_utilities.cpp
    src/imgui_utils/imgui_utilities.h
    src/imgui_utils/imgui_toolbar.cpp
    src/imgui_utils/imgui_toolbar.h
    src/imgui_utils/imgui_control.h
    src/imgui_utils/imgui_file_dialog.cpp
    src/imgui_utils/imgui_file_dialog.h
    src/imgui_utils/imgui_dialogs.cpp
    src/imgui_utils/imgui_dialogs.h
    src/imgui_utils/imgui_message_box.cpp
    src/imgui_utils/imgui_message_box.h
    )

set(FONT_HEADERS
    fonts/include/IconsFontAwesome5ProBrands_c.h
    fonts/include/IconsForkAwesome.h
    fonts/include/IconsForkAwesome_c.h
    fonts/include/IconsIonicons.h
    fonts/include/IconsIonicons_c.h
    fonts/include/IconsKenney.h
    fonts/include/IconsKenney_c.h
    fonts/include/IconsMaterialDesign.h
    fonts/include/IconsMaterialDesign_c.h
    fonts/include/IconsMaterialDesignIcons.h
    fonts/include/IconsMaterialDesignIcons_c.h
    fonts/include/IconsFontAwesome4.h
    fonts/include/IconsFontAwesome4_c.h
    fonts/include/IconsFontAwesome5.h
    fonts/include/IconsFontAwesome5_c.h
    fonts/include/IconsFontAwesome5Brands.h
    fonts/include/IconsFontAwesome5Brands_c.h
    fonts/include/IconsFontAwesome5Pro.h
    fonts/include/IconsFontAwesome5Pro_c.h
    fonts/include/IconsFontAwesome5ProBrands.h
    )

set(DATA_FILES
    data/settings.json
    )

set(SOURCES 
    src/main.cpp
    src/${PARSED_PROJECT_NAME}_version.h
    ${IMGUI_BINDINGS}
    ${IMGUI_UTILS}
    ${FONT_HEADERS}
    ${DATA_FILES}

    src/application.cpp
    src/application.h

    src/app_settings.cpp
    src/app_settings.h

    src/imgui_style.cpp
    src/imgui_style.h
    
    src/common_defines.h
    src/common_includes.h

    src/main_frame.cpp
    src/main_frame.h
    )

#Set executable
if(MSVC)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else(MSVC)
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

source_group("application" FILES ${SOURCES})
source_group("imgui" FILES ${IMGUI_UTILS})
source_group("imgui/bindings" FILES ${IMGUI_BINDINGS})
source_group("fonts" FILES ${FONT_HEADERS})
source_group("data" FILES ${DATA_FILES})


#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
        LINK_FLAGS "/MAP"
    )

#Set flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W4)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)		
endif()

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_utils
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts/include
        ${CMAKE_CURRENT_BINARY_DIR}/bindings/include
    )

add_custom_command(TARGET ${PROJECT_NAME} 
    POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/fonts/ttf $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
        )

#Find libraries
find_package(Threads)

#Finally
target_link_libraries(${PROJECT_NAME} 
    Threads::Threads 
    ${CONAN_LIBS}
)

conan_target_link_libraries(${PROJECT_NAME})
