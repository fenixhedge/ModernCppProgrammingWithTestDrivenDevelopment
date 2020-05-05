#include "Soundex.h"

#include "CppUTest/TestHarness.h"

TEST_GROUP(ASoundexEncoding) {};

TEST(ASoundexEncoding, RetainsSoleLetterOfOneLetterWord) {
   Soundex soundex;

   auto encoded = soundex.encode("A");

   CHECK_EQUAL("A", encoded);
}
