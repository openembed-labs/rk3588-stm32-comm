#include <stdbool.h>
#include <stddef.h>

bool contains(const int *array, size_t size, int value)
{
    for (size_t i = 0; i < size; i++)
    {
        if (array[i] == value)
        {
            return true;
        }
    }
    return false;
}
