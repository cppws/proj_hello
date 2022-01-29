#pragma once

#include <string>


const bool DBG_PRINT = false;

const double DIST_NEAR_ZERO = 0.00001;
const int DIST_ZERO = 0;

const int BIT_STEP = 8; // uint8 bits

const int TEST_POINT_RANDOM_COUNT = 100;
const int TEST_BYTE_COUNT_PER_CELL = 12;

const int BIT_COUNT = TEST_BYTE_COUNT_PER_CELL * BIT_STEP; // how many dimentions 3

const int TEST_POINT_COUNT = 25;
std::string TEST_POINT_STR =
    "000100000000000000010000000000000000100000000000000000010000000000000000010000000000000000100000"
    "111110111111111111111111101111111111111111111111111111111111111111110111111111111111111111111111"
    "100000100000000100000000010000000000000000100000000000000000010000000000000000001000000000000000"
    "000000000100000000100000000000000000000000001111111111111110111111111110111111111111111111110111"
    "111111111110111011111111111111111111111111111111111100111111111111111111111111111101111111111111"
    "000000000000000000001000000000000000100000000000000010000000000010000000000000000000000000000000"
    "111110111111111111111101101111111111111111111111111111111111111111111111011111111111111111111111"
    "000000001000000000000001000000000000000000000100000000000000000000000000001000000000000000000000"
    "111111011111111111111111111111111111011111111111111111111111111111111111011111111111111111111111"
    "000010000000000100000000000000000001000000000000000000010000000000000000000010000000000000100000"
    "111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111"
    "000011000000000000000000010000000000000010000111111111111111111111111111111111111111111111111111"
    "000011000000000000000000000010000000000000000100000000000000000000001000000000000000000000000100"
    "111111111111101111111111111111111111111111111111111101111111111111111111100111111111111111111111"
    "000000000001100000000000000000000000000011100000000000000000000000000000000011100000000000000000"
    "111110111111111111111111111111101111111111111111111111111111111011111111111111111111111111111111"
    "000000000100000000100000000000000001010000000111111111110111111111111111111111011111111111111111"
    "000001100000000000000000001001000000000000001000000000000100000000000000000000011000000000000001"
    "111111111111111110110111111111111111111111110111111111111111101111101111111111111111111111101111"
    "000000010000000001000000000000000000000000010000000000000000000000000000100000000000000010000000"
    "111101111111111111111111011111111111111111110111111111111111111111111111111111111011111111111111"
    "000000000100000000100000000000001000000000000000010000000000000000000000001000000000000000000000"
    "000100000000011000000000001000000000000000000111111111111110111111111111011111111111111111111111"
    "111111110111111111111111011111111111111111110111111111111111111111111011111111111111111111110111" 
    "100000001100000000000000101010000000000000100111111111101111111111111110111111111111111111110111";


// Test data from 0000 ~ 1111
const uint8_t Test_Points[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
// 0: 0,0,0,0
// 1: 0,0,0,1
// 6: 0,1,1,0
// 7: 0,1,1,1
// 2: 0,0,1,0
// 3: 0,0,1,1