# 序列化格式基准测试

本项目对比了三种主流序列化格式的性能：
- JSON
- Protocol Buffers
- FlatBuffers

针对不同编程语言提供了相同功能的实现，使用统一的数据结构，方便进行跨语言比较。

## 项目结构

```
json-protobuf-flatbuffers-benchmarks/
├── data/                       # 数据定义文件
│   ├── flatbuffers/            # FlatBuffers 定义文件
│   │   └── vector_data.fbs     # FlatBuffers 格式的向量数据定义
│   └── proto/                  # Protocol Buffers 定义文件
│       └── vector_data.proto   # Protocol Buffers 格式的向量数据定义
├── c++/                        # C++ 实现
├── go/                         # Go 实现
├── python/                     # Python 实现
└── results/                    # 性能测试结果（可选）
```

## 测试数据

所有语言使用相同的测试数据格式：向量数据集（VectorDataset），包含多个向量（VectorData），每个向量包含一组浮点数值。

数据定义在以下文件中：
- Protocol Buffers: `data/proto/vector_data.proto`
- FlatBuffers: `data/flatbuffers/vector_data.fbs`

## 依赖项

### 全局依赖

- [Protocol Buffers](https://github.com/protocolbuffers/protobuf)
- [FlatBuffers](https://github.com/google/flatbuffers)

## C++ 实现

### 依赖项

- CMake (>= 3.10)
- C++17 兼容的编译器
- Protocol Buffers 库
- FlatBuffers 库
- nlohmann/json 库

### 文件生成

在 C++ 目录下运行脚本生成所需的 Protocol Buffers 和 FlatBuffers 代码：

```bash
cd c++
chmod +x generate_cpp_files.sh
./generate_cpp_files.sh
```

这将在 `c++/generated/` 目录下生成以下文件：
- Protocol Buffers: `vector_data.pb.h` 和 `vector_data.pb.cc`
- FlatBuffers: `vector_data_generated.h`

### 编译

使用 CMake 编译项目：

```bash
cd c++
mkdir -p build && cd build
cmake ..
make
```

### 运行

编译完成后，在 build 目录中运行：

```bash
./benchmark
```

## Go 实现

### 依赖项

- Go (>= 1.13)
- Protocol Buffers Go 插件
- FlatBuffers Go 库

### 安装依赖

```bash
go get -u github.com/google/flatbuffers/go
go get -u google.golang.org/protobuf/proto
go get -u github.com/shirou/gopsutil/v3
```

### 文件生成

在根目录运行脚本生成 Go 代码：

```bash
cd go
chmod +x ../generate_go_files.sh
../generate_go_files.sh
```

这将生成以下文件：
- Protocol Buffers: `benchmark/proto_vector_data/*.go`
- FlatBuffers: `benchmark/flatbuf_vector_data/*.go`

### 运行

```bash
cd go
go run benchmark.go
```

## Python 实现

### 依赖项

- Python 3.6+
- numpy
- psutil
- tqdm
- flatbuffers
- protobuf

### 安装依赖

```bash
cd python
pip install -r requirements.txt
```

### 文件生成

在 Python 目录下运行脚本生成所需的文件：

```bash
cd python
chmod +x generate_python_files.sh
./generate_python_files.sh
```

这将生成以下文件：
- Protocol Buffers: `vector_data_pb2.py`
- FlatBuffers: `vector_data_generated.py`

### 运行

```bash
cd python
python benchmark.py
```

## 基准测试结果

每种语言的基准测试都会输出类似下表的结果：

```
基准测试结果:
------------------------------------------------------------
序列化格式               原始大小(MB)      序列化大小(MB)        序列化(ms)     反序列化(ms)        总耗时(ms)     CPU使用率(%)
------------------------------------------------------------
JSON                        2.930             7.058           133.735          677.000          810.736          0.00
Protocol Buffers            2.930             2.933             0.661            0.533            7.990          0.00
FlatBuffers                 2.930             2.939             0.023           12.167           12.862          0.00
------------------------------------------------------------
```

测试结果解释：
- 原始大小：未序列化的数据大小（MB）
- 序列化大小：序列化后的数据大小（MB）
- 序列化(ms)：序列化过程耗时（毫秒）
- 反序列化(ms)：反序列化过程耗时（毫秒）
- 总耗时(ms)：包括序列化、反序列化以及其他处理的总时间（毫秒）
- CPU使用率(%)：CPU 使用百分比

## 对比分析

通常情况下，各序列化格式的特点如下：

1. **JSON**:
   - 优点：人类可读，广泛支持，无需额外代码生成
   - 缺点：序列化大小最大，序列化和反序列化性能最差

2. **Protocol Buffers**:
   - 优点：序列化大小小，反序列化性能好，总体性能优秀
   - 缺点：需要代码生成，不是人类可读格式

3. **FlatBuffers**:
   - 优点：序列化性能极佳，序列化大小小
   - 缺点：反序列化可能较慢，需要代码生成，不是人类可读格式

## 贡献

欢迎贡献代码或提出问题！如果您想添加新的语言实现或改进现有代码，请提交 Pull Request。

## 许可证

MIT 许可证 