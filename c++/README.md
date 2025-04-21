# C++ 序列化基准测试

这个目录包含用C++实现的序列化基准测试，对比了JSON、Protocol Buffers和FlatBuffers的性能。

## 依赖项

- CMake (>= 3.10)
- Protocol Buffers
- FlatBuffers
- nlohmann/json (C++的JSON库)

## 编译

### 安装依赖项

对于Ubuntu/Debian系统：

```bash
# 安装Protocol Buffers
sudo apt-get install libprotobuf-dev protobuf-compiler

# 安装FlatBuffers
sudo apt-get install flatbuffers-compiler libflatbuffers-dev

# 安装nlohmann/json (通过CMake的FetchContent或手动安装)
sudo apt-get install nlohmann-json3-dev
```

对于其他系统，请参考各个库的官方文档进行安装。

### 构建项目

```bash
# 创建构建目录
mkdir build && cd build

# 配置CMake
cmake ..

# 编译
make
```

如果库不在标准路径中，可能需要手动指定路径：

```bash
cmake -DFLATBUFFERS_INCLUDE_DIR=/path/to/flatbuffers/include -DFLATBUFFERS_LIBRARIES=/path/to/flatbuffers/lib ..
```

## 运行基准测试

编译成功后，在build目录运行：

```bash
./benchmark
```

这将执行5次迭代的序列化基准测试，并输出结果。

## 实现说明

该实现测试了三种序列化格式：

1. **JSON**: 使用nlohmann/json库
2. **Protocol Buffers**: 使用Google的protobuf
3. **FlatBuffers**: 使用Google的FlatBuffers

每个测试都会对相同的数据集进行序列化和反序列化操作，并测量：
- 原始数据大小
- 序列化后的大小
- 序列化时间
- 反序列化时间
- 总处理时间
- CPU使用率 (注意：当前实现返回0，实际应用中应使用系统API)

## 注意事项

- 对于FlatBuffers的测试，为了正确测量反序列化时间，代码会访问所有数据，因为FlatBuffers使用延迟解析机制。
- CPU使用率测量在当前实现中返回0，实际应用中应替换为适当的系统API。 