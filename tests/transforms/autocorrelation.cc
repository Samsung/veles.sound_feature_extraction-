/*! @file autocorrelation.cc
 *  @brief Tests for sound_feature_extraction::transforms::Autocorrelation.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */


#include "src/transforms/autocorrelation.h"
#include "tests/transforms/transform_test.h"
#include <fftf/api.h>

using sound_feature_extraction::formats::ArrayFormatF;
using sound_feature_extraction::BuffersBase;
using sound_feature_extraction::transforms::Autocorrelation;

class AutocorrelationTest : public TransformTest<Autocorrelation> {
 public:
  int Size;

  virtual void SetUp() {
    Size = 65536;
    SetUpTransform(1, Size, 18000);
    for (int i = 0; i < Size / 2; i++) {
      (*Input)[0][i] = (-i + 1) * 2.f / Size;
    }
    for (int i = Size / 2; i < Size; i++) {
      (*Input)[0][i] = (Size - i + 1) * 2.f / Size;
    }
  }
};

TEST_F(AutocorrelationTest, Do) {
  Do((*Input)[0], (*Output)[0]);
  ASSERT_NEAR((*Output)[0][0], 2 * 2.f / Size, 1.f);
  ASSERT_NEAR((*Output)[0][1], 3 * 2.f / Size, 1.f);
  ASSERT_NEAR((*Output)[0][3], -2 * 2.f / Size, 1.f);
  ASSERT_NEAR((*Output)[0][Size - 2], (*Output)[0][Size], 0.1);
}

TEST_F(AutocorrelationTest, DoNormalized) {
  SetParameter("normalize", "true");
  Do((*Input)[0], (*Output)[0]);
  for (int i = 0; i < Size * 2 - 1; i++) {
    ASSERT_LE((*Output)[0][i], 1.f) << i;
  }
  ASSERT_FLOAT_EQ(1.f, (*Output)[0][Size - 1]);
}
