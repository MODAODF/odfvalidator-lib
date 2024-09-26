
# 編譯共享函式庫
add_library(ODFValidator SHARED lib/ODFValidator.cpp)
set_target_properties(ODFValidator PROPERTIES OUTPUT_NAME "ODFValidator")
target_include_directories(ODFValidator PUBLIC include)

# 非 Windows 系統需要連結 stdc++fs
if(WIN32)
    target_compile_definitions(ODFValidator PRIVATE ODFVALIDATOR_EXPORTS _CRT_SECURE_NO_WARNINGS)
else()
    target_link_libraries(ODFValidator PRIVATE stdc++fs)
endif()

# 編譯靜態函式庫
add_library(ODFValidator_static STATIC lib/ODFValidator.cpp)
set_target_properties(ODFValidator_static PROPERTIES OUTPUT_NAME "ODFValidator_static")
target_include_directories(ODFValidator_static PUBLIC include)
# 非 Windows 系統需要連結 stdc++fs
if(WIN32)
    target_compile_definitions(ODFValidator_static PRIVATE ODFVALIDATOR_STATIC_DEFINE _CRT_SECURE_NO_WARNINGS)
else()
    target_link_libraries(ODFValidator_static PRIVATE stdc++fs)
endif()
