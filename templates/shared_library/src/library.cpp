#include "library.h"

#include <iostream>

extern "C" 
{

    int <#PNAME#>_API my_c_export(int num)
    {
        std::cout << "this is my <PNAME> export. Argument num is " << num << std::endl;
        return num;
    }


}

