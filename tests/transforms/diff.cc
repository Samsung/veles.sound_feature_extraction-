/*! @file diffrect.cc
 *  @brief Tests for SoundFeatureExtraction::Transforms::Diff.
 *  @author Shapichev Alexey <a.shapichev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */


#include "src/transforms/diff.h"
#include "tests/transforms/transform_test.h"

using SoundFeatureExtraction::Formats::RawFormatF;
using SoundFeatureExtraction::BuffersBase;
using SoundFeatureExtraction::Transforms::Diff;

class DiffTest : public TransformTest<Diff> {
 public:
  int Size;

  virtual void SetUp() {
    Size = 486;
    SetParameter("rectify", "true");
    SetUpTransform(1, Size, 18000);
    (*Input)[0][0] = 0;
    for (int i = 1; i < Size; i++) {
      int step = (i - 1) % 3;
      if ((i - 1) % 2 == 0)
        step *= -1;
      (*Input)[0][i] = (*Input)[0][i - 1] + step;
    }
  }
};

TEST_F(DiffTest, Do) {
  Do((*Input)[0], (*Output)[0]);
  for (int i = 0; i < Size - 1; i++) {
    int result = (i % 3);
    if (i % 2 == 0) {
      result = 0;
    }
    ASSERT_EQ(result, (*Output)[0][i]);
  }
}

#define CLASS_NAME DiffTest
#define ITER_COUNT 500000
#include "tests/transforms/benchmark.inc"