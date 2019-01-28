#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

    int input = atoi(argv[1]);
    int input2 = atoi(*(argv+1));
    printf("%d", input);
    printf("\n%d", input2);

    printf("\n%s", argv[0]);

    int *inputArray = &argv;
    //char *inputArray = (int*) (argv+1);

    //printf("\nFirst element: %c", *(argv+1));

    printf("\n%d", *inputArray);

    /*
    for (int i=0; i<sizeof(input); i++) {
            printf("%d, ", input[i]);
    }
    */

    return input;
}
