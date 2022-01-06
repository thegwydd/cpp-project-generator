#ifndef __<#PNAME#>_LIBRARY_HEADER__
#define __<#PNAME#>_LIBRARY_HEADER__

/* Compiler detection */
#if defined(_MSC_VER)
#    define __SHARED_LIBRARY_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#    define __SHARED_LIBRARY_IMPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#elif defined(__GNUC__)
#    define __SHARED_LIBRARY_EXPORT __attribute__ ((dllexport))
#    define __SHARED_LIBRARY_IMPORT __attribute__ ((dllimport))
#elif defined(__clang__)
#    define __SHARED_LIBRARY_EXPORT __attribute__ ((dllexport))
#    define __SHARED_LIBRARY_IMPORT __attribute__ ((dllimport))
#else
// add here any thing your compiler likes to export/import shared object stuff
#endif

#ifdef <#PNAME#>_EXPORTS
#   define <#PNAME#>_API      __SHARED_LIBRARY_EXPORT
#else
#   define <#PNAME#>_API      __SHARED_LIBRARY_IMPORT
#endif

#include "<PNAME>_version.h"

extern "C" 
{

    int <#PNAME#>_API my_c_export(int num);
    
}


#endif // __<#PNAME#>_LIBRARY_HEADER__
