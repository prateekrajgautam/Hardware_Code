#include <stdio.h>
using namespace std;
int array[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
int(*ptr) = array;

int printarray(int *array)
{
    int i;
    for (i = 0; i < 9; i++)
    {
        printf("%d\n", array[i]);
        array[i] = array[i] + 100;
    }
    return (i);
// }

int main()
{
    int i;
    int j = printarray(ptr);
    printf("%d\n", j);
    for (i = 0; i < 9; i++)
    {
        printf("%d\n", array[i]);
    }
    return (0);
}
