

#include <vector>

bool ContainInt(std::vector<int> container, int item)
{
    if (container.size() > 0)
    {
        for (int i : container)
        {
            if (i == item)
            {
                return true;
            }
        }
    }
    return false;
}