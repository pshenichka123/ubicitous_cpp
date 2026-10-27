#pragma once
/**
 * @file libcompressor.hpp
 * @brief public interface for compress data
 */

/**
 * @brief supported compression algorithms
 */
enum libcompressor_CompressionAlgorithm {
  libcompressor_Zlib,  ///< compression using the zlib library
  libcompressor_Bzip   ///< compression using the bzip2 library
};

/**
 * @brief data buffer
 *
 * an empty buffer (data is nullptr and size is 0) is used
 * as an error indicator.
 */
struct libcompressor_Buffer {
  char* data;  ///< pointer to the buffer data
  int size;    ///< size of the buffer data
};

/**
 * @brief compresses the input buffer using the specified algorithm
 *
 *
 * @param algo the compression algorithm to use
 * @param input the input buffer containing data to compress
 * @return the output buffer containing compressed data.
 *
 * @see libcompressor_CompressionAlgorithm
 */
libcompressor_Buffer libcompressor_compress(libcompressor_CompressionAlgorithm algo, libcompressor_Buffer input);