/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/soda/constants.h"

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "base/check.h"
#include "base/containers/span.h"
#include "base/i18n/language_tag.h"
#include "base/i18n/language_tag_matcher.h"
#include "base/i18n/tag_converters.h"
#include "base/no_destructor.h"
#include "base/strings/string_util.h"

namespace speech {

namespace {

// Each tag must be a canonical BCP 47 tag, and the speech worker must accept
// it. The matcher gives "fr" or "fr-BE" the first of two locales at the same
// distance, so "fr-FR" must come before "fr-CA".
constexpr auto kLocales = std::to_array<BraveOnDeviceSpeechLocale>({
    {"en-US", LanguageCode::kEnUs},
    {"ar", LanguageCode::kAr},
    {"de-DE", LanguageCode::kDeDe},
    {"es-ES", LanguageCode::kEsEs},
    {"es-US", LanguageCode::kEsUs},
    {"fr-FR", LanguageCode::kFrFr},
    {"fr-CA", LanguageCode::kFrCa},
    {"hi-IN", LanguageCode::kHiIn},
    {"it-IT", LanguageCode::kItIt},
    {"ja-JP", LanguageCode::kJaJp},
    {"ko-KR", LanguageCode::kKoKr},
    {"nl-NL", LanguageCode::kNlNl},
    {"pt-BR", LanguageCode::kPtBr},
    {"pt-PT", LanguageCode::kPtPt},
    {"ru-RU", LanguageCode::kRuRu},
    {"tr-TR", LanguageCode::kTrTr},
    {"uk-UA", LanguageCode::kUkUa},
    {"vi-VN", LanguageCode::kViVn},
});

// Matches a page's tag against the locales. A region can select a locale that
// is not the first of its language, through its CLDR parent locale: "es-MX"
// gives "es-US" (both under es-419), and "pt-AO" gives "pt-PT". A match on the
// language alone gives es-ES and pt-BR. Built once, because building it
// computes the distance from each locale to its parent locales.
const base::i18n::LanguageTagMatcher& GetLocaleMatcher() {
  static const base::NoDestructor<base::i18n::LanguageTagMatcher> matcher([] {
    std::vector<base::i18n::LanguageTag> tags;
    for (const BraveOnDeviceSpeechLocale& locale : kLocales) {
      std::optional<base::i18n::LanguageTag> tag =
          base::i18n::GetLanguageTagFromString(locale.locale);
      CHECK(tag) << locale.locale;
      tags.push_back(*std::move(tag));
    }
    return base::i18n::LanguageTagMatcher::Create(tags);
  }());
  return *matcher;
}

// The locale whose tag is `locale`, ignoring case, or null.
const BraveOnDeviceSpeechLocale* FindBraveOnDeviceSpeechLocaleByName(
    std::string_view locale) {
  auto it = std::ranges::find_if(
      kLocales, [locale](const BraveOnDeviceSpeechLocale& candidate) {
        return base::EqualsCaseInsensitiveASCII(candidate.locale, locale);
      });
  return it != kLocales.end() ? &*it : nullptr;
}

// The locale whose code is `language_code`, or null.
const BraveOnDeviceSpeechLocale* FindBraveOnDeviceSpeechLocaleByCode(
    LanguageCode language_code) {
  auto it = std::ranges::find(kLocales, language_code,
                              &BraveOnDeviceSpeechLocale::language_code);
  return it != kLocales.end() ? &*it : nullptr;
}

}  // namespace

base::span<const BraveOnDeviceSpeechLocale> GetBraveOnDeviceSpeechLocales() {
  return kLocales;
}

const BraveOnDeviceSpeechLocale* ResolveBraveOnDeviceSpeechLocale(
    std::string_view language) {
  std::optional<base::i18n::LanguageTag> tag =
      base::i18n::GetLanguageTagFromString(language);
  if (!tag) {
    return nullptr;
  }
  std::optional<base::i18n::LanguageTag> match = GetLocaleMatcher().Match(*tag);
  return match ? FindBraveOnDeviceSpeechLocaleByName(match->tag_string())
               : nullptr;
}

// The functions in this namespace are file-local. The plaster changes to the
// upstream source below call them, because that source is in this file.
namespace {

// The SODA language config that stands for the locale that serves `language`,
// or none when no locale does. Its name is the locale's tag and its code is the
// locale's code.
std::optional<SodaLanguagePackComponentConfig>
GetBraveOnDeviceSpeechLanguageConfig(std::string_view language) {
  const BraveOnDeviceSpeechLocale* locale =
      ResolveBraveOnDeviceSpeechLocale(language);
  if (!locale) {
    return std::nullopt;
  }
  // SODA's own config, where SODA has one for the code, keeps its pref names
  // and key. Only SODA's own install code reads them, and Brave does not run
  // it, so a config without them is safe too.
  std::optional<SodaLanguagePackComponentConfig> config =
      GetLanguageComponentConfig(locale->language_code);
  if (!config) {
    return SodaLanguagePackComponentConfig{
        .language_code = locale->language_code,
        .language_name = locale->locale,
    };
  }
  config->language_name = locale->locale;
  return config;
}

}  // namespace

}  // namespace speech

#include <components/soda/constants.cc>
