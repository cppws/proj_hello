
#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <string>
#include <time.h>
#include <random>

#include "kutils.h"
#include "kmeans.h"

// const int TEST_POINT_RANDOM_COUNT = 100;
// const int TEST_POINT_COUNT = 25;
// const int TEST_BYTE_COUNT_PER_CELL = 12;

// // std::string TEST_POINT_STR =
// //     "000100000000000000010000000000000000100000000000000000010000000000000000010000000000000000100000"
// //     "111110111111111111111111101111111111111111111111111111111111111111110111111111111111111111111111"
// //     "100000100000000100000000010000000000000000100000000000000000010000000000000000001000000000000000"
// //     "000000000100000000100000000000000000000000001111111111111110111111111110111111111111111111110111"
// //     "111111111110111011111111111111111111111111111111111100111111111111111111111111111101111111111111"
// //     "000000000000000000001000000000000000100000000000000010000000000010000000000000000000000000000000"
// //     "111110111111111111111101101111111111111111111111111111111111111111111111011111111111111111111111"
// //     "000000001000000000000001000000000000000000000100000000000000000000000000001000000000000000000000"
// //     "111111011111111111111111111111111111011111111111111111111111111111111111011111111111111111111111"
// //     "000010000000000100000000000000000001000000000000000000010000000000000000000010000000000000100000"
// //     "111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111"
// //     "000011000000000000000000010000000000000010000111111111111111111111111111111111111111111111111111"
// //     "000011000000000000000000000010000000000000000100000000000000000000001000000000000000000000000100"
// //     "111111111111101111111111111111111111111111111111111101111111111111111111100111111111111111111111"
// //     "000000000001100000000000000000000000000011100000000000000000000000000000000011100000000000000000"
// //     "111110111111111111111111111111101111111111111111111111111111111011111111111111111111111111111111"
// //     "000000000100000000100000000000000001010000000111111111110111111111111111111111011111111111111111"
// //     "000001100000000000000000001001000000000000001000000000000100000000000000000000011000000000000001"
// //     "111111111111111110110111111111111111111111110111111111111111101111101111111111111111111111101111"
// //     "000000010000000001000000000000000000000000010000000000000000000000000000100000000000000010000000"
// //     "111101111111111111111111011111111111111111110111111111111111111111111111111111111011111111111111"
// //     "000000000100000000100000000000001000000000000000010000000000000000000000001000000000000000000000"
// //     "000100000000011000000000001000000000000000000111111111111110111111111111011111111111111111111111"
// //     "111111110111111111111111011111111111111111110111111111111111111111111011111111111111111111110111" //////////////////
// //     "100000001100000000000000101010000000000000100111111111101111111111111110111111111111111111110111";

// const bool DBG_PRINT = false;

// const double DIST_NEAR_ZERO = 0.00001;

// const int BIT_COUNT = 96; // how many dimentions 3
// const int BIT_STEP = 8;   // uint8 bits

class KMeans
{
public:
    typedef struct Point
    {
        uint32_t value_count;
        uint32_t groupID;
        uint32_t cellID;

        uint8_t *values;

        // must write this constructer if you have ones with params
        Point()
        {
        }

        Point(uint8_t *v, uint32_t c, uint32_t id) : values(v), value_count(c), groupID(0), cellID(id)
        {
        }

    } Point;

    typedef struct DimPoint // point in dimension
    {
        double dimension[BIT_COUNT];
        DimPoint()
        {
            for (size_t i = 0; i < BIT_COUNT; i++)
            {
                dimension[i] = 0.0;
            }
        }

        void SetDimValues(double dim[BIT_COUNT])
        {
            for (size_t i = 0; i < BIT_COUNT; i++)
            {
                dimension[i] = dim[i];
            }
        }

    } DimPoint;

private:
    int K;
    int _MovedTimes;

public:
    KMeans(/* args */);
    ~KMeans();

    typedef std::vector<Point> Points;
    typedef std::vector<DimPoint> DimPoints;
    Points originalPoints;
    DimPoints centerPoints;

    std::vector<Points> groupedPoints;

    inline void SetK(int k)
    {
        K = k;
        groupedPoints.resize(K);
        _MovedTimes = 0;
    }

    bool AddOriginalPoint(Point point);
    bool SetOriginalPoints(std::vector<Point> &points);
    bool InitKCenter();

    bool Cluster();

    DimPoints UpdateGroupCenter(std::vector<Points> &grpPoints, DimPoints centers);

