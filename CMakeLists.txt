cmake_minimum_required(VERSION 3.10)

project(ODFValidator VERSION 0.0.9 LANGUAGES C CXX)

# Set default installation prefix
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "C:/Progra~1" CACHE PATH "default install path" FORCE)
else()
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "default install path" FORCE)
endif()

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# Set the output directory for executables
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

add_definitions(-DCMAKE=1)

# Check for Windows platform
if(WIN32)
    message(STATUS "Windows platform detected")
    add_definitions(-DWINDOWS=1)
else()
    message(STATUS "Non-Windows platform detected")
    add_definitions(-DWINDOWS=0)
endif()

# Generate new GUIDs using PowerShell on Windows
if(WIN32)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE PRODUCT_GUID OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE COMPONENT_GUID OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE COMPONENT_GUID_CHECKER OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE COMPONENT_GUID_DLL OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE COMPONENT_GUID_STATIC_LIB OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE COMPONENT_GUID_JAR OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND powershell -Command "[guid]::NewGuid().ToString()" OUTPUT_VARIABLE COMPONENT_GUID_HEADER OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# Find the JAR file
file(GLOB JAR_FILES "resources/odfvalidator*.jar")
list(GET JAR_FILES 0 JAR_FILE)
get_filename_component(ODFVALIDATOR_JAR_NAME ${JAR_FILE} NAME)

# Option for enabling debug build
option(ENABLE_DEBUG "Enable debug build" ON)
if(ENABLE_DEBUG)
    add_definitions(-DENABLE_DEBUG=1)
else()
    add_definitions(-DENABLE_DEBUG=0)
endif()

# Add the path to the JAR file as a definition
if(WIN32)
    set(ODFVALIDATOR_JAR_DIR_PATH "${CMAKE_INSTALL_PREFIX}/ODFValidator/resources")
else()
    set(ODFVALIDATOR_JAR_DIR_PATH "${CMAKE_INSTALL_PREFIX}/share/java/odfvalidator")
endif()

# Add the path to the JAR file as a definition
add_definitions(-DODFVALIDATOR_JAR_PATH="${ODFVALIDATOR_JAR_DIR_PATH}/${ODFVALIDATOR_JAR_NAME}")

# Configure WiX file
configure_file(${CMAKE_SOURCE_DIR}/ODFValidator.wxs.in ${CMAKE_BINARY_DIR}/ODFValidator.wxs @ONLY)

# Subdirectories for building
add_subdirectory(src)
add_subdirectory(resources)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_executable(odfvalidator tools/odfvalidator.cpp)
target_link_libraries(odfvalidator ODFValidator_static)
add_executable(odfchecker tools/odfchecker.cpp)
target_link_libraries(odfchecker ODFValidator_static)

# Install target
install(FILES ${JAR_FILE} DESTINATION ${ODFVALIDATOR_JAR_DIR_PATH})

# Add custom commands to generate MSI package
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/ODFValidator.wixobj
    COMMAND candle.exe ${CMAKE_BINARY_DIR}/ODFValidator.wxs -o ${CMAKE_BINARY_DIR}/ODFValidator.wixobj
    DEPENDS ${CMAKE_BINARY_DIR}/ODFValidator.wxs
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/ODFValidator.msi
    COMMAND light.exe -out ${CMAKE_BINARY_DIR}/ODFValidator.msi ${CMAKE_BINARY_DIR}/ODFValidator.wixobj
    DEPENDS ${CMAKE_BINARY_DIR}/ODFValidator.wixobj
)

add_custom_target(
    package
    ALL
    DEPENDS ${CMAKE_BINARY_DIR}/ODFValidator.msi
)

# Print configuration summary
message(STATUS "ODFValidator version ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
message(STATUS "Enable debug: ${ENABLE_DEBUG}")
message(STATUS "Target jar file: ${ODFVALIDATOR_JAR_DIR_PATH}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
