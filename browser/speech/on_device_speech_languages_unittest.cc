/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/speech/on_device_speech_languages.h"

#include <optional>
#include <string_view>

#include "base/test/scoped_feature_list.h"
#include "brave/components/local_ai/core/features.h"
#include "brave/components/local_ai/core/on_device_speech_models_state.h"
#include "components/soda/constants.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace speech {

namespace {

using local_ai::OnDeviceSpeechModel;

constexpr OnDeviceSpeechModel kEnglish = OnDeviceSpeechModel::kEnglish;
constexpr OnDeviceSpeechModel kMultilingual =
    OnDeviceSpeechModel::kMultilingual;

}  // namespace

// Only en-US uses the English model. All other locales use the multilingual
// model.
TEST(OnDeviceSpeechLanguagesUnitTest, EachLocaleUsesItsModel) {
  base::test::ScopedFeatureList features(
      local_ai::kBraveOnDeviceSpeechMultilingual);
  for (const BraveOnDeviceSpeechLocale& locale :
       GetBraveOnDeviceSpeechLocales()) {
    EXPECT_EQ(locale.locale == "en-US" ? kEnglish : kMultilingual,
              GetOnDeviceSpeechModel(locale))
        << locale.locale;
  }
}

// With its feature disabled, the multilingual model serves no locale. The
// English model is not affected.
TEST(OnDeviceSpeechLanguagesUnitTest, MultilingualLocalesNeedTheirFeature) {
  base::test::ScopedFeatureList features;
  features.InitAndDisableFeature(local_ai::kBraveOnDeviceSpeechMultilingual);
  for (const BraveOnDeviceSpeechLocale& locale :
       GetBraveOnDeviceSpeechLocales()) {
    EXPECT_EQ(locale.locale == "en-US" ? std::optional(kEnglish) : std::nullopt,
              GetOnDeviceSpeechModel(locale))
        << locale.locale;
  }
}

}  // namespace speech
