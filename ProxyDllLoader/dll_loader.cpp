#include "file_utils.hpp"

#define LOADER_FOLDER_NAME "adaf-dll"

#ifndef UNICODE
    #define LOADER_STR string
#else
    #define LOADER_STR wstring
#endif // UNICODE

void __load_dll__()
{
	const auto base_path = get_executable_directory() / TEXT(LOADER_FOLDER_NAME);
	if (!std::filesystem::is_directory(base_path) || !std::filesystem::exists(base_path))
	{
		std::filesystem::create_directory(base_path);
	}
	for (const auto& file : std::filesystem::directory_iterator(base_path))
	{
		LoadLibrary(file.path().LOADER_STR().c_str());
	}
}