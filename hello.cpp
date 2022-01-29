
#include <climits>
#include <float.h>
#include <iostream>
#include "kmath.h"

void showByteBinary(uint8_t *value, int count)
{
    for (int c = 0; c < count; c++)
    {
        for (int i = 7; i >= 0; --i)
        {
            int bit = (value[c] >> i) & 0x1;
            std::cout << bit;
        }
    }

    std::cout << std::endl;
}

int addsome(int a, int some)
{
    return a+some;
}

int main()
{
    std::cout << "Hello, Welcome! " << Math::add(5, 6) << std::endl;

    float number1 = 123456789.123456789;
    float number2 = 0.00012345678f;
    double number3 = 123456789.123456789;
    float number4 = 1234.123456789;
    double number5 = -123.1255;

    std::cout << number1 << std::endl;
    std::cout << number2 << std::endl;
    std::cout << number3 << std::endl;
    std::cout << number4 << std::endl;
    std::cout << number5 << std::endl;
    std::cout << sizeof(number4) << std::endl;
    std::cout << sizeof(number5) << std::endl;
    std::cout << "===============================" << std::endl;

    float a = 1.5f;
    float b = 1.3f;

    std::cout << ((a - b) == 0.2) << std::endl;
    std::cout << ((a - b) > 0.2) << std::endl;

    std::cout << "===============================" << std::endl;
    float c = 16777216;
    std::cout << ((c) == (c + 1)) << std::endl;
    float d = FLT_MAX;
    std::cout << ((d) == (d - UINT_MAX)) << std::endl;

    std::cout << "center===============================" << std::endl;

    int cell_count = 8;
    int byte_count = 2;
    int bit_count = byte_count * 8;
    std::string TEST_DATA_STR =
        "0100100010001000"
        "0000100010001000"
        "0100100000001000"
        "0000000010000000"
        "0100000000000000"
        "0000000010000000"
        "0000000000000000"
        "0100000010000000";

    uint8_t pv[cell_count][byte_count] = {};
    for (size_t i = 0; i < cell_count; i++)
    {
        uint8_t p = 0; // TODO delete
        int start = (i)*bit_count;

        for (int c = 0; c < byte_count; c++)
        {
            int b = 0;
            for (int j = start + c * 8; j < start + c * 8 + 8; ++b, ++j)
            {
                int bit2switch = ((b + 8 - 1) % 8);
                int v = TEST_DATA_STR.at(j) - '0';
                p |= (v & 0x1) << bit2switch;
            }

            pv[i][c] = p;
        }

        // std::cout << (int)pv[i] << std::endl;
    }

    for (size_t i = 0; i < cell_count; i++)
    {
        showByteBinary(pv[i], byte_count);
    }

    // add sum
    // int sum = 0;
    // int average = 0;
    // for (int i = 0; i < cell_count; i++)
    // {
    //     sum += pv[i];
    // }
    // average = sum / cell_count;
    // std::cout << "sum and average: " << sum << " " << average << std::endl;
    // showByteBinary(average);

    // int test = (8 & 8) + ((8 ^ 8) >> 1);
    // std::cout << test << std::endl;

    int sumBits[bit_count] = {};
    for (int i = 0; i < cell_count; i++)
    {
        for (int c = 0; c < byte_count; c++)
        {
            uint8_t v = pv[i][c];
            int start = c * 8;
            sumBits[start + 0] += (v >> 0) & 0x1;
            sumBits[start + 1] += (v >> 1) & 0x1;
            sumBits[start + 2] += (v >> 2) & 0x1;
            sumBits[start + 3] += (v >> 3) & 0x1;
            sumBits[start + 4] += (v >> 4) & 0x1;
            sumBits[start + 5] += (v >> 5) & 0x1;
            sumBits[start + 6] += (v >> 6) & 0x1;
            sumBits[start + 7] += (v >> 7) & 0x1;
        }
    }

    int half = cell_count / 2;
    uint8_t center[byte_count] = {};
    for (int i = 0; i < bit_count; i++)
    {
        sumBits[i] = sumBits[i] > half ? 1 : 0;
        int c = i/8;
        int b = i%8;
        center[c] |= sumBits[i]<<b;
    }

    std::cout << "center is:" << std::endl;
    showByteBinary(center, byte_count);


    int vint = 1;
    if(vint=addsome(vint,100); vint--, vint)
    {
        std::cout << vint << std::endl;
    }

    return 0;
}