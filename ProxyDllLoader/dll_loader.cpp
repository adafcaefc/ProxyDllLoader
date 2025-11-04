#include "file_utils.hpp"

#define LOADER_FOLDER_NAME "adaf"

#ifndef UNICODE
    #define LOADER_STR string
#else
    #define LOADER_STR wstring
#endif // UNICODE

#include <thread>

void __load_dll__()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    const auto base_path = get_executable_directory() / TEXT(LOADER_FOLDER_NAME);
    if (!std::filesystem::is_directory(base_path) || !std::filesystem::exists(base_path))
    {
        std::filesystem::create_directory(base_path);
    }
    for (const auto& file : std::filesystem::directory_iterator(base_path))
    {
        if (file.is_regular_file())
        {
            auto extension = file.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            if (extension == ".dll" || extension == ".asi")
            {
                LoadLibrary(file.path().LOADER_STR().c_str());
            }
        }
    }
}