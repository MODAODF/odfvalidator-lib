cmake_minimum_required(VERSION 3.10)

# 指定專案名稱與版本
project(ODFValidator VERSION 0.0.9)

# 指定 C++ 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含標頭檔案目錄
include_directories(src/include)

# 新增原始碼子目錄
add_subdirectory(src)

# 從來源檔產生執行檔
add_executable(odfvalidator tools/odfvalidator.cpp)

# 指定要連結的函式庫
target_link_libraries(odfvalidator ODFValidator)
