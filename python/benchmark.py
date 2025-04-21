import json
import time
import numpy as np
from tqdm import tqdm
import psutil
import vector_data_pb2 as pb
import flatbuffers
import VectorData, VectorDataset

def generate_test_data(num_vectors=500, vector_dim=768):
    """生成测试数据"""
    return np.random.random((num_vectors, vector_dim))

def get_cpu_usage():
    """获取CPU使用率"""
    return psutil.cpu_percent(interval=0.1)

def get_data_size(data):
    """计算数据大小（字节）"""
    return data.nbytes

def json_serialization_test(data):
    """JSON序列化测试"""
    original_size = get_data_size(data)
    cpu_start = get_cpu_usage()
    total_start_time = time.time()
    
    start_time = time.time()
    json_data = json.dumps({"vectors": data.tolist()})
    serialize_time = time.time() - start_time
    
    start_time = time.time()
    parsed_data = json.loads(json_data)
    deserialize_time = time.time() - start_time
    
    total_time = time.time() - total_start_time
    cpu_end = get_cpu_usage()
    serialized_size = len(json_data.encode('utf-8'))
    
    return {
        'original_size': original_size,
        'serialized_size': serialized_size,
        'serialize_time': serialize_time,
        'deserialize_time': deserialize_time,
        'total_time': total_time,
        'cpu_usage': cpu_end - cpu_start
    }

def protobuf_serialization_test(data):
    """Protocol Buffers序列化测试"""
    original_size = get_data_size(data)
    cpu_start = get_cpu_usage()
    total_start_time = time.time()
    
    dataset = pb.VectorDataset()
    for vector in data:
        vector_data = dataset.vectors.add()
        vector_data.values.extend(vector)
    
    start_time = time.time()
    serialized_data = dataset.SerializeToString()
    serialize_time = time.time() - start_time
    
    start_time = time.time()
    parsed_dataset = pb.VectorDataset()
    parsed_dataset.ParseFromString(serialized_data)
    deserialize_time = time.time() - start_time
    
    total_time = time.time() - total_start_time
    cpu_end = get_cpu_usage()
    
    return {
        'original_size': original_size,
        'serialized_size': len(serialized_data),
        'serialize_time': serialize_time,
        'deserialize_time': deserialize_time,
        'total_time': total_time,
        'cpu_usage': cpu_end - cpu_start
    }

def flatbuffers_serialization_test(data):
    """FlatBuffers序列化测试"""
    original_size = get_data_size(data)
    cpu_start = get_cpu_usage()
    total_start_time = time.time()
    
    # 预计算buffer大小，避免重新分配
    estimated_size = data.size * 8 + 1024  # 每个float64占8字节，额外加一些开销
    builder = flatbuffers.Builder(estimated_size)
    
    # 批量处理向量
    vector_offsets = []
    for vector in data:
        # 创建单个向量的值数组
        builder.StartVector(8, len(vector), 8)  # 8是float64的大小
        for value in reversed(vector):  # FlatBuffers要求反序
            builder.PrependFloat64(value)
        values = builder.EndVector()
        
        # 创建VectorData对象
        VectorData.Start(builder)
        VectorData.AddValues(builder, values)
        vector_offsets.append(VectorData.End(builder))
    
    # 创建向量数组
    VectorDataset.StartVectorsVector(builder, len(vector_offsets))
    for offset in reversed(vector_offsets):
        builder.PrependUOffsetTRelative(offset)
    vectors = builder.EndVector()
    
    # 完成数据集构建
    VectorDataset.Start(builder)
    VectorDataset.AddVectors(builder, vectors)
    dataset = VectorDataset.End(builder)
    builder.Finish(dataset)
    
    # 计时序列化过程
    start_time = time.time()
    serialized_data = builder.Output()
    serialize_time = time.time() - start_time
    
    # 计时反序列化过程
    start_time = time.time()
    buf = VectorDataset.VectorDataset.GetRootAs(bytearray(serialized_data), 0)
    deserialize_time = time.time() - start_time
    
    total_time = time.time() - total_start_time
    cpu_end = get_cpu_usage()
    
    return {
        'original_size': original_size,
        'serialized_size': len(serialized_data),
        'serialize_time': serialize_time,
        'deserialize_time': deserialize_time,
        'total_time': total_time,
        'cpu_usage': cpu_end - cpu_start
    }

