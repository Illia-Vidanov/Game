#include "Utils/String.hpp"

#include "TestSetup.hpp"

TEST(SplitStringTest, SplitStringResult)
{
    EXPECT_EQ(SplitString(std::string("12  34 56"), std::string(" "))[0], "12");
    EXPECT_EQ(SplitString(std::string("12  34 56"), std::string(" "))[1], "34");
    EXPECT_EQ(SplitString(std::string("  12 34 56"), std::string(" "))[0], "12");

    EXPECT_EQ(SplitString(std::string("12 34  56"), std::string(" "))[2], "56");
    EXPECT_EQ(SplitString(std::string("12 34 56  "), std::string(" "))[2], "56");

    EXPECT_EQ(SplitString(std::string("12 34 56"), std::string(" ")).size(), 3);
    EXPECT_EQ(SplitString(std::string("12 34 56  "), std::string(" ")).size(), 3);
    EXPECT_EQ(SplitString(std::string("  12 34 56  "), std::string(" ")).size(), 3);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}