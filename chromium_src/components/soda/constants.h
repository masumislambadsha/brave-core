/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_SODA_CONSTANTS_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_SODA_CONSTANTS_H_

#include <components/soda/constants.h>  // IWYU pragma: export

#include <string_view>

#include "base/containers/span.h"

namespace speech {

// A locale that Brave's on-device speech recognition serves.
struct BraveOnDeviceSpeechLocale {
  // The canonical BCP 47 tag. The speech worker gets it as the language of a
  // stream.
  std::string_view locale;
};

// All the locales that Brave's on-device speech recognition serves.
base::span<const BraveOnDeviceSpeechLocale> GetBraveOnDeviceSpeechLocales();

// The locale that serves `language`, a page's BCP 47 tag, or null when no
// locale does. For example, "de-AT" gives "de-DE".
const BraveOnDeviceSpeechLocale* ResolveBraveOnDeviceSpeechLocale(
    std::string_view language);

}  // namespace speech

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_SODA_CONSTANTS_H_
