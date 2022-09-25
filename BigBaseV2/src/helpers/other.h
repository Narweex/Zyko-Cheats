#pragma once

namespace big
{
    std::filesystem::path get_appdata_folder();
    std::filesystem::path get_additional_folder(const char* directory);
    bool file_exists(const std::filesystem::path& filename);
    void register_ytd();
    std::size_t callback(const char* in, std::size_t size, std::size_t num, std::string* out);
    std::uint64_t get_rid_from_name(std::string name);
    void CopyToClipboard(const char* text);
}