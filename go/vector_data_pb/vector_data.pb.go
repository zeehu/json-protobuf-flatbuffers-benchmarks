// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.6
// 	protoc        v3.12.4
// source: vector_data.proto

package proto_vector_data

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
	unsafe "unsafe"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

// VectorData 表示一个向量（一维数组）
type VectorData struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	Values        []float64              `protobuf:"fixed64,1,rep,packed,name=values,proto3" json:"values,omitempty"` // 向量中的值
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *VectorData) Reset() {
	*x = VectorData{}
	mi := &file_vector_data_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *VectorData) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*VectorData) ProtoMessage() {}

func (x *VectorData) ProtoReflect() protoreflect.Message {
	mi := &file_vector_data_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use VectorData.ProtoReflect.Descriptor instead.
func (*VectorData) Descriptor() ([]byte, []int) {
	return file_vector_data_proto_rawDescGZIP(), []int{0}
}

func (x *VectorData) GetValues() []float64 {
	if x != nil {
		return x.Values
	}
	return nil
}

// VectorDataset 表示向量的集合
type VectorDataset struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	Vectors       []*VectorData          `protobuf:"bytes,1,rep,name=vectors,proto3" json:"vectors,omitempty"` // 多个向量的集合
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *VectorDataset) Reset() {
	*x = VectorDataset{}
	mi := &file_vector_data_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *VectorDataset) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*VectorDataset) ProtoMessage() {}

func (x *VectorDataset) ProtoReflect() protoreflect.Message {
	mi := &file_vector_data_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use VectorDataset.ProtoReflect.Descriptor instead.
func (*VectorDataset) Descriptor() ([]byte, []int) {
	return file_vector_data_proto_rawDescGZIP(), []int{1}
}

func (x *VectorDataset) GetVectors() []*VectorData {
	if x != nil {
		return x.Vectors
	}
	return nil
}

var File_vector_data_proto protoreflect.FileDescriptor

const file_vector_data_proto_rawDesc = "" +
	"\n" +
	"\x11vector_data.proto\x12\x11proto.vector_data\"$\n" +
	"\n" +
	"VectorData\x12\x16\n" +
	"\x06values\x18\x01 \x03(\x01R\x06values\"H\n" +
	"\rVectorDataset\x127\n" +
	"\avectors\x18\x01 \x03(\v2\x1d.proto.vector_data.VectorDataR\avectorsBC\n" +
	"\x1fcom.benchmark.proto.vector_dataP\x01Z\x1bbenchmark/proto_vector_data\xf8\x01\x01b\x06proto3"

var (
	file_vector_data_proto_rawDescOnce sync.Once
	file_vector_data_proto_rawDescData []byte
)

func file_vector_data_proto_rawDescGZIP() []byte {
	file_vector_data_proto_rawDescOnce.Do(func() {
		file_vector_data_proto_rawDescData = protoimpl.X.CompressGZIP(unsafe.Slice(unsafe.StringData(file_vector_data_proto_rawDesc), len(file_vector_data_proto_rawDesc)))
	})
	return file_vector_data_proto_rawDescData
}

var file_vector_data_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_vector_data_proto_goTypes = []any{
	(*VectorData)(nil),    // 0: proto.vector_data.VectorData
	(*VectorDataset)(nil), // 1: proto.vector_data.VectorDataset
}
var file_vector_data_proto_depIdxs = []int32{
	0, // 0: proto.vector_data.VectorDataset.vectors:type_name -> proto.vector_data.VectorData
	1, // [1:1] is the sub-list for method output_type
	1, // [1:1] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_vector_data_proto_init() }
func file_vector_data_proto_init() {
	if File_vector_data_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: unsafe.Slice(unsafe.StringData(file_vector_data_proto_rawDesc), len(file_vector_data_proto_rawDesc)),
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_vector_data_proto_goTypes,
		DependencyIndexes: file_vector_data_proto_depIdxs,
		MessageInfos:      file_vector_data_proto_msgTypes,
	}.Build()
	File_vector_data_proto = out.File
	file_vector_data_proto_goTypes = nil
	file_vector_data_proto_depIdxs = nil
}
