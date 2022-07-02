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

    std::size_t callback(const char* in, std::size_t size, std::size_t num, std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);

        return totalBytes;
    }

    std::uint64_t get_rid_from_name(std::string name)
    {
        // RID API, may get outdated though
        std::string site = xorstr_("https://eintim.one/rid/?username=") + name;
        std::string result;

        // RID
        uint64_t rid = 0;

        // Request
        CURL* curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, site.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, xorstr_("GET"));
            curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        if (result.empty() || result == xorstr_("User not found."))
            return rid;

        std::istringstream iss(result);
        iss >> rid;

        return rid;
    }
}