    double DistanceBetweenPoints(Point &point1, DimPoint &dp2);
    double DistanceBetweenPoints(DimPoint &dp1, DimPoint &dp2);

    bool IsCenterShiftExist(DimPoints &prevCenters, DimPoints &curCenters);

    DimPoint Point2Dim(Point &point);
    // Point Dim2Point(DimPoint &dimpoint);

    void showPoint(Point &point);
    void showOriginalPoints();
    void showGroupedPoints();

    int GetByteCount()
    {
        return (BIT_COUNT % BIT_STEP == 0) ? (BIT_COUNT / BIT_STEP) : ((BIT_COUNT / BIT_STEP) + 1);
    }
};

KMeans ::KMeans(/* args */)
{
    originalPoints.clear();
    centerPoints.clear();
    groupedPoints.clear();
    K = 0;
}

KMeans ::~KMeans()
{
}

KMeans::DimPoint KMeans::Point2Dim(Point &point)
{
    DimPoint dimpoint;
    for (int i = 0; i < BIT_COUNT; i++)
    {
        int index = (i / BIT_STEP);
        int bit2switch = (i % BIT_STEP);
        dimpoint.dimension[i] = (point.values[index] >> bit2switch) & 0x1;
    }
    return dimpoint;
}

// Point KMeans::Dim2Point(DimPoint &dimpoint)
// {
//     Point point;
//     int bytecount = GetByteCount();
//     uint8_t *values = (uint8_t *)malloc(bytecount);//TODO delete
//     //TODO memset
//     for (size_t i = 0; i < bytecount; i++)
//     {
//         values[i] = 0;
//     }

//     for (size_t i = 0; i < BIT_COUNT; i++)
//     {
//         int index = (i / BIT_STEP);
//         int bit2switch = (i % BIT_STEP);
//         int dimInt = std::round(dimpoint.dimension[i]); // round convert double to int, not casting
//         values[index] |= (dimInt & 0x1) << bit2switch;
//     }

//     point.value_count = bytecount;
//     point.values = values;

//     return point;
// }

double KMeans::DistanceBetweenPoints(Point &point1, DimPoint &dp2)
{
    // construct dimention values array for 2 points
    double p1[BIT_COUNT];
    // double p2[BIT_COUNT];

    double diff = 0.0;
    double diffSum = 0.0;
    double dist = 0.0;

    for (int i = 0; i < BIT_COUNT; i++)
    {
        int index = (i / BIT_STEP);
        int bit2switch = (i % BIT_STEP);
        p1[i] = (point1.values[index] >> bit2switch) & 0x1;
        // p2[i] = (point2.values[index] >> bit2switch) & 0x1;

        diff = p1[i] - dp2.dimension[i];
        diffSum += diff * diff;
    }

    if (false)
    {
        // show 2 points' values in dimentions
        std::cout << "P1 in " << BIT_COUNT << " dimentions:" << std::endl;
        for (int i = 0; i < BIT_COUNT; i++)
        {
            std::cout << p1[i] << " ";
        }
        std::cout << std::endl;

        // std::cout << "P2 in " << BIT_COUNT << " dimentions:" << std::endl;
        // for (int i = 0; i < BIT_COUNT; i++)
        // {
        //     std::cout << p2[i] << " ";
        // }
        // std::cout << std::endl;
    }

    dist = std::sqrt(diffSum);

    return dist;
}

double KMeans::DistanceBetweenPoints(DimPoint &dp1, DimPoint &dp2)
{
    double diff = 0.0;
    double diffSum = 0.0;
    double dist = 0.0;

    for (int i = 0; i < BIT_COUNT; i++)
    {
        diff = dp1.dimension[i] - dp2.dimension[i];
        diffSum += diff * diff;
    }

    dist = std::sqrt(diffSum);

    return dist;
}

bool KMeans::IsCenterShiftExist(DimPoints &prevCenters, DimPoints &curCenters)
{
    for (int i = 0; i < K; i++)
    {
        double dist = DistanceBetweenPoints(prevCenters[i], curCenters[i]);
        if (dist > DIST_NEAR_ZERO)
        {
            return true;
        }
    }
    return false;
}

