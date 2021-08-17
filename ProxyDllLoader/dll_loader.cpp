#include "file_utils.hpp"

#define DL_FOLDER_NAME "adaf-dll"

#ifndef UNICODE
    #define DL_STR string
#else
    #define DL_STR wstring
#endif // UNICODE

void __load_dll__()
{
	const auto base_path = get_executable_directory() / TEXT(DL_FOLDER_NAME);
	if (!std::filesystem::is_directory(base_path) || !std::filesystem::exists(base_path))
	{
		std::filesystem::create_directory(base_path);
	}
	for (const auto& file : std::filesystem::directory_iterator(base_path))
	{
		LoadLibrary(file.path().DL_STR().c_str());
	}
}