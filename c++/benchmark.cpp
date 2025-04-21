#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <map>
#include <nlohmann/json.hpp>
#include <flatbuffers/flatbuffers.h>

// 包含生成的头文件
#include "generated/vector_data.pb.h"
#include "generated/vector_data_generated.h"

// 使用命名空间
using json = nlohmann::json;
using namespace std::chrono;

// 使用正确的命名空间
// Protocol Buffers 命名空间
namespace pb = proto::vector_data;
// FlatBuffers 命名空间
namespace fb = flatbuf::vector_data;

// 定义测试数据结构
struct TestData {
    std::vector<std::vector<double>> vectors;
};

// 用于统计结果的结构
struct BenchmarkResult {
    size_t originalSize = 0;
    size_t serializedSize = 0;
    double serializeTime = 0.0;
    double deserializeTime = 0.0;
    double totalTime = 0.0;
    double cpuUsage = 0.0;
};

// 获取CPU使用率（简化版，实际应根据系统API实现）
double getCpuUsage() {
    // 在实际应用中，这里应该使用系统API获取CPU使用率
    // 这里简化处理，返回0
    return 0.0;
}

// 生成测试数据
std::vector<std::vector<double>> generateTestData(int numVectors = 500, int vectorDim = 768) {
    std::vector<std::vector<double>> data(numVectors, std::vector<double>(vectorDim));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (auto& vector : data) {
        for (auto& val : vector) {
            val = dis(gen);
        }
    }
    return data;
}

// 计算数据大小（字节）
size_t getDataSize(const std::vector<std::vector<double>>& data) {
    size_t size = 0;
    for (const auto& vector : data) {
        size += vector.size() * sizeof(double);
    }
    return size;
}

// JSON序列化测试
BenchmarkResult jsonSerializationTest(const std::vector<std::vector<double>>& data) {
    BenchmarkResult result;
    result.originalSize = getDataSize(data);
    double cpuStart = getCpuUsage();
    auto totalStartTime = high_resolution_clock::now();

    // 序列化
    auto startTime = high_resolution_clock::now();
    json j;
    j["vectors"] = data;
    std::string jsonData = j.dump();
    auto endTime = high_resolution_clock::now();
    result.serializeTime = duration_cast<microseconds>(endTime - startTime).count() / 1000.0;

    // 反序列化
    startTime = high_resolution_clock::now();
    auto parsedJson = json::parse(jsonData);
    std::vector<std::vector<double>> parsedData = parsedJson["vectors"].get<std::vector<std::vector<double>>>();
    endTime = high_resolution_clock::now();
    result.deserializeTime = duration_cast<microseconds>(endTime - startTime).count() / 1000.0;

    auto totalEndTime = high_resolution_clock::now();
    result.totalTime = duration_cast<microseconds>(totalEndTime - totalStartTime).count() / 1000.0;
    result.serializedSize = jsonData.size();
    double cpuEnd = getCpuUsage();
    result.cpuUsage = cpuEnd - cpuStart;

    return result;
}

// Protocol Buffers序列化测试
BenchmarkResult protobufSerializationTest(const std::vector<std::vector<double>>& data) {
    BenchmarkResult result;
    result.originalSize = getDataSize(data);
    double cpuStart = getCpuUsage();
    auto totalStartTime = high_resolution_clock::now();

    // 构建protobuf消息
    pb::VectorDataset dataset;
    for (const auto& vector : data) {
        pb::VectorData* vectorData = dataset.add_vectors();
        for (const auto& value : vector) {
            vectorData->add_values(value);
        }
    }

    // 序列化
    auto startTime = high_resolution_clock::now();
    std::string serializedData;
    dataset.SerializeToString(&serializedData);
    auto endTime = high_resolution_clock::now();
    result.serializeTime = duration_cast<microseconds>(endTime - startTime).count() / 1000.0;

    // 反序列化
    startTime = high_resolution_clock::now();
    pb::VectorDataset parsedDataset;
    parsedDataset.ParseFromString(serializedData);
    endTime = high_resolution_clock::now();
    result.deserializeTime = duration_cast<microseconds>(endTime - startTime).count() / 1000.0;

    auto totalEndTime = high_resolution_clock::now();
    result.totalTime = duration_cast<microseconds>(totalEndTime - totalStartTime).count() / 1000.0;
    result.serializedSize = serializedData.size();
    double cpuEnd = getCpuUsage();
    result.cpuUsage = cpuEnd - cpuStart;

    return result;
}

