

add_library(ODFValidator SHARED lib/ODFValidator.cpp)
# 包含頭文件目錄
target_include_directories(ODFValidator PUBLIC include)

# 編譯靜態庫
add_library(ODFValidator_static STATIC lib/ODFValidator.cpp)
target_include_directories(ODFValidator_static PUBLIC include)
