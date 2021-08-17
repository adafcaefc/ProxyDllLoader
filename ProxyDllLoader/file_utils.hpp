#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
    #define __API__ inline
#else 
    #define __API__ static
#endif

#include <Windows.h>
#include <filesystem>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

__API__ std::filesystem::path get_dll_path()
{
    TCHAR DllPath[MAX_PATH] = { 0 };
    GetModuleFileName((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
    return std::filesystem::path(DllPath);
}

__API__ std::filesystem::path get_dll_directory()
{
    return get_dll_path().parent_path();
}

__API__ std::filesystem::path get_executable_path()
{
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    return szFilePath;
}

__API__ std::filesystem::path get_executable_directory()
{
    return get_executable_path().parent_path();
}

#endif // FILE_UTILS_HPP