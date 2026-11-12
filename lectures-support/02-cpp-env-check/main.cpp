#include <stdlib.h>

#include "ExampleClass.h"

int main(int argc, char* argv[])
{
    auto object = new ExampleClass();

    object->printHelloWorld();

    return EXIT_SUCCESS;
}
