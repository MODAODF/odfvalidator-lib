cmake_minimum_required(VERSION 3.10)

# 設置項目名稱和版本
project(ODFValidator VERSION 0.0.9)

# 指定 C++ 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含頭文件目錄
include_directories(src/include)

# 添加子目錄
add_subdirectory(src)

# 添加可執行文件
add_executable(odfvalidator tools/odfvalidator.cpp)

# 連接庫
target_link_libraries(odfvalidator ODFValidator)