// FlatBuffers序列化测试
BenchmarkResult flatbuffersSerializationTest(const std::vector<std::vector<double>>& data) {
    BenchmarkResult result;
    result.originalSize = getDataSize(data);
    double cpuStart = getCpuUsage();
    auto totalStartTime = high_resolution_clock::now();

    // 预计算buffer大小
    size_t estimatedSize = getDataSize(data) + 1024; // 额外空间用于开销
    flatbuffers::FlatBufferBuilder builder(estimatedSize);

    // 构建FlatBuffers的向量数据结构
    std::vector<flatbuffers::Offset<fb::VectorData>> vectorOffsets;
    for (const auto& vector : data) {
        auto values = builder.CreateVector(vector);
        auto vectorData = fb::CreateVectorData(builder, values);
        vectorOffsets.push_back(vectorData);
    }

    // 创建数据集
    auto serializeStartTime = high_resolution_clock::now();
    
    auto vectors = builder.CreateVector(vectorOffsets);
    auto dataset = fb::CreateVectorDataset(builder, vectors);
    builder.Finish(dataset);

    // 获取序列化后的数据
    uint8_t* serializedData = builder.GetBufferPointer();
    int size = builder.GetSize();
    auto serializeEndTime = high_resolution_clock::now();
    result.serializeTime = duration_cast<microseconds>(serializeEndTime - serializeStartTime).count() / 1000.0;

    // 反序列化
    auto startTime = high_resolution_clock::now();
    auto fbDataset = fb::GetVectorDataset(serializedData);
    
    // 访问数据才能真正触发反序列化
    auto vectorsCount = fbDataset->vectors()->size();
    for (size_t i = 0; i < vectorsCount; i++) {
        auto vector = fbDataset->vectors()->Get(i);
        auto valuesCount = vector->values()->size();
        for (size_t j = 0; j < valuesCount; j++) {
            double value = vector->values()->Get(j);
            (void)value; // 避免未使用变量警告
        }
    }
    auto endTime = high_resolution_clock::now();
    result.deserializeTime = duration_cast<microseconds>(endTime - startTime).count() / 1000.0;

    auto totalEndTime = high_resolution_clock::now();
    result.totalTime = duration_cast<microseconds>(totalEndTime - totalStartTime).count() / 1000.0;
    result.serializedSize = size;
    double cpuEnd = getCpuUsage();
    result.cpuUsage = cpuEnd - cpuStart;

    return result;
}

// 运行基准测试
void runBenchmark(int numIterations = 5) {
    std::cout << "生成测试数据..." << std::endl;
    auto data = generateTestData();

    std::map<std::string, BenchmarkResult> results = {
        {"JSON", BenchmarkResult()},
        {"Protocol Buffers", BenchmarkResult()},
        {"FlatBuffers", BenchmarkResult()}
    };

    std::cout << "\n运行" << numIterations << "次迭代的基准测试..." << std::endl;
    for (int i = 0; i < numIterations; i++) {
        std::cout << "迭代 " << (i + 1) << "/" << numIterations << std::endl;

        // 运行所有测试
        std::map<std::string, BenchmarkResult> testResults = {
            {"JSON", jsonSerializationTest(data)},
            {"Protocol Buffers", protobufSerializationTest(data)},
            {"FlatBuffers", flatbuffersSerializationTest(data)}
        };

        // 累加结果
        for (const auto& [format, metrics] : testResults) {
            results[format].originalSize = metrics.originalSize;
            results[format].serializedSize = metrics.serializedSize;
            results[format].serializeTime += metrics.serializeTime / numIterations;
            results[format].deserializeTime += metrics.deserializeTime / numIterations;
            results[format].totalTime += metrics.totalTime / numIterations;
            results[format].cpuUsage += metrics.cpuUsage / numIterations;
        }
    }

    // 打印结果
    std::cout << "\n基准测试结果:" << std::endl;
    std::string divider(120, '-');
    std::cout << divider << std::endl;
    
    std::cout << std::left << std::setw(27) << "序列化格式" 
              << std::right << std::setw(20) << "原始大小(MB)" 
              << std::setw(20) << "序列化(MB)" 
              << std::setw(20) << "序列化(ms)" 
              << std::setw(20) << "反序列化(ms)" 
              << std::setw(20) << "总时间(ms)" 
              << std::setw(10) << "CPU(%)" << std::endl;
    
    std::cout << divider << std::endl;

    for (const auto& [format, metrics] : results) {
        double originalMB = static_cast<double>(metrics.originalSize) / (1024 * 1024);
        double serializedMB = static_cast<double>(metrics.serializedSize) / (1024 * 1024);

        std::cout << std::left << std::setw(17) << format 
                  << std::right << std::setw(17) << std::fixed << std::setprecision(3) << originalMB 
                  << std::setw(17) << serializedMB 
                  << std::setw(17) << metrics.serializeTime 
                  << std::setw(17) << metrics.deserializeTime 
                  << std::setw(17) << metrics.totalTime 
                  << std::setw(12) << std::setprecision(2) << metrics.cpuUsage << std::endl;
    }

    std::cout << divider << std::endl;
}

int main() {
    runBenchmark();
    return 0;
}