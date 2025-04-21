# JSON vs Protocol Buffers vs FlatBuffers 性能对比

这个项目比较了三种不同序列化格式（JSON、Protocol Buffers和FlatBuffers）在处理高维向量数据时的性能表现。

## 测试数据
- 500个向量
- 每个向量包含768个float64数值
- 对比指标：数据大小、序列化时间、反序列化时间

## 环境设置

1. 安装依赖：
```bash
pip install -r requirements.txt
```

2. 编译Protocol Buffers和FlatBuffers定义文件：
```bash
protoc --python_out=. vector_data.proto
flatc --python vector_data.fbs
```

## 运行基准测试

执行以下命令运行基准测试：
```bash
python benchmark.py
```

## 测试内容
- 数据大小（字节）
- 序列化耗时（秒）
- 反序列化耗时（秒）

每个测试会运行5次迭代并计算平均值，以获得更准确的结果。 