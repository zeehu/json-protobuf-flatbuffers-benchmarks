package main

import (
	"encoding/json"
	"fmt"
	"math/rand"
	"strings"
	"time"

	flatbuffers "github.com/google/flatbuffers/go"
	"github.com/shirou/gopsutil/v3/cpu"
	"google.golang.org/protobuf/proto"
	pb "json-protobuf-flatbuffers-benchmarks/vector_data_pb"
	fb "json-protobuf-flatbuffers-benchmarks/vector_data_fbs/flatbuf_vector_data"
)

// TestData 表示测试数据结构
type TestData struct {
	Vectors [][]float64 `json:"vectors"`
}

// 生成测试数据
func generateTestData(numVectors, vectorDim int) [][]float64 {
	data := make([][]float64, numVectors)
	for i := 0; i < numVectors; i++ {
		data[i] = make([]float64, vectorDim)
		for j := 0; j < vectorDim; j++ {
			data[i][j] = rand.Float64()
		}
	}
	return data
}

// 获取CPU使用率
func getCPUUsage() float64 {
	percentage, err := cpu.Percent(100*time.Millisecond, false)
	if err != nil {
		return 0
	}
	return percentage[0]
}

// 获取数据大小（字节）
func getDataSize(data [][]float64) int64 {
	return int64(len(data) * len(data[0]) * 8) // float64 = 8 bytes
}

// JSON序列化测试
func jsonSerializationTest(data [][]float64) map[string]float64 {
	originalSize := getDataSize(data)
	cpuStart := getCPUUsage()
	totalStartTime := time.Now()

	// 序列化
	startTime := time.Now()
	jsonData, _ := json.Marshal(TestData{Vectors: data})
	serializeTime := time.Since(startTime).Seconds()

	// 反序列化
	startTime = time.Now()
	var parsedData TestData
	json.Unmarshal(jsonData, &parsedData)
	deserializeTime := time.Since(startTime).Seconds()

	totalTime := time.Since(totalStartTime).Seconds()
	cpuEnd := getCPUUsage()

	return map[string]float64{
		"original_size":    float64(originalSize),
		"serialized_size": float64(len(jsonData)),
		"serialize_time":   serializeTime,
		"deserialize_time": deserializeTime,
		"total_time":      totalTime,
		"cpu_usage":       cpuEnd - cpuStart,
	}
}

// Protocol Buffers序列化测试
func protobufSerializationTest(data [][]float64) map[string]float64 {
	originalSize := getDataSize(data)
	cpuStart := getCPUUsage()
	totalStartTime := time.Now()

	// 构建protobuf消息
	dataset := &pb.VectorDataset{
		Vectors: make([]*pb.VectorData, len(data)),
	}
	for i, vector := range data {
		dataset.Vectors[i] = &pb.VectorData{
			Values: vector,
		}
	}

	// 序列化
	startTime := time.Now()
	serializedData, _ := proto.Marshal(dataset)
	serializeTime := time.Since(startTime).Seconds()

	// 反序列化
	startTime = time.Now()
	parsedDataset := &pb.VectorDataset{}
	proto.Unmarshal(serializedData, parsedDataset)
	deserializeTime := time.Since(startTime).Seconds()

	totalTime := time.Since(totalStartTime).Seconds()
	cpuEnd := getCPUUsage()

	return map[string]float64{
		"original_size":    float64(originalSize),
		"serialized_size": float64(len(serializedData)),
		"serialize_time":   serializeTime,
		"deserialize_time": deserializeTime,
		"total_time":      totalTime,
		"cpu_usage":       cpuEnd - cpuStart,
	}
}

