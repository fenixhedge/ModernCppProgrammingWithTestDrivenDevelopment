#include "gtest/gtest.h"
#include "WavReader.h"
#include <iostream>
#include <string>

using namespace std;

class WavReaderTest : public testing::Test {
};

TEST(WavReaderTest, HasExtensionTest) {
   string s{"a.bcd"};
   ASSERT_FALSE(hasExtension(s, "xxxx"));
   ASSERT_TRUE(hasExtension(s, "bcd"));
   string bigger{"aaabcd"};
   ASSERT_TRUE(!hasExtension(s, bigger));
}

class WavReader_WriteSamples : public testing::Test {
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

class WavReader_DataLength : public testing::Test {
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
