// test_application.cpp : Defines the entry point for the console application.
//
#include "application.h"

#ifdef _MSC_VER
//////////////////////////////////////////////////////////////////////////
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPWSTR, INT)
    {
    UNREFERENCED_PARAMETER(hInst);
    UNREFERENCED_PARAMETER(hPrevInstance);

    int argc;
    char ** argv;
    {
    LPWSTR * lpArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
    argv = (char **)malloc(argc * sizeof(char *));
    size_t size, i = 0;
    for (; i < argc; ++i) {
        size = wcslen(lpArgv[i]) + 1;
        argv[i] = (char *)malloc(size);
        wcstombs(argv[i], lpArgv[i], size);
        }
    LocalFree(lpArgv);
    }

#else
//////////////////////////////////////////////////////////////////////////
int main(int argc, char * argv[])
    {
#endif

    int code = application::get_insance().run(argc, argv);

#ifdef _MSC_VER
    {
    int i = 0;
    for (; i < argc; ++i) free(argv[i]);
    free(argv);
    }
#endif

    return code;
    }


