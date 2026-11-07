/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/speech/on_device_speech_languages.h"

#include <optional>
#include <string_view>

#include "base/feature_list.h"
#include "brave/components/local_ai/core/features.h"
#include "brave/components/local_ai/core/on_device_speech_models_state.h"
#include "components/soda/constants.h"

namespace speech {

std::optional<local_ai::OnDeviceSpeechModel> GetOnDeviceSpeechModel(
    const BraveOnDeviceSpeechLocale& locale) {
  // The one locale for all English tags.
  if (locale.locale == "en-US") {
    return local_ai::OnDeviceSpeechModel::kEnglish;
  }
  if (!base::FeatureList::IsEnabled(
          local_ai::kBraveOnDeviceSpeechMultilingual)) {
    return std::nullopt;
  }
  return local_ai::OnDeviceSpeechModel::kMultilingual;
}

}  // namespace speech
