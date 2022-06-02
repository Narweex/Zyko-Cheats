#include <gta/joaat.hpp>
#include <filesystem>
namespace big
{
	class TranslationManager
	{
	public:
		explicit TranslationManager()
		{
			std::snprintf(&m_TranslationDir[0], sizeof(m_TranslationDir) - 1, "%s\\Zyko\\Translations\\", std::getenv("appdata"));
			
				if (!std::filesystem::exists(&m_TranslationDir[0]))
					std::filesystem::create_directory(&m_TranslationDir[0]);
			
			
		}

		/**
		 * \brief Loads a translation file
		 * \param name The stem of the translation file
		 */
		void LoadTranslations(const char* name)
		{
			m_Translations.clear();
			LOG(INFO) << "Loading %s translations.";

			try
			{
				char filename[100] = {};
				std::snprintf(&filename[0], sizeof(filename) - 1, "%s.zyko", name);
				auto filePath = std::filesystem::path(m_TranslationDir).append(filename);

				std::ifstream file(filePath);
				if (file.good())
				{
					std::stringstream str;
					std::string line;
					while (std::getline(file, line))
					{
						str << line << '\n';
					}

					nlohmann::json json = nlohmann::json::parse(str, nullptr, false);
					auto objects = json.get<std::unordered_map<std::string, nlohmann::json>>();

					for (auto&& [key, value] : objects)
					{
						m_Translations.emplace(RAGE_JOAAT_IMPL(key.c_str()), value.get<std::string>());
					}
				}
			}
			catch (nlohmann::detail::exception const&)
			{
				//g_Logger->Info("Exception while parsing %s translations: %s", name, ex.what());
				LOG(INFO) << "Failed to parse %s translations.";
			}
		}

		/**
		 * \brief Loads a translation file
		 * \param label The hash of the label
		 * \return The translation string, or a placeholder string
		 */
		const char* GetTranslation(std::uint32_t label)
		{
			if (auto it = m_Translations.find(label); it != m_Translations.end())
			{
				return &(it->second)[0];
			}
			else
			{
				static char buf[64];

				std::fill(std::begin(buf), std::end(buf), '\0');
				std::snprintf(&buf[0], sizeof(buf) - 1, "Unknown (0x%08X)", label);

				return &buf[0];
			}
		}

		/**
		 * \brief Gets the directory of the translations
		 * \return const char*
		 */
		const char* GetTranslationDirectory()
		{
			return &m_TranslationDir[0];
		}

		~TranslationManager() noexcept = default;
		TranslationManager(TranslationManager const&) = delete;
		TranslationManager(TranslationManager&&) = delete;
		TranslationManager& operator=(TranslationManager const&) = delete;
		TranslationManager& operator=(TranslationManager&&) = delete;
	private:
		std::unordered_map<std::uint32_t, std::string> m_Translations;
		char m_TranslationDir[1024] = {};
	};
	
		inline std::unique_ptr<TranslationManager> g_TranslationManager;
	
}

#define BIG_TRANSLATE_IMPL(label) (::Big::g_TranslationManager->GetTranslation(RAGE_IMPL_JOAAT(label)))
#define BIG_TRANSLATE(label) BIG_TRANSLATE_IMPL(label)
