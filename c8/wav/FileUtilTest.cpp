#include "gtest/gtest.h"

#include <fstream>
#include <ostream>
#include <stdio.h>

#include <string>

#include "FileUtil.h"
#include "StreamUtil.h"

using namespace std;
using namespace testing;

class FileUtilTest : public Test {
protected:
   const string TempFileName{"FileUtil_SizeTest.dat"};
   FileUtil util;

   void TearDown() override {
      remove(TempFileName.c_str());
   }

   void createTempFile(const string& content) {
      ofstream stream{TempFileName, ios::out | ios::binary};
      stream << content << endl;
      stream.close();
   }
};

class FileUtil_Size : public FileUtilTest {
};

TEST_F(FileUtil_Size, AnswersFileContentSize) {
   string content{"12345"};
   createTempFile(content);

   size_t expectedSize { content.length() + sizeof('\0') };
   ASSERT_EQ(expectedSize, (unsigned)util.size(TempFileName));
}

class FileUtil_Execute : public FileUtilTest {
};

TEST_F(FileUtil_Execute, IsPassedStreamFromFile) {
   string content{"abcdefg"};
   streamsize returnValue{999};
   createTempFile(content);
   string buffer;

   auto func = [&](istream& stream) {
      getline(stream, buffer);
      return returnValue;
   };
   auto result = util.execute(TempFileName, func);

   ASSERT_EQ(returnValue, result);
   ASSERT_EQ(content, buffer);
}
