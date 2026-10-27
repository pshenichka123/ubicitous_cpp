#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libcompressor/libcompressor.hpp>
#include <string_view>

namespace {

void setup_logger() {
  auto logger = spdlog::stderr_logger_mt("compressor");
  logger->set_level(spdlog::level::err);
  spdlog::set_default_logger(logger);
}

bool parse_algorithm(std::string_view name, libcompressor_CompressionAlgorithm& algo) {
  if (name == "zlib") {
    algo = libcompressor_Zlib;
    return true;
  }

  if (name == "bzip") {
    algo = libcompressor_Bzip;
    return true;
  }

  return false;
}

void print_buffer(const libcompressor_Buffer& buffer) {
  for (int i = 0; i < buffer.size; ++i) {
    std::printf("%02hhx", static_cast<unsigned char>(buffer.data[i]));
  }
  std::printf("\n");
}

}  // namespace

int main(int argc, char* argv[]) {
  setup_logger();

  if (argc < 3) {
    spdlog::error("expected 2 arguments: <zlib|bzip> <string>, got {}", argc - 1);
    return EXIT_FAILURE;
  }

  libcompressor_CompressionAlgorithm algo{};
  if (!parse_algorithm(argv[1], algo)) {
    spdlog::error("unknown compression algorithm '{}', expected 'zlib' or 'bzip'", argv[1]);
    return EXIT_FAILURE;
  }

  const libcompressor_Buffer input{argv[2], static_cast<int>(std::strlen(argv[2]))};

  const libcompressor_Buffer output = libcompressor_compress(algo, input);
  if (output.data == nullptr || output.size == 0) {
    spdlog::error("compression failed for algorithm '{}'", argv[1]);
    return EXIT_FAILURE;
  }

  print_buffer(output);

  std::free(output.data);

  return EXIT_SUCCESS;
}
