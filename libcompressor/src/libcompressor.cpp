#include <bzlib.h>
#include <zlib.h>

#include <cstdlib>
#include <libcompressor/libcompressor.hpp>

constexpr int kExtraSpace = 1024;

constexpr int kBzipBlockSize = 1;
constexpr int kBzipVerbosity = 0;
constexpr int kBzipWorkFactor = 0;

namespace {
bool compress_zlib(libcompressor_Buffer& input, libcompressor_Buffer& output) {
  auto dest_len = static_cast<uLongf>(output.size);

  const int rc = compress2(reinterpret_cast<Bytef*>(output.data), &dest_len, reinterpret_cast<const Bytef*>(input.data),
                           static_cast<uLong>(input.size), Z_DEFAULT_COMPRESSION);
  if (rc != Z_OK) {
    return false;
  }

  output.size = static_cast<int>(dest_len);
  return true;
}

bool compress_bzip(libcompressor_Buffer& input, libcompressor_Buffer& output) {
  auto dest_len = static_cast<unsigned int>(output.size);

  const int rc = BZ2_bzBuffToBuffCompress(output.data, &dest_len, input.data, static_cast<unsigned int>(input.size),
                                          kBzipBlockSize, kBzipVerbosity, kBzipWorkFactor);
  if (rc != BZ_OK) {
    return false;
  }

  output.size = static_cast<int>(dest_len);
  return true;
}
}  // namespace

libcompressor_Buffer libcompressor_compress(libcompressor_CompressionAlgorithm algo, libcompressor_Buffer input) {
  if (input.data == nullptr || input.size <= 0) {
    return libcompressor_Buffer{nullptr, 0};
  }

  bool result = false;

  libcompressor_Buffer output;
  output.data = static_cast<char*>(std::malloc(input.size + kExtraSpace));
  output.size = input.size + kExtraSpace;

  if (algo == libcompressor_CompressionAlgorithm::libcompressor_Zlib) {
    result = compress_zlib(input, output);
  } else if (algo == libcompressor_CompressionAlgorithm::libcompressor_Bzip) {
    result = compress_bzip(input, output);
  } else {
    result = false;
  }

  if (!result) {
    std::free(output.data);
    output.data = nullptr;
    output.size = 0;
  }

  return output;
}