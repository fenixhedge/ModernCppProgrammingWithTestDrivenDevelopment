#include "gtest/gtest.h"
#include "WavReader.h"
#include <iostream>
#include <string>

using namespace std;

class WavReaderTest : public testing::Test {
};

TEST_F(WavReaderTest, HasExtensionTest) {
   string s{"a.bcd"};
   ASSERT_FALSE(hasExtension(s, "xxxx"));
   ASSERT_TRUE(hasExtension(s, "bcd"));
   //string bigger{"aaabcd"};
   //CHECK(!hasExtension(s, bigger));
}