def run_benchmark(num_iterations=5):
    """运行基准测试"""
    print("生成测试数据...")
    data = generate_test_data()
    
    results = {
        "JSON": {"original_size": 0, "serialized_size": 0, "serialize_time": 0, 
                "deserialize_time": 0, "total_time": 0, "cpu_usage": 0},
        "Protocol Buffers": {"original_size": 0, "serialized_size": 0, "serialize_time": 0, 
                           "deserialize_time": 0, "total_time": 0, "cpu_usage": 0},
        "FlatBuffers": {"original_size": 0, "serialized_size": 0, "serialize_time": 0, 
                       "deserialize_time": 0, "total_time": 0, "cpu_usage": 0}
    }
    
    print(f"\n运行{num_iterations}次迭代的基准测试...")
    for _ in tqdm(range(num_iterations)):
        # 运行所有测试
        test_results = {
            "JSON": json_serialization_test(data),
            "Protocol Buffers": protobuf_serialization_test(data),
            "FlatBuffers": flatbuffers_serialization_test(data)
        }
        
        # 累加结果
        for format_name, metrics in test_results.items():
            results[format_name]["original_size"] = metrics["original_size"]
            results[format_name]["serialized_size"] = metrics["serialized_size"]
            for key in ["serialize_time", "deserialize_time", "total_time", "cpu_usage"]:
                results[format_name][key] += metrics[key] / num_iterations
    
    # 定义列宽和格式
    formats = {
        "format": "{:<17}",
        "orig_size": "{:>15}",
        "ser_size": "{:>15}",
        "ser_time": "{:>15}",
        "deser_time": "{:>15}",
        "total_time": "{:>15}",
        "cpu": "{:>15}"
    }
    
    # 打印表头
    print("\n基准测试结果:")
    print("-" * 140)
    
    # 构建表头
    header_row = (
        formats["format"].format("序列化格式") +
        formats["orig_size"].format("原始大小(MB)") +
        formats["ser_size"].format("序列化大小(MB)") +
        formats["ser_time"].format("序列化(ms)") +
        formats["deser_time"].format("反序列化(ms)") +
        formats["total_time"].format("总耗时(ms)") +
        formats["cpu"].format("CPU使用率(%)")
    )
    print(header_row)
    print("-" * 140)
    # 定义列宽和格式
    formats = {
        "format": "{:<17}",
        "orig_size": "{:>20}",
        "ser_size": "{:>20}",
        "ser_time": "{:>20}",
        "deser_time": "{:>17}",
        "total_time": "{:>20}",
        "cpu": "{:>17}"
    }
    # 打印数据行
    for format_name, metrics in results.items():
        original_mb = metrics['original_size'] / (1024 * 1024)
        serialized_mb = metrics['serialized_size'] / (1024 * 1024)
        
        row = (
            formats["format"].format(format_name) +
            formats["orig_size"].format(f"{original_mb:.3f}") +
            formats["ser_size"].format(f"{serialized_mb:.3f}") +
            formats["ser_time"].format(f"{metrics['serialize_time']*1000:.3f}") +
            formats["deser_time"].format(f"{metrics['deserialize_time']*1000:.3f}") +
            formats["total_time"].format(f"{metrics['total_time']*1000:.3f}") +
            formats["cpu"].format(f"{metrics['cpu_usage']:.2f}")
        )
        print(row)
    
    print("-" * 140)

if __name__ == "__main__":
    run_benchmark()