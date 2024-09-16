#include "Utils/FlagParser.hpp"

#include "TestSetup.hpp"

TEST(FlagParser, FlagParserValueCheck)
{
    int argc = 9;
    const char **argv = new const char*[argc];
    argv[0] = "-1=123";
    argv[1] = "-2=\"123\"";
    argv[2] = "-3=\"\\\"123\\\"\"";
    argv[3] = "-4=\"123";
    argv[4] = "456\"";
    argv[5] = "-5=\"\\\"";
    argv[6] = "\"";
    argv[7] = "-6=\"";
    argv[8] = "\"";
    Flags flags(argc, argv);

    EXPECT_EQ(flags.Get("-1"), "123");
    EXPECT_EQ(flags.Get("-2"), "123");
    EXPECT_EQ(flags.Get("-3"), "\"123\"");
    EXPECT_EQ(flags.Get("-4"), "123 456");
    EXPECT_EQ(flags.Get("-5"), "\" ");
    EXPECT_EQ(flags.Get("-6"), " ");
}

TEST(FlagParser, FlagParserErrorCheck)
{
    int argc = 4;
    const char **argv = new const char*[argc];
    argv[0] = "-1=";
    argv[1] = "-2=\"\"\"";
    argv[2] = "-3=\"\"456";
    argv[3] = "-4=\"";
    Flags flags(argc, argv);

    EXPECT_EQ(flags.Get("-1"), "");
    EXPECT_EQ(flags.Get("-2"), "");
    EXPECT_EQ(flags.Get("-3"), "");
    EXPECT_EQ(flags.Get("-4"), "");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}