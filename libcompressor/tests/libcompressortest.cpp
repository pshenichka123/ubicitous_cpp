#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdlib>
#include <libcompressor/libcompressor.hpp>
#include <vector>

namespace {

char kTestString[] = "test_string";
constexpr int kTestStringSize = static_cast<int>(sizeof(kTestString) - 1);

constexpr auto kZlibExpected = std::to_array<unsigned char>({
    0x78, 0x9c, 0x2b, 0x49, 0x2d, 0x2e, 0x89, 0x2f, 0x2e, 0x29, 0xca, 0xcc, 0x4b, 0x07, 0x00, 0x1c, 0x79, 0x04, 0xb7,
});

constexpr auto kBzipExpected = std::to_array<unsigned char>({
    0x42, 0x5a, 0x68, 0x31, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0x4a, 0x7c, 0x69, 0x05, 0x00, 0x00,
    0x04, 0x83, 0x80, 0x00, 0x00, 0x82, 0xa1, 0x1c, 0x00, 0x20, 0x00, 0x22, 0x03, 0x68, 0x84, 0x30,
    0x22, 0x50, 0xdf, 0x04, 0x99, 0xe2, 0xee, 0x48, 0xa7, 0x0a, 0x12, 0x09, 0x4f, 0x8d, 0x20, 0xa0,
});

class BufferGuard {
 public:
  explicit BufferGuard(libcompressor_Buffer buffer) : buffer_(buffer) {}
  ~BufferGuard() { std::free(buffer_.data); }

  BufferGuard(const BufferGuard&) = delete;
  BufferGuard& operator=(const BufferGuard&) = delete;
  BufferGuard(BufferGuard&&) = delete;
  BufferGuard& operator=(BufferGuard&&) = delete;

  [[nodiscard]] const libcompressor_Buffer& get() const { return buffer_; }

 private:
  libcompressor_Buffer buffer_;
};

template <std::size_t N>
void ExpectBytesEqual(const libcompressor_Buffer& actual, const std::array<unsigned char, N>& expected) {
  ASSERT_NE(actual.data, nullptr);
  ASSERT_EQ(static_cast<std::size_t>(actual.size), expected.size());

  for (std::size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(static_cast<unsigned char>(actual.data[i]), expected[i]) << "mismatch at byte " << i;
  }
}

}  // namespace

class CompressBothAlgorithms : public ::testing::TestWithParam<libcompressor_CompressionAlgorithm> {};

TEST_P(CompressBothAlgorithms, NonEmptyInputProducesNonEmptyOutput) {
  const libcompressor_Buffer input{kTestString, kTestStringSize};

  const BufferGuard output(libcompressor_compress(GetParam(), input));

  EXPECT_NE(output.get().data, nullptr);
  EXPECT_GT(output.get().size, 0);
}

TEST_P(CompressBothAlgorithms, NullInputProducesEmptyOutput) {
  const libcompressor_Buffer input{nullptr, 0};

  const BufferGuard output(libcompressor_compress(GetParam(), input));

  EXPECT_EQ(output.get().data, nullptr);
  EXPECT_EQ(output.get().size, 0);
}

TEST_P(CompressBothAlgorithms, ZeroSizeInputProducesEmptyOutput) {
  const libcompressor_Buffer input{kTestString, 0};

  const BufferGuard output(libcompressor_compress(GetParam(), input));

  EXPECT_EQ(output.get().data, nullptr);
  EXPECT_EQ(output.get().size, 0);
}

INSTANTIATE_TEST_SUITE_P(AllAlgorithms, CompressBothAlgorithms,
                         ::testing::Values(libcompressor_Zlib, libcompressor_Bzip));

TEST(CompressZlib, TestStringProducesExpectedBytes) {
  const libcompressor_Buffer input{kTestString, kTestStringSize};

  const BufferGuard output(libcompressor_compress(libcompressor_Zlib, input));

  ExpectBytesEqual(output.get(), kZlibExpected);
}

TEST(CompressBzip, TestStringProducesExpectedBytes) {
  const libcompressor_Buffer input{kTestString, kTestStringSize};

  const BufferGuard output(libcompressor_compress(libcompressor_Bzip, input));

  ExpectBytesEqual(output.get(), kBzipExpected);
}