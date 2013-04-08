/*! @file complex_to_real.cc
 *  @brief Converts complex numbers to the corresponding real numbers.
 *  @author Shapichev Alexey <a.shapichev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#define __STDC_LIMIT_MACROS
#include "src/transforms/complex_to_real.h"
#ifdef __AVX__
#include <immintrin.h>
#elif defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

namespace SpeechFeatureExtraction {
namespace Transforms {

void ComplexToReal::OnFormatChanged() {
  outputFormat_->SetSize(inputFormat_->Size() / 2);
}

void ComplexToReal::InitializeBuffers(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* buffers) const noexcept {
  buffers->Initialize(in.Size(), outputFormat_->Size());
}

void ComplexToReal::Do(
    const BuffersBase<Formats::WindowF>& in,
    BuffersBase<Formats::WindowF>* out) const noexcept {
  for (size_t i = 0; i < in.Size(); i++) {
    Do(true, in[i]->Data.get(), inputFormat_->Size(), (*out)[i]->Data.get());
  }
}

void ComplexToReal::Do(bool simd, const float* input, int length,
                       float* output) noexcept {
  if (simd) {
#ifdef __AVX__
    for (int i = 0; i < length - 15; i += 16) {
      __m256 vec1 = _mm256_load_ps(input + i);
      __m256 vec2 = _mm256_load_ps(input + i + 8);
      __m256 vec2even = _mm256_shuffle_ps(vec2, vec2, 160);
      __m256 low = _mm256_permute2f128_ps(vec1, vec2even, 32);
      __m256 high = _mm256_permute2f128_ps(vec1, vec2even, 49);
      __m256 result = _mm256_shuffle_ps(low, high, 136);
      _mm256_store_ps(output + i / 2, result);
    }
    for (int i = ((length >> 4) << 4); i < length; i += 2) {
      output[i / 2] = input[i];
    }
  } else {
#elif defined(__ARM_NEON__) //TODO(A.Shapichev): ARM_NEON implementation of C2R convertation
  } else {
#else
  } {
#endif
    for (int i = 0; i < length; i += 2) {
      output[i / 2] = input[i];
    }
  }
}

REGISTER_TRANSFORM(ComplexToReal);

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction
