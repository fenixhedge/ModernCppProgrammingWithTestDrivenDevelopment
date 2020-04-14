#include "gtest/gtest.h"
#include "RomanConverter.h"

TEST(ARomanConverter, AnswersArabicEquivalents) {
	RomanConverter converter;
	ASSERT_EQ("I", converter.convert(1));
	ASSERT_EQ("II", converter.convert(2));
	ASSERT_EQ("III", converter.convert(3));
	ASSERT_EQ("IV", converter.convert(4));
	ASSERT_EQ("V", converter.convert(5));
}
