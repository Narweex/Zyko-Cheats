#pragma once

namespace big
{
    std::filesystem::path get_appdata_folder();
    std::filesystem::path get_additional_folder(const char* directory);
    bool file_exists(const std::filesystem::path& filename);
    void register_ytd();
}