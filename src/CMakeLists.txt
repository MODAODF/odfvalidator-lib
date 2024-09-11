
# 編譯共享函式庫
add_library(ODFValidator SHARED lib/ODFValidator.cpp)
set_target_properties(ODFValidator PROPERTIES OUTPUT_NAME "ODFValidator")
target_include_directories(ODFValidator PUBLIC include)
target_link_libraries(ODFValidator PRIVATE stdc++fs)

# 編譯靜態庫
add_library(ODFValidator_static STATIC lib/ODFValidator.cpp)
set_target_properties(ODFValidator_static PROPERTIES OUTPUT_NAME "ODFValidator")
target_include_directories(ODFValidator_static PUBLIC include)
target_link_libraries(ODFValidator_static PRIVATE stdc++fs)
