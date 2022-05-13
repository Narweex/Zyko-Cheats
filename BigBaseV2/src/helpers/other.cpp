#include "other.h"
#include "pointers.hpp"

namespace big
{
    void validate_path(std::filesystem::path path)
    {
        if (!std::filesystem::exists(path))
        {
            std::filesystem::create_directory(path);
        }
        else if (!std::filesystem::is_directory(path))
        {
            std::filesystem::remove(path);
            std::filesystem::create_directory(path);
        }
    }

    std::filesystem::path get_appdata_folder()
    {
        auto file_path = std::filesystem::path(std::getenv(xorstr_("appdata")));
        file_path /= xorstr_("Zyko");

        validate_path(file_path);

        return file_path;
    }

    std::filesystem::path get_additional_folder(const char* directory)
    {
        auto file_path = get_appdata_folder();
        file_path /= directory;

        validate_path(file_path);

        return file_path;
    }

    bool file_exists(const std::filesystem::path& filename)
    {
        const std::filesystem::path p = filename;
        return (std::filesystem::exists(p));
    }
    
    void register_ytd()
    {
        auto path = get_appdata_folder().string();
        std::string file = "\\logo.ytd";
        std::string file_path = path + file;
        if (file_exists(file_path))
        {
            int textureID;
            if (g_pointers->m_file_register(&textureID, file_path.c_str(), true, "logo.ytd", false))
            {
                LOG(INFO) << xorstr_("Header loaded.");
            }
            else
            {
                LOG(INFO) << xorstr_("Failed to load header.");
            }
        }
    }
}