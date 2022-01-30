#include <iostream>
#include <string.h>
#include <time.h>

#include "kmath.h"

const int WALL_PERCENT = 10;

const int MAX_STEP = 4;
const int X_RANGE = 10;
const int Y_RANGE = 10;

const char AIR = '0';
const char WALL = '1';
const char BLOCK = 'A';

unsigned char TEST_SPACE[Y_RANGE][X_RANGE] = {{WALL, AIR, AIR, WALL}, {AIR, AIR, AIR, WALL}, {AIR, AIR, WALL, AIR}, {AIR, WALL, WALL, AIR}};

unsigned char SPACE[Y_RANGE][X_RANGE];

bool IsPercentUnder(int percent)
{
    int rand_num = rand() % 10000;
    return rand_num < (percent * 100);
}

void ConstructSpace()
{
    for (int y = 0; y < Y_RANGE; y++)
    {
        for (int x = 0; x < X_RANGE; x++)
        {
            // for test
            // SPACE[y][x] = TEST_SPACE[y][x];
            // continue;
            SPACE[y][x] = IsPercentUnder(WALL_PERCENT) ? WALL : AIR;
        }
    }
}

int SearchToX(int startX, int startY)
{
    for (int x = startX; x < X_RANGE; x++)
    {
        if (SPACE[startY][x] != AIR)
        {
            return x;
        }
    }
    return X_RANGE;
}

bool checkStartXY(int *startX, int *startY)
{
    while (*startY < Y_RANGE)
    {
        while (*startX < X_RANGE)
        {
            if (SPACE[*startY][*startX] == AIR)
            {
                return true;
            }
            else
            {
                (*startX)++;
            }
        }
        (*startY)++;
        *startX = 0; // reset to 0
    }

    return false;
}

void BuildBlocks()
{
    int datas[Y_RANGE][X_RANGE];
    memset(datas, 0, sizeof(datas));

    int toX = 0, startX = 0, startY = 0;
    int curBlockValue = 0;
    int minTall = 0;
    while (checkStartXY(&startX, &startY))
    {
        toX = SearchToX(startX, startY);
        // step 1, get depth air
        for (int x = startX; x < toX; x++)
        {
            for (int y = startY; y < Y_RANGE; y++)
            {
                if (SPACE[y][x] == AIR)
                {
                    // bufferSpace[y][x] = curBlockValue;
                    datas[startY][x]++; // NOTE: startY is fixed here
                }
                else
                {
                    // if not air, then break;
                    break;
                }
            }
        }

        // step 2, get minimum tall
        int minTall = datas[startY][startX];
        for (int x = startX; x < toX; x++)
        {
            if (datas[startY][x] == 1)
            {
                minTall = 1;
                break;
            }

            if (datas[startY][x] < minTall)
            {
                minTall = datas[startY][x];
            }
        }

        // step 3, fill block value
        for (int x = startX; x < toX; x++) // NOTE, <toX
        {
            for (int y = startY; y < startY + minTall; y++) // NOTE: <startY+minTall
            {
                SPACE[y][x] = BLOCK + curBlockValue;
            }
        }
        // step3, ++ curBlockValue
        ++curBlockValue;
    }

    std::cout << "Build Blocks Done." << std::endl;
}

void ShowSpace()
{
    std::cout << "======================" << std::endl;
    for (int y = 0; y < Y_RANGE; y++)
    {
        for (int x = 0; x < X_RANGE; x++)
        {
            std::cout << ' ' << SPACE[y][x];
        }
        std::cout << std::endl;
    }
    std::cout << "======================" << std::endl;
}

// int main(int argc, char **argv)
int main()
{
    std::cout << "Hello Space " << std::endl << std::endl;

    unsigned int seed = (unsigned)time(NULL);
    srand(seed); // random seed

    ConstructSpace();

    std::cout << "Before Build Blocks:" << std::endl;
    ShowSpace();

    BuildBlocks();

    ShowSpace();

    std::cout << Math::add(153, 200) << std::endl;
    return 0;
}