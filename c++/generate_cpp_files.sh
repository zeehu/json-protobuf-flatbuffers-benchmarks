#!/bin/bash

# 设置目录路径
CPP_DIR="$(pwd)"
DATA_DIR="../data"
PROTO_DIR="${DATA_DIR}/proto"
FLATBUFFERS_DIR="${DATA_DIR}/flatbuffers"
GENERATED_DIR="${CPP_DIR}/generated"

# 创建生成文件目录（如果不存在）
mkdir -p "${GENERATED_DIR}"

echo "正在生成 C++ 文件..."

# 生成 Protocol Buffers C++ 文件
echo "生成 Protocol Buffers C++ 文件..."
protoc --cpp_out="${GENERATED_DIR}" \
       --proto_path="${PROTO_DIR}" \
       "${PROTO_DIR}/vector_data.proto"

# 生成 FlatBuffers C++ 文件
echo "生成 FlatBuffers C++ 文件..."
flatc --cpp -o "${GENERATED_DIR}" \
      "${FLATBUFFERS_DIR}/vector_data.fbs"

echo "所有 C++ 文件生成完成。"
echo "生成的文件:"
echo "Protocol Buffers: ${GENERATED_DIR}/vector_data.pb.h 和 ${GENERATED_DIR}/vector_data.pb.cc"
echo "FlatBuffers: ${GENERATED_DIR}/vector_data_generated.h"
echo ""
echo "请确保在CMakeLists.txt中包含这些文件。" 