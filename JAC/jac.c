// Built-in
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Local
#include "include/util.h"
#include "include/jacio.h"




int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filePath>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filePath = argv[1];
    FILE* file = fopen(filePath, "r");

    if (!file) 
    {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return EXIT_FAILURE;
    }
    
    while (!feof(file))
    {
        printf("%d", getToken(file).type);
    }

    return EXIT_SUCCESS;
}
