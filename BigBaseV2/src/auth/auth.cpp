#define CPPHTTPLIB_OPENSSL_SUPPORT
#define jwtkey xorstr_("-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmt0Muy8v3rH4qU5Bj7Ae\nf0qdNe/rBkzsWYr23kbja1ZU78j+3Ou6MZtXPIWSpZChFKeeL/9c66amDG4kWd4U\n6ROtrQaBDSTZk4iN95UNZIPR0kotNygxNuv/3Yvqq119gMoC9aVjmZE6OS9Mviof\nW5ysZtWzK7He36ZQbMTsiHJz3PIOU99Pw2Nl6tQid56N2/eue1X2gvbTYtXTUL2X\nDLQxhxOoIAKd8FhuWjoKaMbEXgU1Qt1hSpocxtF9F+bEuSs08dZa1WRkVTl6vQp5\njkhVhrZfKlNQk+vtbF+tjnJZS2fhWHzIr+FVbGJlGZBZkhX2nSB3eChdNMcZTU5o\nIwIDAQAB\n-----END PUBLIC KEY-----") //encrypt this

#define WIN32_LEAN_AND_MEAN
#include <notifications/notifications.h>
#include <filesystem>
#include <sha512.hpp>
#include "lazy_importer.hpp"
#include "auth/includes/curl/curl.h"
#include "auth/includes/httplib.h"
#include "auth/includes/jwt/jwt.hpp"
#include "../json/single_include/nlohmann/json.hpp"
#include "auth.hpp"
#include <auth/includes/httplib.h>
#pragma comment (lib, "urlmon.lib")  
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")
#include "lazy_importer.hpp"

HW_PROFILE_INFOA hwProfileInfo; 

using namespace httplib;
using namespace jwt;






void auth3(const std::string& username, const std::string& password)
{
    std::string login_file = getenv("APPDATA");
    login_file += "\\Zyko\\autologin.json";
    const char* logfil = login_file.c_str();
    

    if (std::filesystem::exists(login_file))
    {
        std::string username2;
        std::string password2;
        nlohmann::json login;

        std::ifstream login_file(logfil, std::ifstream::binary);
        login_file >> login;

        username2 = login[xorstr_("Username")];
        password2 = login[xorstr_("Password")];


    }
    GetCurrentHwProfileA(&hwProfileInfo);

    auto cli = httplib::SSLClient("zykocheats.org");
    httplib::Params data = { {"user", username}, {"pass", sw::sha512::calculate(password)}, {"hwid", std::string(hwProfileInfo.szHwProfileGuid) + std::string(hwProfileInfo.szHwProfileName)}};
    auto res = cli.Post("/dashboard/api/menuauth.php", data);
    std::cout << res->body << std::endl;
  
    jwt::jwt_object decodedToken = jwt::decode(res->get_header_value("res"), jwt::params::algorithms({ "RS256" }), jwt::params::secret(jwtkey), jwt::params::verify(true));
    std::stringstream strBuf;
    strBuf << decodedToken.payload();
    nlohmann::json payloadJson = nlohmann::json::parse(strBuf.str().c_str());

    if (payloadJson["result"] == "successpaid") {
        auth::login = true;
        auth::login1 = true;
       
       
        
    }
    else if (payloadJson["result"] == "invalid credentials") {
        LOG(INFO) << xorstr_("Invalid credentials!");
        zyko::Notify(xorstr_("Your credentials are invalid!"), auth::username, 7000, zyko::Error);
        Sleep(5000);
        exit(420);

    }
    else if (payloadJson["result"] == "invalid hwid") {
        LOG(INFO) << xorstr_("You Got Bad HWID");
        zyko::Notify(xorstr_("Invalid HWID Please open a ticket to reset it"), auth::username, 7000, zyko::Error);
        Sleep(5000);
        exit(420);
    }
    else  if (payloadJson["result"] == "banned") {
        zyko::Notify(xorstr_("You have been banned from the menu"), auth::username, 7000, zyko::Error);
        LOG(INFO) << xorstr_("You are banned.");
        Sleep(5000);
        exit(420);

    }
    else if (payloadJson["result"] == "success") {
        LOG(INFO) << xorstr_("You dont have license radeemed");
        zyko::Notify(xorstr_("You forgot to radeem a license"), auth::username, 7000, zyko::Error);
        Sleep(5000);
        exit(420);
    }
   
}


int Auth::auth(const std::string& username, const std::string& password)
{
    void (*nigger)(const std::string&username, const std::string&password);
    nigger = &auth3;
    LI_FN(nigger);
    (nigger)(username, password);
    return 1;
}





















