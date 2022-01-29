
#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <string>

#include "kutils.h"
#include "kmeansxor.h"

class KMeansXOR
{
public:
    typedef struct Point
    {
        uint value_count;
        uint groupID;
        uint cellID;

        uint8_t *values;

        // must write this constructer if you have ones with params
        Point()
        {
        }

        Point(uint8_t *v, uint c, uint id) : values(v), value_count(c), groupID(0), cellID(id)
        {
        }

    } Point;

    typedef struct DimPoint // point in dimension
    {
        uint8_t dimension[BIT_COUNT];
        DimPoint()
        {
            for (size_t i = 0; i < BIT_COUNT; i++)
            {
                dimension[i] = 0;
            }
        }

        void SetDimValues(uint8_t dim[BIT_COUNT])
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
    KMeansXOR(/* args */);
    ~KMeansXOR();

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

    uint DistanceBetweenPoints(Point &point1, DimPoint &dp2);
    uint DistanceBetweenPoints(DimPoint &dp1, DimPoint &dp2);

    bool IsCenterShiftExist(DimPoints &prevCenters, DimPoints &curCenters);

    DimPoint Point2Dim(Point &point);

    void showPoint(Point &point);
    void showOriginalPoints();
    void showGroupedPoints();

    int GetByteCount()
    {
        return (BIT_COUNT % BIT_STEP == 0) ? (BIT_COUNT / BIT_STEP) : ((BIT_COUNT / BIT_STEP) + 1);
    }
};

KMeansXOR ::KMeansXOR(/* args */)
{
    originalPoints.clear();
    centerPoints.clear();
    groupedPoints.clear();
    K = 0;
}

KMeansXOR ::~KMeansXOR()
{
}

KMeansXOR::DimPoint KMeansXOR::Point2Dim(Point &point)
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

uint KMeansXOR::DistanceBetweenPoints(Point &point1, DimPoint &dp2)
{
    // construct dimention values array for 2 points
    uint8_t p1[BIT_COUNT]{};
    uint dist = 0;
    for (int i = 0; i < BIT_COUNT; i++)
    {
        int index = (i / BIT_STEP);
        int bit2switch = (i % BIT_STEP);
        p1[i] = (point1.values[index] >> bit2switch) & 0x1;

        if (p1[i] != dp2.dimension[i])
        {
            dist++;
        }
    }
    return dist;
}

uint KMeansXOR::DistanceBetweenPoints(DimPoint &dp1, DimPoint &dp2)
{
    uint dist = 0;
    for (int i = 0; i < BIT_COUNT; i++)
    {
        if (dp1.dimension[i] != dp2.dimension[i])
        {
            dist++;
        }
    }
    return dist;
}

bool KMeansXOR::IsCenterShiftExist(DimPoints &prevCenters, DimPoints &curCenters)
{
    for (int i = 0; i < K; i++)
    {
        int dist = DistanceBetweenPoints(prevCenters[i], curCenters[i]);
        if (dist > DIST_NEAR_ZERO)
        {
            return true;
        }
    }
    return false;
}

KMeansXOR::DimPoints KMeansXOR::UpdateGroupCenter(std::vector<Points> &grpPoints, DimPoints centers)
{
    uint bitvalues[BIT_COUNT]{}; // add {} to init with 0
    for (size_t i = 0; i < K; i++)
    {
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
            // if 1 is more than half count, set to 1
            if (bitvalues[index] > (grpSize / 2))
            {
                bitvalues[index] = 1;
            }
            else
            {
                bitvalues[index] = 0;
            }
        }

        uint8_t pv[BIT_COUNT]{};
        for (int index = 0; index < BIT_COUNT; ++index)
        {
            pv[index] = bitvalues[index];
        }

        centers[i].SetDimValues(pv);
    }

    return centers;
}

bool KMeansXOR::Cluster()
{
    InitKCenter();

    DimPoints centers(K);

    int pointCount = originalPoints.size();

    do
    {
        for (size_t i = 0; i < pointCount; i++)
        {
            uint min_dist = BIT_COUNT;
            int grp = 0;
            for (size_t j = 0; j < K; j++)
            {
                uint dist = DistanceBetweenPoints(originalPoints[i], centerPoints[j]);

                // if (min_dist - dist > DIST_ZERO)//TODO convert bug
                if (min_dist > dist)
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

bool KMeansXOR::InitKCenter()
{
    bool INIT_CENTER_RANDOM = false;
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
                int seed = random() % (size);
                while (ContainInt(usedseed, seed))
                {
                    seed = random() % (size);
                }
                usedseed.push_back(seed);
                centerPoints[i] = Point2Dim(originalPoints[seed]);
            }
        }
        // equal assigment
        else
        {
            int step = size / (K + 1); // K+1, 1 more is for center the centerpoints
            for (int i = 0; i < K; i++)
            {
                int seed = (i + 1) * step;
                centerPoints[i] = Point2Dim(originalPoints[seed]);
            }
        }

        return true;
    }

    return false;
}

bool KMeansXOR::AddOriginalPoint(Point point)
{
    originalPoints.push_back(point);

    uint8_t p1[BIT_COUNT];
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

bool KMeansXOR::SetOriginalPoints(Points &points)
{
    if (points.size() > 0)
    {
        originalPoints = points;
        return true;
    }
    return false;
}

namespace XOR
{

    KMeansXOR loadRamdomPoint()
    {
        KMeansXOR kmeans;
        // generate random 0 1 values
        srand(unsigned(time(NULL)));
        int *values = new int[TEST_POINT_RANDOM_COUNT * TEST_BYTE_COUNT_PER_CELL * BIT_STEP]{};
        for (size_t i = 0; i < (TEST_POINT_RANDOM_COUNT * TEST_BYTE_COUNT_PER_CELL * BIT_STEP); i++)
        {
            int seed = random() % 2;
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

            KMeansXOR::Point p = KMeansXOR::Point(pv, value_count, i);
            kmeans.AddOriginalPoint(p);
        }

        return kmeans;
    }

    KMeansXOR loadStringPoint()
    {
        KMeansXOR kmeans;

        int value_count = kmeans.GetByteCount();

        for (size_t i = 0; i < TEST_POINT_COUNT; i++)
        {
            uint8_t *pv = new uint8_t[value_count]; // TODO delete

            int start = i * BIT_COUNT;
            int index = 0;

            for (int j = start; j < start + BIT_COUNT; j++)
            {
                int index = ((j - start) / BIT_STEP);
                int bit2switch = (j % BIT_STEP);
                // int v = std::stoi(TEST_POINT_STR.substr(j, j + 1));
                int v = TEST_POINT_STR.at(j) - '0';
                pv[index] |= (v & 0x1) << bit2switch;
            }

            KMeansXOR::Point p = KMeansXOR::Point(pv, value_count, i);
            kmeans.AddOriginalPoint(p);
        }

        return kmeans;
    }
}

void KMeansXOR::showPoint(Point &point)
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

void KMeansXOR::showOriginalPoints()
{
    std::cout << "Original Points Are:" << std::endl;
    for (Point p : originalPoints)
    {
        showPoint(p);
    }
}

void KMeansXOR::showGroupedPoints()
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

    KMeansXOR kmeans;

    kmeans = XOR::loadStringPoint();

    // kmeans = XOR::loadRamdomPoint();

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

    kmeans.SetK(4);

    kmeans.showOriginalPoints();
    kmeans.Cluster();
    kmeans.showGroupedPoints();

    return 0;
}
