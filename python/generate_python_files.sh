#!/bin/bash

# 设置目录路径
PYTHON_DIR="$(pwd)"
DATA_DIR="../data"
PROTO_DIR="${DATA_DIR}/proto"
FLATBUFFERS_DIR="${DATA_DIR}/flatbuffers"

echo "正在生成 Python 文件..."

# 生成 Protocol Buffers Python 文件
echo "生成 Protocol Buffers Python 文件..."
protoc --python_out="${PYTHON_DIR}" \
       --proto_path="${PROTO_DIR}" \
       "${PROTO_DIR}/vector_data.proto"

# 生成 FlatBuffers Python 文件
echo "生成 FlatBuffers Python 文件..."
# 使用 --python-flatbuffers-dir 选项将文件直接生成在当前目录下
flatc --python -o "${PYTHON_DIR}" "${FLATBUFFERS_DIR}/vector_data.fbs"

echo "所有 Python 文件生成完成。"
echo "生成的文件:"
echo "Protocol Buffers: vector_data_pb2.py"
echo "FlatBuffers: flatbuf/vector_data/VectorData*.py"