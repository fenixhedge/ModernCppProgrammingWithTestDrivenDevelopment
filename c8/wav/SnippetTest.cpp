#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <string>

#include "Snippet.h"

using namespace std;
using namespace testing;

class MockFileUtil : public FileUtil {
public:
   MOCK_METHOD(streamsize, size, (const string& name), (override));
};

class MockWavDescriptor : public WavDescriptor {
public:
   MockWavDescriptor() : WavDescriptor("") {}

   MOCK_METHOD(void, add, (const string&, const string&, uint32_t, uint32_t, uint32_t, uint32_t), (override));
};

class SnippetTest : public Test {
public:
   shared_ptr<MockFileUtil> fileUtil{make_shared<MockFileUtil>()};
   shared_ptr<MockWavDescriptor> descriptor{make_shared<MockWavDescriptor>()};
   rlog::RLogChannel* channel{nullptr};

   Snippet snippet{fileUtil, descriptor, "", channel};
};

class Snippet_WriteSamples : public SnippetTest {
public:
   ostringstream out;
};

TEST_F(Snippet_WriteSamples, WritesSingleSample) {
    char data[] {"abcd"};
    uint32_t bytesPerSample {1};
    uint32_t startingSample {0};
    uint32_t samplesToWrite {1};
    snippet.writeSamples(&out, data, startingSample, samplesToWrite, bytesPerSample);
    ASSERT_EQ("a", out.str());
}

TEST_F(Snippet_WriteSamples, WritesMultibyteSampleFromMiddle) {
    char data[] {"0123456789ABCDEFG"};
    uint32_t bytesPerSample { 2 };
    uint32_t startingSample { 4 };
    uint32_t samplesToWrite { 3 };

    snippet.writeSamples(&out, data, startingSample, samplesToWrite, bytesPerSample);
    ASSERT_EQ("89ABCD", out.str());
}

TEST_F(Snippet_WriteSamples, IncorporatesChannelCount) {
    char data[] {"0123456789ABCDEFG"};
    uint32_t bytesPerSample { 2 };
    uint32_t startingSample { 0 };
    uint32_t samplesToWrite { 2 };
    uint32_t channels { 2 };

    snippet.writeSamples(
          &out, data, startingSample, samplesToWrite, bytesPerSample, channels);

    ASSERT_EQ("01234567", out.str());
}

class Snippet_DataLength : public SnippetTest {
};

TEST_F(Snippet_DataLength, IsProductOfChannels_BytesPerSample_and_Samples) {
    uint32_t bytesPerSample { 2 };
    uint32_t samples { 5 };
    uint32_t channels { 4 };
    
    uint32_t length { snippet.dataLength(bytesPerSample, samples, channels) };

    ASSERT_EQ(2 * 5 * 4, length);
}

class Snippet_Write : public SnippetTest {
public:
   istringstream input{""};
   FormatSubchunk formatSubchunk;
   ostringstream output;
   DataChunk dataChunk;
   char* data;
   uint32_t TwoBytesWorthOfBits{2 * 8};
   
   const int ArbitraryFileSize{5};

   void SetUp() override {
      data = new char[4];
   }

   void TearDown() override {
      delete[] data;
   }
};

TEST_F(Snippet_Write, SendsFileLengthAndTotalSecondsToDescriptor) {
   dataChunk.length = 8;
   formatSubchunk.bitsPerSample = TwoBytesWorthOfBits;
   formatSubchunk.samplesPerSecond = 1;

   EXPECT_CALL(*fileUtil, size)
      .WillOnce(Return(ArbitraryFileSize));

   uint32_t actual_totalSeconds;
   uint32_t actual_fileSize;
   EXPECT_CALL(*descriptor, add)
      .WillOnce(DoAll(SaveArg<2>(&actual_totalSeconds), SaveArg<5>(&actual_fileSize)));

   snippet.write("any", input, output, formatSubchunk, dataChunk, data);

   EXPECT_THAT(8 / 2 / 1, actual_totalSeconds);
   EXPECT_THAT(ArbitraryFileSize, actual_fileSize);
}
