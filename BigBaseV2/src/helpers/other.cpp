#include "other.h"
#include "pointers.hpp"
#include <auth/includes/curl/curl.h>
#include <auth/includes/curl/easy.h>
#include <Windows.h>

namespace zyko
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
       
         const char* file_path = "C:\\Zyko\\logo.ytd";
         if (!std::filesystem::exists("C:\\Zyko"))
         {
             std::filesystem::create_directory("C:\\Zyko");
         }

        if (file_exists(file_path))
        {
            static int textureID;
            if (g_pointers->m_file_register(&textureID, file_path, true, "logo.ytd", false))
            {
                LOG(INFO_TO_FILE) << "Header loaded.";
            }
        }
        else {
           
            LOG(INFO_TO_FILE) << "Creating header directory";
            std::filesystem::create_directory("C:\\Zyko\\");
            
            if(URLDownloadToFile(0, L"http://zykocheats.org/dashboard/downloads/logo.ytd", L"C:\\Zyko\\logo.ytd", 0, 0))
            {
                LOG(INFO_TO_FILE) << "Downloaded header";
            }
            register_ytd();
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

    static std::string Get_Autologin()
    {
        static std::string login_file = getenv(xorstr_("APPDATA"));
        login_file += xorstr_("\\Zyko\\autologin.json");
        return login_file;
    }
}