#define CPPHTTPLIB_OPENSSL_SUPPORT
#define jwtkey "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmt0Muy8v3rH4qU5Bj7Ae\nf0qdNe/rBkzsWYr23kbja1ZU78j+3Ou6MZtXPIWSpZChFKeeL/9c66amDG4kWd4U\n6ROtrQaBDSTZk4iN95UNZIPR0kotNygxNuv/3Yvqq119gMoC9aVjmZE6OS9Mviof\nW5ysZtWzK7He36ZQbMTsiHJz3PIOU99Pw2Nl6tQid56N2/eue1X2gvbTYtXTUL2X\nDLQxhxOoIAKd8FhuWjoKaMbEXgU1Qt1hSpocxtF9F+bEuSs08dZa1WRkVTl6vQp5\njkhVhrZfKlNQk+vtbF+tjnJZS2fhWHzIr+FVbGJlGZBZkhX2nSB3eChdNMcZTU5o\nIwIDAQAB\n-----END PUBLIC KEY-----" //encrypt this

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>
#include <Urlmon.h>
#include <filesystem>
#include <http.h>
#include <sha512.hpp>
#include <urlmon.h>
#include <winspool.h>
#include <httplib.h>
#include <winsock2.h>
#include <ws2def.h>
#include "auth/includes/ini.h"
#include <urlmon.h>
#include <conio.h>
#include <sys/stat.h>
#include "auth/includes/anti_debug.hpp"
#include "core/xor_string.hpp"
#include <WinBase.h>
#include "auth/includes/httplib.h"
#include "auth/includes//curl/curl.h"
#include <shellapi.h>
#include <fstream>
#include "auth/includes/jwt/jwt.hpp"
#include "../json/single_include/nlohmann/json.hpp"
#include "auth.hpp"
#pragma comment (lib, "urlmon.lib")  
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")
HW_PROFILE_INFOA hwProfileInfo;

using namespace httplib;
using namespace jwt;



void auth::auth()
{
    std::string password1(password);
    std::string username1(username);
    
    GetCurrentHwProfileA(&hwProfileInfo);

    auto cli = httplib::SSLClient("zykocheats.org");
    httplib::Params data = { {"user", username}, {"pass", sw::sha512::calculate(password)}, {"hwid", std::string(hwProfileInfo.szHwProfileGuid) + std::string(hwProfileInfo.szHwProfileName)}};
    auto res = cli.Post("/dashboard/api/menuauth.php", data);
    std::cout << res->body << std::endl;

    jwt::jwt_object decodedToken = jwt::decode(res->get_header_value("res"), jwt::params::algorithms({ "RS256" }), jwt::params::secret(jwtkey), jwt::params::verify(true));
    std::stringstream strBuf;
    strBuf << decodedToken.payload();
    nlohmann::json payloadJson = nlohmann::json::parse(strBuf.str().c_str());

    //verify more then the result, add integrity checks and encrypt all strings in this file
    if (payloadJson["result"] == "invalid credentials") {
        LOG(INFO) << "Invalid credentials but works";
        
        

    }
    if (payloadJson["result"] == "invalid hwid") {
        LOG(INFO) << "Bad hwid who cares";
    }
    if (payloadJson["result"] == "banned") {
        LOG(INFO) << "you got banned lol";
    }
    if (payloadJson["result"] == "success") {
        LOG(INFO) << "success without license";
    }
    if (payloadJson["result"] == "successpaid") {
        LOG(INFO) << "Everything works";
    }
}

void main()
{


    //if (auth::authloop)
    //{

    //    auth::auth();
    //   
    //}
}



