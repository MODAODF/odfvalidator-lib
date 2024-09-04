# 動態
add_library(ODFValidator SHARED lib/ODFValidator.cpp)

# 包含頭文件目錄
target_include_directories(ODFValidator PUBLIC include)