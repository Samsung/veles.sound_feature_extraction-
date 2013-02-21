/*! @file highpass_filter.h
 *  @brief New file description.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef HIGHPASS_FILTER_H_
#define HIGHPASS_FILTER_H_

#include "src/transforms/fir_filter_base.h"

namespace SpeechFeatureExtraction {
namespace Transforms {

/// @brief Discards the frequencies which are lower than the threshold.
class HighpassFilter : public FirFilterBase {
 public:
  HighpassFilter() noexcept;

  TRANSFORM_NAME("Highpass")

  FIR_FILTER_PARAMETERS(
      _TP_("frequency", "Cutoff frequency",
                        std::to_string(DEFAULT_FILTER_LOW_FREQUENCY))
  )

 protected:
  virtual void SetFilterParameter(const std::string& name,
                                  const std::string& value);

  virtual void CalculateFilter(float *filter) const noexcept;

 private:
  int frequency_;
};

}  // namespace Transforms
}  // namespace SpeechFeatureExtraction
#endif  // INCLUDE_HIGHPASS_FILTER_H_