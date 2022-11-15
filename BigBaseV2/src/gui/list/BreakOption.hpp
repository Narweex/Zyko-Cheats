#pragma once
#include "BaseOption.hpp"
#include "UIManager.hpp"



//const char* StringToChar(std::string string)
//{
//	return _strdup(string.c_str());
//}
//AbstractOption* opt;
namespace zyko::UserInterface
{
	class BreakOption : public BaseOption<BreakOption>
	{
	public:
		explicit BreakOption() = default;
		explicit BreakOption(const char* text, const char* description = nullptr, std::function<void()> action = [] {})
		{
			//	SetLeftText(StringToChar("" + std::string(text) + ""));
			SetRightText(((std::string)"~italic~[ " + text + " ]").c_str());
			


			if (description)
				SetDescription(description);
			SetAction(std::move(action));
		}

		bool GetFlag(OptionFlag flag) override
		{
			if (flag == OptionFlag::Break)
			{
				return true;
			}

			return false;
		}
		~BreakOption() noexcept = default;
		BreakOption(BreakOption const&) = default;
		BreakOption& operator=(BreakOption const&) = default;
		BreakOption(BreakOption&&) = default;
		BreakOption& operator=(BreakOption&&) = default;
	};
}


