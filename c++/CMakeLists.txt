cmake_minimum_required(VERSION 3.10)
project(serialization_benchmarks)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找必要的库
find_package(Protobuf REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(nlohmann_json REQUIRED)

# 设置生成文件的路径
set(GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated)

# 包含目录
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${FLATBUFFERS_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GENERATED_DIR})

# 添加可执行文件
add_executable(benchmark 
    benchmark.cpp
    ${GENERATED_DIR}/vector_data.pb.cc
)

# 链接必要的库
target_link_libraries(benchmark
    ${PROTOBUF_LIBRARIES}
    flatbuffers
    nlohmann_json::nlohmann_json
)