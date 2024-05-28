#include "map_util.c"



int main() {
    printLegend();

    // Set the number of trainers directly
    int numtrainers = 10;

    printf("Number of trainers: %d\n", numtrainers);
    printmap(' ', -1, 200, 200, numtrainers);

    return 0;
}