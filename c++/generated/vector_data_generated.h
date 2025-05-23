// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_VECTORDATA_FLATBUF_VECTOR_DATA_H_
#define FLATBUFFERS_GENERATED_VECTORDATA_FLATBUF_VECTOR_DATA_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 25 &&
              FLATBUFFERS_VERSION_MINOR == 2 &&
              FLATBUFFERS_VERSION_REVISION == 10,
             "Non-compatible flatbuffers version included");

namespace flatbuf {
namespace vector_data {

struct VectorData;
struct VectorDataBuilder;

struct VectorDataset;
struct VectorDatasetBuilder;

struct VectorData FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef VectorDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VALUES = 4
  };
  const ::flatbuffers::Vector<double> *values() const {
    return GetPointer<const ::flatbuffers::Vector<double> *>(VT_VALUES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_VALUES) &&
           verifier.VerifyVector(values()) &&
           verifier.EndTable();
  }
};

struct VectorDataBuilder {
  typedef VectorData Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_values(::flatbuffers::Offset<::flatbuffers::Vector<double>> values) {
    fbb_.AddOffset(VectorData::VT_VALUES, values);
  }
  explicit VectorDataBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<VectorData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<VectorData>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<VectorData> CreateVectorData(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<double>> values = 0) {
  VectorDataBuilder builder_(_fbb);
  builder_.add_values(values);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<VectorData> CreateVectorDataDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<double> *values = nullptr) {
  auto values__ = values ? _fbb.CreateVector<double>(*values) : 0;
  return flatbuf::vector_data::CreateVectorData(
      _fbb,
      values__);
}

struct VectorDataset FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef VectorDatasetBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VECTORS = 4
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::vector_data::VectorData>> *vectors() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::vector_data::VectorData>> *>(VT_VECTORS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_VECTORS) &&
           verifier.VerifyVector(vectors()) &&
           verifier.VerifyVectorOfTables(vectors()) &&
           verifier.EndTable();
  }
};

struct VectorDatasetBuilder {
  typedef VectorDataset Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_vectors(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::vector_data::VectorData>>> vectors) {
    fbb_.AddOffset(VectorDataset::VT_VECTORS, vectors);
  }
  explicit VectorDatasetBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<VectorDataset> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<VectorDataset>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<VectorDataset> CreateVectorDataset(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::vector_data::VectorData>>> vectors = 0) {
  VectorDatasetBuilder builder_(_fbb);
  builder_.add_vectors(vectors);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<VectorDataset> CreateVectorDatasetDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<::flatbuffers::Offset<flatbuf::vector_data::VectorData>> *vectors = nullptr) {
  auto vectors__ = vectors ? _fbb.CreateVector<::flatbuffers::Offset<flatbuf::vector_data::VectorData>>(*vectors) : 0;
  return flatbuf::vector_data::CreateVectorDataset(
      _fbb,
      vectors__);
}

inline const flatbuf::vector_data::VectorDataset *GetVectorDataset(const void *buf) {
  return ::flatbuffers::GetRoot<flatbuf::vector_data::VectorDataset>(buf);
}

inline const flatbuf::vector_data::VectorDataset *GetSizePrefixedVectorDataset(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<flatbuf::vector_data::VectorDataset>(buf);
}

inline const char *VectorDatasetIdentifier() {
  return "VDAT";
}

inline bool VectorDatasetBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, VectorDatasetIdentifier());
}

inline bool SizePrefixedVectorDatasetBufferHasIdentifier(const void *buf) {
  return ::flatbuffers::BufferHasIdentifier(
      buf, VectorDatasetIdentifier(), true);
}

inline bool VerifyVectorDatasetBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<flatbuf::vector_data::VectorDataset>(VectorDatasetIdentifier());
}

inline bool VerifySizePrefixedVectorDatasetBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<flatbuf::vector_data::VectorDataset>(VectorDatasetIdentifier());
}

inline const char *VectorDatasetExtension() {
  return "fbdat";
}

inline void FinishVectorDatasetBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<flatbuf::vector_data::VectorDataset> root) {
  fbb.Finish(root, VectorDatasetIdentifier());
}

inline void FinishSizePrefixedVectorDatasetBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<flatbuf::vector_data::VectorDataset> root) {
  fbb.FinishSizePrefixed(root, VectorDatasetIdentifier());
}

}  // namespace vector_data
}  // namespace flatbuf

#endif  // FLATBUFFERS_GENERATED_VECTORDATA_FLATBUF_VECTOR_DATA_H_
