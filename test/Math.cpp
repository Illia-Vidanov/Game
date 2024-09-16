#include "Math/Math.hpp"

#include "TestSetup.hpp"

TEST(DigitCountTest, NmberDigitsReturn)
{
    EXPECT_EQ(DigitCount(static_cast<char>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<char>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<char>(10)), 2);
    EXPECT_EQ(DigitCount(static_cast<char>(100)), 3);
    EXPECT_EQ(DigitCount(static_cast<unsigned char>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<unsigned char>(10)), 2);
    EXPECT_EQ(DigitCount(static_cast<unsigned char>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<unsigned char>(100)), 3);

    EXPECT_EQ(DigitCount(static_cast<short>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<short>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<short>(999)), 3);
    EXPECT_EQ(DigitCount(static_cast<short>(9999)), 4);
    EXPECT_EQ(DigitCount(static_cast<short>(std::numeric_limits<short>::max())), 5);

    EXPECT_EQ(DigitCount(static_cast<unsigned short>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<unsigned short>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<unsigned short>(999)), 3);
    EXPECT_EQ(DigitCount(static_cast<unsigned short>(9999)), 4);
    EXPECT_EQ(DigitCount(static_cast<unsigned short>(std::numeric_limits<unsigned short>::max())), 5);
    
    EXPECT_EQ(DigitCount(static_cast<int>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<int>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<int>(999)), 3);
    EXPECT_EQ(DigitCount(static_cast<int>(9999)), 4);
    EXPECT_EQ(DigitCount(static_cast<int>(99999)), 5);
    EXPECT_EQ(DigitCount(static_cast<int>(999999)), 6);
    EXPECT_EQ(DigitCount(static_cast<int>(9999999)), 7);
    EXPECT_EQ(DigitCount(static_cast<int>(99999999)), 8);
    EXPECT_EQ(DigitCount(static_cast<int>(999999999)), 9);
    EXPECT_EQ(DigitCount(static_cast<int>(std::numeric_limits<int>::max())), 10);

    EXPECT_EQ(DigitCount(static_cast<unsigned int>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(999)), 3);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(9999)), 4);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(99999)), 5);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(999999)), 6);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(9999999)), 7);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(99999999)), 8);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(999999999)), 9);
    EXPECT_EQ(DigitCount(static_cast<unsigned int>(std::numeric_limits<unsigned int>::max())), 10);
    
    EXPECT_EQ(DigitCount(static_cast<long long>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<long long>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<long long>(999)), 3);
    EXPECT_EQ(DigitCount(static_cast<long long>(9999)), 4);
    EXPECT_EQ(DigitCount(static_cast<long long>(99999)), 5);
    EXPECT_EQ(DigitCount(static_cast<long long>(999999)), 6);
    EXPECT_EQ(DigitCount(static_cast<long long>(9999999)), 7);
    EXPECT_EQ(DigitCount(static_cast<long long>(99999999)), 8);
    EXPECT_EQ(DigitCount(static_cast<long long>(999999999)), 9);
    EXPECT_EQ(DigitCount(static_cast<long long>(9999999999)), 10);
    EXPECT_EQ(DigitCount(static_cast<long long>(99999999999)), 11);
    EXPECT_EQ(DigitCount(static_cast<long long>(999999999999)), 12);
    EXPECT_EQ(DigitCount(static_cast<long long>(9999999999999)), 13);
    EXPECT_EQ(DigitCount(static_cast<long long>(99999999999999)), 14);
    EXPECT_EQ(DigitCount(static_cast<long long>(999999999999999)), 15);
    EXPECT_EQ(DigitCount(static_cast<long long>(9999999999999999)), 16);
    EXPECT_EQ(DigitCount(static_cast<long long>(99999999999999999)), 17);
    EXPECT_EQ(DigitCount(static_cast<long long>(999999999999999999)), 18);
    EXPECT_EQ(DigitCount(static_cast<long long>(std::numeric_limits<long long>::max())), 19);

    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9)), 1);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(99)), 2);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(999)), 3);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9999)), 4);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(99999)), 5);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(999999)), 6);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9999999)), 7);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(99999999)), 8);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(999999999)), 9);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9999999999)), 10);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(99999999999)), 11);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(999999999999)), 12);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9999999999999)), 13);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(99999999999999)), 14);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(999999999999999)), 15);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9999999999999999)), 16);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(99999999999999999)), 17);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(999999999999999999)), 18);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(9999999999999999999u)), 19);
    EXPECT_EQ(DigitCount(static_cast<unsigned long long>(std::numeric_limits<unsigned long long>::max())), 20);
}

TEST(DigitCountTest, DigitCountIgnoreMinus)
{
    EXPECT_EQ(DigitCount(static_cast<int>(-9)), 1);
}

TEST(DigitCountTest, DigitCountZeroEqZero)
{
    EXPECT_EQ(DigitCount(0), 0);
}

TEST(GetDigitTest, GetDigitReturn)
{
    EXPECT_EQ(GetDigit(9876543210, 0), 0);
    EXPECT_EQ(GetDigit(987654321, 0), 1);
    EXPECT_EQ(GetDigit(9876543210, 9), 9);
    EXPECT_EQ(GetDigit(9876543210, 10), 0);
    EXPECT_EQ(GetDigit(9876543210, 100), 0);
    EXPECT_EQ(GetDigit(9876543210, -1), 0);
    EXPECT_EQ(GetDigit(9876543210, -100), 0);
}

TEST(GetDigitTest, GetDigitOverflow)
{
    EXPECT_EQ(GetDigit(9876543210, 10), 0);
    EXPECT_EQ(GetDigit(9876543210, 100), 0);
}

TEST(GetDigitTest, GetDigitNegative)
{
    EXPECT_EQ(GetDigit(-9876543210, 0), 0);
    EXPECT_EQ(GetDigit(-987654321, 0), 1);
    EXPECT_EQ(GetDigit(-9876543210, 9), 9);
    EXPECT_EQ(GetDigit(9876543210, -1), 0);
    EXPECT_EQ(GetDigit(9876543210, -100), 0);
    EXPECT_EQ(GetDigit(-9876543210, -1), 0);
    EXPECT_EQ(GetDigit(-9876543210, -100), 0);
}


TEST(FastPowerTest, FastPowerReturn)
{
    EXPECT_EQ(FastPower10(0), 1);
    EXPECT_EQ(FastPower10(1), 10);
    EXPECT_EQ(FastPower10(9), 1000000000);
    EXPECT_EQ(FastPower10<unsigned long long>(19), 10000000000000000000ULL);
    EXPECT_EQ(FastPower10(-0), 1);
    EXPECT_FLOAT_EQ(FastPower10<float>(-1), 0.1);
    EXPECT_FLOAT_EQ(FastPower10<float>(-19), 0.0000000000000000001);
}

TEST(FastPowerTest, FastPowerOverflow)
{
    EXPECT_EQ(FastPower10<unsigned long long>(20), 1);
    EXPECT_FLOAT_EQ(FastPower10<double>(-21), 1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}