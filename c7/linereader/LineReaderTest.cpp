#include "gtest/gtest.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "LineReader.h"

static int TemporaryFile() {
	static const char templ[] = "/tmp/line-reader-unittest-XXXXXXX";
	char templ_copy[sizeof(templ)];
	memcpy(templ_copy, templ, sizeof(templ));
	const int fd = mkstemp(templ_copy);
	if (fd >= 0)
		unlink(templ_copy);

	return fd;
}

static int TemporaryFile(const char* record) {
	const int fd = TemporaryFile();
	write(fd, record, strlen(record));
	lseek(fd, 0, SEEK_SET);
	return fd;
}

class LineReaderTest : public testing::Test {
public:
	int fd;
	const char *line;
	unsigned len;
};

TEST_F(LineReaderTest, EmptyFile) {
	fd = TemporaryFile("");
	LineReader reader(fd);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

void ASSERT_EQ_WITH_LENGTH(
		const char* expected, const char* actual, unsigned length) {
	ASSERT_EQ(length, strlen(actual));
	ASSERT_STREQ(expected, actual);
}

TEST_F(LineReaderTest, OneLineTeminated) {
	fd = TemporaryFile("a\n");
	LineReader reader(fd);

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ_WITH_LENGTH("a", line, len);
	reader.PopLine(len);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, OneLine) {
	LineReader reader(TemporaryFile("a"));

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'a');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, TwoLineTeminated) {
	fd = TemporaryFile("a\nb\n");
	LineReader reader(fd);

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'a');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'b');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, TwoLine) {
	fd = TemporaryFile("a\nb");
	LineReader reader(fd);

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'a');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'b');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));
}

TEST_F(LineReaderTest, MaxLength) {
	char l[LineReader::kMaxLineLen - 1];
	memset(l, 'a', sizeof(l));
	fd = TemporaryFile(l);
	LineReader reader(fd);

	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, sizeof(l));
	ASSERT_TRUE(memcmp(l, line, sizeof(l)) == 0);
	ASSERT_EQ(line[len], 0);
}

TEST_F(LineReaderTest, TooLong) {
	char l[LineReader::kMaxLineLen];
	memset(l, 'a', sizeof(l));
	fd = TemporaryFile(l);
	LineReader reader(fd);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));
}
