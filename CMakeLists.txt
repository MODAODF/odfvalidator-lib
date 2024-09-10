cmake_minimum_required(VERSION 3.10)

project(ODFValidator VERSION 0.0.9 LANGUAGES C CXX)

# Set default installation prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "default install path" FORCE)
endif()

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_definitions(-DCMAKE=1)

# Check for Windows platform
if(WIN32)
    message(STATUS "Windows platform detected")
    add_definitions(-DWINDOWS=1)
else()
    message(STATUS "Non-Windows platform detected")
    add_definitions(-DWINDOWS=0)
endif()


# Find the JAR file
file(GLOB JAR_FILES "resources/odfvalidator*.jar")
list(GET JAR_FILES 0 JAR_FILE)
get_filename_component(ODFVALIDATOR_JAR_NAME ${JAR_FILE} NAME)

# Option for enabling debug build
option(ENABLE_DEBUG "Enable debug build" ON)
if(ENABLE_DEBUG)
    add_definitions(-DENABLE_DEBUG=1)
    set(ODFVALIDATOR_JAR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/${ODFVALIDATOR_JAR_NAME}")
else()
    add_definitions(-DENABLE_DEBUG=0)
    set(ODFVALIDATOR_JAR_PATH "${CMAKE_INSTALL_PREFIX}/share/java/odfvalidator/${ODFVALIDATOR_JAR_NAME}")
endif()

# Add the path to the JAR file as a definition
add_definitions(-DODFVALIDATOR_JAR_PATH="${ODFVALIDATOR_JAR_PATH}")

# Subdirectories for building
add_subdirectory(src)
add_subdirectory(resources)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_executable(odfvalidator tools/odfvalidator.cpp)
target_link_libraries(odfvalidator ODFValidator_static)
add_executable(odfchecker tools/odfchecker.cpp)
target_link_libraries(odfchecker ODFValidator_static)

# Install target
install(FILES ${JAR_FILE} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/java/odfvalidator)

# Print configuration summary
message(STATUS "ODFValidator version ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
message(STATUS "Enable debug: ${ENABLE_DEBUG}")
message(STATUS "Target jar file: ${ODFVALIDATOR_JAR_PATH}")