// FlatBuffers序列化测试
func flatbuffersSerializationTest(data [][]float64) map[string]float64 {
	originalSize := getDataSize(data)
	cpuStart := getCPUUsage()
	totalStartTime := time.Now()

	// 构建部分不计入序列化时间
	// 预计算buffer大小
	estimatedSize := len(data)*len(data[0])*8 + 1024
	builder := flatbuffers.NewBuilder(estimatedSize)

	// 构建FlatBuffers的向量数据结构
	vectorOffsets := make([]flatbuffers.UOffsetT, len(data))
	for i, vector := range data {
		// 创建值数组
		fb.VectorDataStartValuesVector(builder, len(vector))
		for j := len(vector) - 1; j >= 0; j-- {
			builder.PrependFloat64(vector[j])
		}
		values := builder.EndVector(len(vector))

		// 创建VectorData
		fb.VectorDataStart(builder)
		fb.VectorDataAddValues(builder, values)
		vectorOffsets[i] = fb.VectorDataEnd(builder)
	}

	// 创建向量数组
	fb.VectorDatasetStartVectorsVector(builder, len(vectorOffsets))
	for i := len(vectorOffsets) - 1; i >= 0; i-- {
		builder.PrependUOffsetT(vectorOffsets[i])
	}
	vectors := builder.EndVector(len(vectorOffsets))

	// 开始序列化计时
	serializeStartTime := time.Now()
	
	// 完成数据集构建
	fb.VectorDatasetStart(builder)
	fb.VectorDatasetAddVectors(builder, vectors)
	dataset := fb.VectorDatasetEnd(builder)
	builder.Finish(dataset)

	// 获取序列化后的数据
	serializedData := builder.FinishedBytes()
	serializeTime := time.Since(serializeStartTime).Seconds()

	// 反序列化 - 需要实际访问数据才能测量反序列化性能
	startTime := time.Now()
	fbDataset := fb.GetRootAsVectorDataset(serializedData, 0)
	// 访问数据才能真正触发反序列化
	vectorsLen := fbDataset.VectorsLength()
	for i := 0; i < vectorsLen; i++ {
		vector := new(fb.VectorData)
		if fbDataset.Vectors(vector, i) {
			valuesLen := vector.ValuesLength()
			for j := 0; j < valuesLen; j++ {
				_ = vector.Values(j)
			}
		}
	}
	deserializeTime := time.Since(startTime).Seconds()

	totalTime := time.Since(totalStartTime).Seconds()
	cpuEnd := getCPUUsage()

	return map[string]float64{
		"original_size":    float64(originalSize),
		"serialized_size":  float64(len(serializedData)),
		"serialize_time":   serializeTime,
		"deserialize_time": deserializeTime,
		"total_time":       totalTime,
		"cpu_usage":        cpuEnd - cpuStart,
	}
}

// 运行基准测试
func runBenchmark(numIterations int) {
	fmt.Println("生成测试数据...")
	data := generateTestData(500, 768)

	results := map[string]map[string]float64{
		"JSON": {
			"original_size": 0, "serialized_size": 0,
			"serialize_time": 0, "deserialize_time": 0,
			"total_time": 0, "cpu_usage": 0,
		},
		"Protocol Buffers": {
			"original_size": 0, "serialized_size": 0,
			"serialize_time": 0, "deserialize_time": 0,
			"total_time": 0, "cpu_usage": 0,
		},
		"FlatBuffers": {
			"original_size": 0, "serialized_size": 0,
			"serialize_time": 0, "deserialize_time": 0,
			"total_time": 0, "cpu_usage": 0,
		},
	}

	fmt.Printf("\n运行%d次迭代的基准测试...\n", numIterations)
	for i := 0; i < numIterations; i++ {
		fmt.Printf("迭代 %d/%d\n", i+1, numIterations)

		// 运行所有测试
		testResults := map[string]map[string]float64{
			"JSON":             jsonSerializationTest(data),
			"Protocol Buffers": protobufSerializationTest(data),
			"FlatBuffers":      flatbuffersSerializationTest(data),
		}

		// 累加结果
		for format, metrics := range testResults {
			results[format]["original_size"] = metrics["original_size"]
			results[format]["serialized_size"] = metrics["serialized_size"]
			for key, value := range metrics {
				if key != "original_size" && key != "serialized_size" {
					results[format][key] += value / float64(numIterations)
				}
			}
		}
	}

	// 打印结果
	fmt.Println("\n基准测试结果:")
	divider := strings.Repeat("-", 125)
	fmt.Println(divider)
	fmt.Printf("%-15s %14s %14s %14s %14s %14s %10s\n",
		"序列化格式", "原始大小(MB)", "序列化(MB)",
		"序列化(ms)", "反序列化(ms)", "总时间(ms)", "CPU(%)")
	fmt.Println(divider)

	for format, metrics := range results {
		fmt.Printf("%-17s %16.3f %19.3f %17.3f %17.3f %19.3f %12.2f\n",
			format,
			metrics["original_size"]/(1024*1024),
			metrics["serialized_size"]/(1024*1024),
			metrics["serialize_time"]*1000,
			metrics["deserialize_time"]*1000,
			metrics["total_time"]*1000,
			metrics["cpu_usage"])
	}
	fmt.Println(divider)
}

func main() {
	rand.Seed(time.Now().UnixNano())
	runBenchmark(5)
}