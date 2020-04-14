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

namespace {
typedef testing::Test LineReaderTest;
}

TEST(LineReaderTest, EmptyFile) {
	const int fd = TemporaryFile("");
	LineReader reader(fd);

	const char *line;
	unsigned len;
	ASSERT_FALSE(reader.GetNextLine(&line, &len));

	close(fd);
}

TEST(LineReaderTest, OneLineTeminated) {
	const int fd = TemporaryFile("a\n");
	LineReader reader(fd);

	const char *line;
	unsigned len;
	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'a');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));

	close(fd);
}

TEST(LineReaderTest, OneLine) {
	const int fd = TemporaryFile("a");
	LineReader reader(fd);

	const char *line;
	unsigned len;
	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, (unsigned)1);
	ASSERT_EQ(line[0], 'a');
	ASSERT_EQ(line[1], 0);
	reader.PopLine(len);

	ASSERT_FALSE(reader.GetNextLine(&line, &len));

	close(fd);
}

TEST(LineReaderTest, TwoLineTeminated) {
	const int fd = TemporaryFile("a\nb\n");
	LineReader reader(fd);

	const char *line;
	unsigned len;
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

	close(fd);
}

TEST(LineReaderTest, TwoLine) {
	const int fd = TemporaryFile("a\nb");
	LineReader reader(fd);

	const char *line;
	unsigned len;
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

	close(fd);
}

TEST(LineReaderTest, MaxLength) {
	char l[LineReader::kMaxLineLen - 1];
	memset(l, 'a', sizeof(l));
	const int fd = TemporaryFile(l);
	LineReader reader(fd);

	const char *line;
	unsigned len;
	ASSERT_TRUE(reader.GetNextLine(&line, &len));
	ASSERT_EQ(len, sizeof(l));
	ASSERT_TRUE(memcmp(l, line, sizeof(l)) == 0);
	ASSERT_EQ(line[len], 0);

	close(fd);
}

TEST(LineReaderTest, TooLong) {
	char l[LineReader::kMaxLineLen];
	memset(l, 'a', sizeof(l));
	const int fd = TemporaryFile(l);
	LineReader reader(fd);

	const char *line;
	unsigned len;
	ASSERT_FALSE(reader.GetNextLine(&line, &len));

	close(fd);
}
