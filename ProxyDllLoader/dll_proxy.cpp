#include "dll_proxy.h"
#include <Windows.h>
#include <filesystem>

#define __PROXY_MODULE__    h_proxy_dll
#define __PROXY_PATH__      proxy_dll_path
#define __PROXY_NAMESPACE__ proxy

#ifdef MAX_PATH
    #define __PROXY_MAX_PATH__ MAX_PATH
#else
    #define __PROXY_MAX_PATH__ 260
#endif // MAX_PATH

#define DLL_PROXY_LOAD_RAW(handle, name) __PROXY_NAMESPACE__::original_##name = GetProcAddress(handle, ###name)
#define DLL_PROXY_LOAD(name) DLL_PROXY_LOAD_RAW(__PROXY_MODULE__, name)

#define DLL_PROXY_EXPORT(name)                                       \
    namespace __PROXY_NAMESPACE__ { FARPROC original_##name; }       \
    __volatile __declspec(naked) __declspec(noinline) void _##name() \
    {                                                                \
        __asm jmp[__PROXY_NAMESPACE__::original_##name]              \
    }

#define __DLL_NAME__ DLL_PROXY_EXPORT
#include "dll_proxy_symbols.h"
#undef __DLL_NAME__

namespace __PROXY_NAMESPACE__
{
	static std::filesystem::path get_system_directory()
	{
		const auto system_directory(std::make_unique<char[]>(__PROXY_MAX_PATH__));
		::GetSystemDirectoryA(system_directory.get(), __PROXY_MAX_PATH__);
		return std::filesystem::path(system_directory.get());
	}

	static bool load()
	{
		const auto __PROXY_PATH__   = get_system_directory() / "xinput9_1_0.dll";
		const auto __PROXY_MODULE__ = LoadLibraryW(__PROXY_PATH__.wstring().c_str());
		if (!__PROXY_MODULE__) return false;
		#define __DLL_NAME__ DLL_PROXY_LOAD
		#include "dll_proxy_symbols.h"
		#undef __DLL_NAME__
		return true;
	}
}

bool __load_proxy__() { return __PROXY_NAMESPACE__::load(); }