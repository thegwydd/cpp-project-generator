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

#Set executable
add_executable(${PROJECT_NAME}     
    src/main.cpp
    src/${PARSED_PROJECT_NAME}_version.h
    )

#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

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
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)		
endif()

#Find libraries
find_package(Threads)

#Finally
target_link_libraries(${PROJECT_NAME} 
    Threads::Threads 
    ${CONAN_LIBS}
)
