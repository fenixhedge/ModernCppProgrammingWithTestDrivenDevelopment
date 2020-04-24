#include "gtest/gtest.h"
#include "gmock/gmock.h"

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

class WavReader_WriteSamples : public Test {
public:
    WavReader reader{"", ""};
    ostringstream out;
};

TEST_F(WavReader_WriteSamples, WritesSingleSample) {
    char data[] {"abcd"};
    uint32_t bytesPerSample {1};
    uint32_t startingSample {0};
    uint32_t samplesToWrite {1};
    reader.writeSamples(&out, data, startingSample, samplesToWrite, bytesPerSample);
    ASSERT_EQ("a", out.str());
}

TEST_F(WavReader_WriteSamples, WritesMultibyteSampleFromMiddle) {
    char data[] {"0123456789ABCDEFG"};
    uint32_t bytesPerSample { 2 };
    uint32_t startingSample { 4 };
    uint32_t samplesToWrite { 3 };

    reader.writeSamples(&out, data, startingSample, samplesToWrite, bytesPerSample);
    ASSERT_EQ("89ABCD", out.str());
}

TEST_F(WavReader_WriteSamples, IncorporatesChannelCount) {
    char data[] {"0123456789ABCDEFG"};
    uint32_t bytesPerSample { 2 };
    uint32_t startingSample { 0 };
    uint32_t samplesToWrite { 2 };
    uint32_t channels { 2 };

    reader.writeSamples(
          &out, data, startingSample, samplesToWrite, bytesPerSample, channels);

    ASSERT_EQ("01234567", out.str());
}

class WavReader_DataLength : public Test {
public:
    WavReader reader{"", ""};
};

TEST_F(WavReader_DataLength, IsProductOfChannels_BytesPerSample_and_Samples) {
    uint32_t bytesPerSample { 2 };
    uint32_t samples { 5 };
    uint32_t channels { 4 };
    
    uint32_t length { reader.dataLength(bytesPerSample, samples, channels) };

    ASSERT_EQ(2 * 5 * 4, length);
}

class MockWavDescriptor : public WavDescriptor {
public:
   MockWavDescriptor() : WavDescriptor("") {}

   MOCK_METHOD(void, add, (const string&, const string&, uint32_t, uint32_t, uint32_t, uint32_t), (override));
};

class MockFileUtil : public FileUtil {
public:
   MOCK_METHOD(streamsize, size, (const string& name), (override));
};

class WavReader_WriteSnippet : public Test {
public:
   shared_ptr<MockWavDescriptor> descriptor{new MockWavDescriptor};
   WavReader reader{"", "", descriptor};

   shared_ptr<MockFileUtil> fileUtil{make_shared<MockFileUtil>()};

   istringstream input{""};
   FormatSubchunk formatSubchunk;
   ostringstream output;
   DataChunk dataChunk;
   char* data;
   uint32_t TwoBytesWorthOfBits{2 * 8};
   
   const int ArbitraryFileSize{5};

   void SetUp() override {
      data = new char[4];
      reader.useFileUtil(fileUtil);
   }

   void TearDown() override {
      delete[] data;
   }
};

TEST_F(WavReader_WriteSnippet, SendsFileLengthAndTotalSecondsToDescriptor) {
   dataChunk.length = 8;
   formatSubchunk.bitsPerSample = TwoBytesWorthOfBits;
   formatSubchunk.samplesPerSecond = 1;

   EXPECT_CALL(*fileUtil, size)
      .WillOnce(Return(ArbitraryFileSize));

   uint32_t actual_totalSeconds;
   uint32_t actual_fileSize;
   EXPECT_CALL(*descriptor, add)
      .WillOnce(DoAll(SaveArg<2>(&actual_totalSeconds), SaveArg<5>(&actual_fileSize)));

   reader.writeSnippet("any", input, output, formatSubchunk, dataChunk, data);

   EXPECT_THAT(8 / 2 / 1, actual_totalSeconds);
   EXPECT_THAT(ArbitraryFileSize, actual_fileSize);
}
