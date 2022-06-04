#include "ASILoader.h"
#include "logger.hpp"
#include "core/PEImage.h"
#include "helpers/other.h"
using namespace Utility;

void ASILoader::Initialize()
{
	big::get_additional_folder("ASI");

	LOG(INFO) << xorstr_("Loading *.asi plugins.");

	std::string currentFolder = std::getenv("appdata");
	currentFolder += "\\Zyko";
	const std::string asiFolder = currentFolder + "\\ASI";
	const std::string asiSearchQuery = asiFolder + "\\*.asi";

	WIN32_FIND_DATAA fileData;
	HANDLE fileHandle = FindFirstFileA(asiSearchQuery.c_str(), &fileData);
	if (fileHandle != INVALID_HANDLE_VALUE) {

		do {

			const std::string pluginPath = asiFolder + "\\" + fileData.cFileName;

			LOG(INFO) << xorstr_("Loading ") << pluginPath.c_str();

			PEImage pluginImage;
			if (!pluginImage.Load(pluginPath)) {

				LOG(FATAL) << xorstr_("Failed to load image.");
				continue;
			}

			// Image not compatible, needs patching
			if (!pluginImage.IsOpenVHookCompatible()) {

				LOG(INFO) << xorstr_("Detected non compatible image. Patching compatibility.");

				if (pluginImage.PatchCompatibility()) {
					LOG(INFO) << xorstr_("Successfully patched.");
				}
				else {
					LOG(FATAL) << xorstr_("Failed to patch compatibility.");
					continue;
				}
			}

			// Image compatible (now), load it
			HMODULE module = LoadLibraryA(pluginPath.c_str());
			if (module) {
				LOG(INFO) << xorstr_("Loaded ") << fileData.cFileName << " -> " HEX_TO_UPPER(module);
			}
			else {
				LOG(FATAL) << xorstr_("Failed to load");
			}

		} while (FindNextFileA(fileHandle, &fileData));

		FindClose(fileHandle);
	}

	LOG(INFO) << xorstr_("Finished loading *.asi plugins.");
}
