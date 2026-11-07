/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_SPEECH_ON_DEVICE_SPEECH_LANGUAGES_H_
#define BRAVE_BROWSER_SPEECH_ON_DEVICE_SPEECH_LANGUAGES_H_

#include <optional>

#include "brave/components/local_ai/core/on_device_speech_models_state.h"

namespace speech {

struct BraveOnDeviceSpeechLocale;

// The model that transcribes `locale`: the English model for `en-US`, and the
// multilingual model for all other locales. Nothing for the multilingual
// model's locales while `kBraveOnDeviceSpeechMultilingual` is disabled.
std::optional<local_ai::OnDeviceSpeechModel> GetOnDeviceSpeechModel(
    const BraveOnDeviceSpeechLocale& locale);

}  // namespace speech

#endif  // BRAVE_BROWSER_SPEECH_ON_DEVICE_SPEECH_LANGUAGES_H_
