#include "Soundex.h"

#include "CppUTest/TestHarness.h"

TEST_GROUP(ASoundexEncoding) {
   Soundex soundex;
};

TEST(ASoundexEncoding, RetainsSoleLetterOfOneLetterWord) {
   CHECK_EQUAL("A000", soundex.encode("A"));
   CHECK_EQUAL("B000", soundex.encode("B"));
}