KMeans::DimPoints KMeans::UpdateGroupCenter(std::vector<Points> &grpPoints, DimPoints centers)
{
    double bitvalues[BIT_COUNT];
    for (size_t i = 0; i < K; i++)
    {
        for (int index = 0; index < BIT_COUNT; ++index)
        {
            bitvalues[index] = 0.0;
        }

        int grpSize = grpPoints[i].size();

        for (size_t j = 0; j < grpSize; j++)
        {
            DimPoint dimpoint = Point2Dim(grpPoints[i][j]);

            for (int index = 0; index < BIT_COUNT; ++index)
            {
                bitvalues[index] += dimpoint.dimension[index];
            }
        }

        for (int index = 0; index < BIT_COUNT; ++index)
        {
            bitvalues[index] /= grpSize;
        }

        centers[i].SetDimValues(bitvalues);
    }

    return centers;
}

bool KMeans::Cluster()
{
    InitKCenter();

    DimPoints centers(centerPoints.size()); // K

    int pointCount = originalPoints.size();

    do
    {
        for (size_t i = 0; i < pointCount; i++)
        {
            double min_dist = DBL_MAX;
            int grp = 0;
            for (size_t j = 0; j < K; j++)
            {
                // Point center = Dim2Point(centerPoints[j]);//TODO cellID is garbage
                double dist = DistanceBetweenPoints(originalPoints[i], centerPoints[j]);

                if (min_dist - dist > DIST_NEAR_ZERO)
                {
                    min_dist = dist;
                    grp = j;
                }
            }
            originalPoints[i].groupID = grp;
            groupedPoints[grp].push_back(originalPoints[i]);
        }

        // bak last centers
        for (size_t i = 0; i < K; i++)
        {
            centers[i] = centerPoints[i];
        }

        centerPoints = UpdateGroupCenter(groupedPoints, centerPoints);

        if (!IsCenterShiftExist(centers, centerPoints))
        {
            break;
        }

        _MovedTimes++;

        for (size_t i = 0; i < K; i++)
        {
            groupedPoints[i].clear();
        }

    } while (true);

    return true;
}

bool INIT_CENTER_RANDOM = true;

bool KMeans::InitKCenter()
{
    if (K > 0)
    {

        centerPoints.resize(K);
        int size = originalPoints.size();

        // pure random:
        if (INIT_CENTER_RANDOM)
        {
            srand(unsigned(time(NULL)));
            std::vector<int> usedseed;

            for (int i = 0; i < K; i++)
            {
                int seed = rand() % (size);
                while (ContainInt(usedseed, seed))
                {
                    seed = rand() % (size);
                }
                usedseed.push_back(seed);
                centerPoints[i] = Point2Dim(originalPoints[seed]);
            }
        }
        else
        {
            int step = size / (K + 1); // K+1, 1 more is for center the centerpoints
            for (int i = 0; i < K; i++)
            {
                int seed = (i + 1) * step;
                centerPoints[i] = Point2Dim(originalPoints[seed]);
            }
        }

        // equal assigment

        return true;
    }

    return false;
}

