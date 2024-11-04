cmake_minimum_required(VERSION 3.10)

project(ODFValidator VERSION 1.0.0 LANGUAGES C CXX)

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
    set(PRODUCT_GUID "c1f95b49-a185-4d07-a874-c78877e4b8b7")
    set(COMPONENT_GUID "85661116-ebd5-41e1-80ad-15ba8efe59ca")
    set(COMPONENT_GUID_CHECKER "0528fe4e-5de2-4eb1-9bb7-b6b19998225d")
    set(COMPONENT_GUID_DLL "c25670f7-d10f-4f19-b97d-8a0544602ff0")
    set(COMPONENT_GUID_STATIC_LIB "5b57c7b1-895d-4dcd-b473-28de2043cc19")
    set(COMPONENT_GUID_JAR "645c56e0-6abe-4ba8-8270-304b8a034856")
    set(COMPONENT_GUID_HEADER "4a9fe8b9-02d1-4a78-8ca7-de64c8f4a11e")
    set(COMPONENT_GUID_PHP_EXAMPLE "94a1cc6f-7030-49d1-8152-c2d2af12d8a5")
    set(COMPONENT_GUID_CSHARP_EXAMPLE "ef7050f0-d669-47bf-bb87-5ff54799a2fb")
    set(COMPONENT_GUID_LICENSE "f79f2454-28bd-4d0f-b334-cca384917908")
    set(COMPONENT_GUID_README "956ee123-0a1d-4eb2-83ee-a85f39182077")
    set(COMPONENT_GUID_SAMPLE_CODE_USAGE "74d32083-7dd1-43a0-9ada-35ca64994fc3")
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
    OUTPUT ${CMAKE_BINARY_DIR}/ODFValidator-Library-${PROJECT_VERSION}.msi
    COMMAND light.exe -ext WixUIExtension -cultures:zh-TW -loc ${CMAKE_SOURCE_DIR}/zh-TW.wxl -out ${CMAKE_BINARY_DIR}/ODFValidator-Library-${PROJECT_VERSION}.msi ${CMAKE_BINARY_DIR}/ODFValidator.wixobj
    DEPENDS ${CMAKE_BINARY_DIR}/ODFValidator.wixobj
)

add_custom_target(
    package
    ALL
    DEPENDS ${CMAKE_BINARY_DIR}/ODFValidator-Library-${PROJECT_VERSION}.msi
)

# Print configuration summary
message(STATUS "ODFValidator version ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
message(STATUS "Enable debug: ${ENABLE_DEBUG}")
message(STATUS "Target jar file: ${ODFVALIDATOR_JAR_DIR_PATH}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
