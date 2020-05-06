#include "Soundex.h"

#include "CppUTest/TestHarness.h"

TEST_GROUP(ASoundexEncoding) {
   Soundex soundex;
};

TEST(ASoundexEncoding, RetainsSoleLetterOfOneLetterWord) {
   CHECK_EQUAL("A000", soundex.encode("A"));
   CHECK_EQUAL("B000", soundex.encode("B"));
}

TEST(ASoundexEncoding, ReplacesConsonantsWithAppropriateDigits) {
   CHECK_EQUAL("A100", soundex.encode("Ab"));
   CHECK_EQUAL("A200", soundex.encode("Ac"));
}

TEST(ASoundexEncoding, ReplacesMultipleConsonantsWithDigits) {
   CHECK_EQUAL("A234", soundex.encode("Acdl"));
}
