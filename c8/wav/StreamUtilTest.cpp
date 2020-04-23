#include "gtest/gtest.h"

#include <fstream>
#include <sstream>

#include "StreamUtil.h"

using namespace std;

TEST(StreamUtil_Size, AnswersNumberOfBytesWrittenToStream) {
   istringstream readFrom{"abcdefg"};

   ASSERT_EQ(7, StreamUtil::size(readFrom));
}
