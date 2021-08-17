#include "dll_proxy.h"
#include <Windows.h>
#include <filesystem>

#define __PROXY_NAMESPACE__ proxy

#define DLL_PROXY_LOAD_RAW(handle, name) original_##name = GetProcAddress(handle, ###name)
#define DLL_PROXY_LOAD(name) DLL_PROXY_LOAD_RAW(h_version_dll, name)

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
	bool load()
	{
		const std::unique_ptr<char[]> system_directory(std::make_unique<char[]>(MAX_PATH));
		::GetSystemDirectoryA(system_directory.get(), MAX_PATH);
		const auto version_dll_path = std::filesystem::path(system_directory.get()) / "xinput9_1_0.dll";
		auto h_version_dll = LoadLibraryW(version_dll_path.wstring().c_str());
		if (!h_version_dll) return false;
		#define __DLL_NAME__ DLL_PROXY_LOAD
		#include "dll_proxy_symbols.h"
		#undef __DLL_NAME__
		return true;
	}
}

bool __load_proxy__() { return __PROXY_NAMESPACE__::load(); }