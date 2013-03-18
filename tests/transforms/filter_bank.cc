/*! @file filter_bank.cc
 *  @brief Tests for SpeechFeatureExtraction::Transforms::FilterBank.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */



#include <gtest/gtest.h>
#include "src/transforms/filter_bank.h"

using namespace SpeechFeatureExtraction::Formats;
using SpeechFeatureExtraction::BuffersBase;
using SpeechFeatureExtraction::Transforms::FilterBank;

class FilterBankTest : public FilterBank, public testing::Test {
 public:
  BuffersBase<WindowF> Input;
  BuffersBase<WindowF> Output;
  int Size;

  virtual void SetUp() {
    Size = 378;
    Input.Initialize(1, Size);
    for (int i = 0; i < Size; i++) {
      Input[0]->Data.get()[i] = 100;
    }
    auto format = std::make_shared<WindowFormatF>(Size * 1000 / 18000, 18000);
    SetInputFormat(format);
    TypeSafeInitializeBuffers(Input, &Output);
    Initialize();
  }
};

#define EPSILON 0.000075f

#define ASSERT_EQF(a, b) do { \
  ASSERT_GT(a + EPSILON, b); \
  ASSERT_LT(a - EPSILON, b); \
} while (0)

TEST_F(FilterBankTest, Do) {
  TypeSafeDo(Input, &Output);
  for (int i = 0; i < Size; i++) {
    ASSERT_EQF(100 * filterBank_.get()[i], Output[0]->Data.get()[i]);
  }
}

#include "tests/google/src/gtest_main.cc"