bool KMeans::AddOriginalPoint(Point point)
{
    originalPoints.push_back(point);

    double p1[BIT_COUNT];
    for (int i = 0; i < BIT_COUNT; i++)
    {
        int index = (i / BIT_STEP);
        int bit2switch = (i % BIT_STEP);
        p1[i] = (point.values[index] >> bit2switch) & 0x1;
    }

    if (DBG_PRINT)
    {
        int c = GetByteCount();
        std::cout << "Point in " << c << " bytes:" << std::endl;
        for (int i = 0; i < c; i++)
        {
            std::cout << (int)point.values[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Point in " << BIT_COUNT << " dimentions:" << std::endl;
        for (int i = 0; i < BIT_COUNT; i++)
        {
            std::cout << p1[i];
        }
        std::cout << std::endl;
    }

    return true;
}

bool KMeans::SetOriginalPoints(Points &points)
{
    if (points.size() > 0)
    {
        originalPoints = points;
        return true;
    }
    return false;
}

KMeans loadRamdomPoint()
{
    KMeans kmeans;
    // generate random 0 1 values
    srand(unsigned(time(NULL)));
    int *values = new int[TEST_POINT_RANDOM_COUNT * TEST_BYTE_COUNT_PER_CELL * BIT_STEP]{};
    for (size_t i = 0; i < (TEST_POINT_RANDOM_COUNT * TEST_BYTE_COUNT_PER_CELL * BIT_STEP); i++)
    {
        int seed = rand() % 2;
        values[i] = seed;
    }

    int value_count = kmeans.GetByteCount();
    for (size_t i = 0; i < TEST_POINT_RANDOM_COUNT; i++)
    {
        uint8_t *pv = new uint8_t[value_count]{}; // TODO delete

        int start = i * BIT_COUNT;
        int index = 0;

        for (int j = start; j < start + BIT_COUNT; j++)
        {
            int index = ((j - start) / BIT_STEP);
            int bit2switch = (j % BIT_STEP);
            int v = values[j];
            pv[index] |= (v & 0x1) << bit2switch;
        }

        KMeans::Point p = KMeans::Point(pv, value_count, i);
        kmeans.AddOriginalPoint(p);
    }

    return kmeans;
}

KMeans loadStringPoint()
{
    KMeans kmeans;

    // int value_count = GetByteCount();

    // for (size_t i = 0; i < TEST_POINT_COUNT; i++)
    // {
    //     uint8_t *pv = new uint8_t[value_count]; // TODO delete

    //     int start = i * BIT_COUNT;
    //     int index = 0;

    //     for (int j = start; j < start + BIT_COUNT; j++)
    //     {
    //         int index = ((j - start) / BIT_STEP);
    //         int bit2switch = (j % BIT_STEP);
    //         // int v = std::stoi(TEST_POINT_STR.substr(j, j + 1));
    //         int v = TEST_POINT_STR.at(j) - '0';
    //         pv[index] |= (v & 0x1) << bit2switch;
    //     }

    //     Point p = Point(pv, value_count, i);
    //     kmeans.AddOriginalPoint(p);
    // }

    return kmeans;
}

void KMeans::showPoint(Point &point)
{
    if (DBG_PRINT)
    {
        int c = GetByteCount();
        std::cout << "Point in " << c << " bytes=====:" << std::endl;
        for (int i = 0; i < c; i++)
        {
            std::cout << (int)point.values[i] << " ";
        }
        std::cout << std::endl;
    }

    double p1[BIT_COUNT];
    for (int i = 0; i < BIT_COUNT; i++)
    {
        int index = (i / BIT_STEP);
        int bit2switch = (i % BIT_STEP);
        p1[i] = (point.values[index] >> bit2switch) & 0x1;
    }
    // std::cout << "Point in " << BIT_COUNT << " dimentions:" << std::endl;
    for (int i = 0; i < BIT_COUNT; i++)
    {
        std::cout << p1[i];
    }
    std::cout << std::endl;
}

void KMeans::showOriginalPoints()
{
    std::cout << "Original Points Are:" << std::endl;
    for (Point p : originalPoints)
    {
        showPoint(p);
    }
}

void KMeans::showGroupedPoints()
{
    std::cout << "Clustered Grouped Points Are:" << std::endl;
    int i = 0;
    for (Points points : groupedPoints)
    {
        std::cout << "Group ID: " << i++ << std::endl;
        for (Point p : points)
        {
            showPoint(p);
        }
    }
}

//////////////////////////////////////
int main()
{
    std::cout << "K-means started." << std::endl;

    KMeans kmeans;

    // kmeans = loadStringPoint();

    kmeans = loadRamdomPoint();

    // int test_point_count = 4;
    // int test_points[] = {0,6,1,7};
    // int test_point_count = 6;
    // int test_points[] = {0, 1, 6, 7, 2, 3};

    // for (size_t i = 0; i < test_point_count; i++)
    // {
    //     uint8_t *pv = new uint8_t[GetByteCount()]; // TODO delete
    //     pv[0] = Test_Points[test_points[i]];
    //     Point p = Point(pv, 1, i);
    //     kmeans.AddOriginalPoint(p);
    // }

    // uint8_t p1values[] = {Test_Points[0]};
    // uint8_t p3values[] = {Test_Points[1]};
    // uint8_t p2values[] = {Test_Points[6]};
    // uint8_t p4values[] = {Test_Points[7]};
    // uint8_t p4values[] = {Test_Points[2]};
    // uint8_t p4values[] = {Test_Points[3]};

    // Point p1 = Point(p1values, 1, 0);
    // kmeans.AddOriginalPoint(p1);
    // Point p2 = Point(p2values, 1, 1);
    // kmeans.AddOriginalPoint(p2);
    // Point p3 = Point(p3values, 1, 2);
    // kmeans.AddOriginalPoint(p3);
    // Point p4 = Point(p4values, 1, 3);
    // kmeans.AddOriginalPoint(p4);

    // kmeans.DistanceBetweenPoints(p1, p2);

    kmeans.SetK(5);

    kmeans.showOriginalPoints();
    kmeans.Cluster();
    kmeans.showGroupedPoints();

    return 0;
}
