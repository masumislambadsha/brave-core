/* Copyright (c) 2026 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <optional>
#include <set>
#include <string_view>

#include "base/i18n/language_tag.h"
#include "base/i18n/tag_converters.h"
#include "components/soda/constants.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace speech {

namespace {

std::optional<std::string_view> Resolve(std::string_view language) {
  const BraveOnDeviceSpeechLocale* locale =
      ResolveBraveOnDeviceSpeechLocale(language);
  return locale ? std::optional(locale->locale) : std::nullopt;
}

}  // namespace

// `ResolveBraveOnDeviceSpeechLocale` finds the row of a match by its tag.
TEST(BraveSodaConstantsUnitTest, LocalesAreUniqueAndCanonical) {
  std::set<std::string_view> names;
  for (const BraveOnDeviceSpeechLocale& locale :
       GetBraveOnDeviceSpeechLocales()) {
    SCOPED_TRACE(locale.locale);
    EXPECT_TRUE(names.insert(locale.locale).second);
    std::optional<base::i18n::LanguageTag> tag =
        base::i18n::GetLanguageTagFromString(locale.locale);
    ASSERT_TRUE(tag);
    EXPECT_EQ(locale.locale, tag->tag_string());
    EXPECT_EQ(locale.locale, Resolve(locale.locale));
  }
}

// A region can select a locale that is not the first of its language. See
// `GetLocaleMatcher()`.
TEST(BraveSodaConstantsUnitTest, ResolvesSpanishAndPortugueseByRegion) {
  EXPECT_EQ("es-ES", Resolve("es"));
  EXPECT_EQ("es-US", Resolve("es-MX"));
  EXPECT_EQ("pt-BR", Resolve("pt"));
  EXPECT_EQ("pt-PT", Resolve("pt-AO"));
}

TEST(BraveSodaConstantsUnitTest, RefusesTagsThatNoModelServes) {
  // Languages that no locale has, and tags that do not parse.
  for (std::string_view language :
       {"zh-CN", "zh-TW", "pl-PL", "nb-NO", "th-TH", "", "not a tag"}) {
    EXPECT_EQ(std::nullopt, Resolve(language)) << language;
  }
}

}  // namespace speech
