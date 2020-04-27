#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include "WavReader.h"

using namespace std;
using namespace testing;

class WavReaderTest : public Test {
};

TEST(WavReaderTest, HasExtensionTest) {
   string s{"a.bcd"};
   ASSERT_FALSE(hasExtension(s, "xxxx"));
   ASSERT_TRUE(hasExtension(s, "bcd"));
   string bigger{"aaabcd"};
   ASSERT_TRUE(!hasExtension(s, bigger));
}
