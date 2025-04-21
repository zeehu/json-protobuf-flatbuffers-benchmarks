#!/bin/bash

# 创建目录
mkdir -p vector_data_fbs
mkdir -p vector_data_pb

# 生成 Protocol Buffers Go 文件
protoc --go_out=./vector_data_pb --go_opt=paths=source_relative \
       --proto_path=../data/proto vector_data.proto

# 生成 FlatBuffers Go 文件
flatc --go --go-namespace flatbuf_vector_data -o ./vector_data_fbs \
      ../data/flatbuffers/vector_data.fbs

echo "Go 文件生成完成。"