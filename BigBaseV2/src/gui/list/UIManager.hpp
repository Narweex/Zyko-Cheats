#pragma once
#include "common.hpp"
#include "gta/color.hpp"
#include "gta/vector.hpp"
#include "TextBox.hpp"
#include "AbstractSubmenu.hpp"

namespace zyko::UserInterface
{
	enum class Font : std::int32_t
	{
		ChaletLondon = 0,
		HouseScript = 1,
		Monospace = 2,
		Wingdings = 3,
		ChaletComprimeCologne = 4,
		Pricedown = 7
	};

	enum class HeaderType : std::int32_t
	{
		Static = 0,
		Gradient = 1
	};

	struct Rectangle
	{
		rage::vector2 m_Center;
		rage::vector2 m_Size;

		bool IsInBounds(rage::vector2 point)
		{
			auto left = m_Center.x - (m_Size.x / 2.f);
			auto right = m_Center.x + (m_Size.x / 2.f);
			auto top = m_Center.y + (m_Size.y / 2.f);
			auto bottom = m_Center.y - (m_Size.y / 2.f);
			
			if (point.x > right || point.x < left)
				return false;
			if (point.y > top || point.y < bottom)
				return false;

			return true;
		}
	};
	class UIManager
	{
	public:
		explicit UIManager();
		~UIManager() noexcept;

		template <typename SubmenuType, typename ...TArgs>
		void AddSubmenu(TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_SubmenuStack.empty())
			{
				m_SubmenuStack.push(sub.get());
			}

			m_AllSubmenus.push_back(std::move(sub));
		}

		void SwitchToSubmenu(std::uint32_t id)
		{
			for (auto&& sub : m_AllSubmenus)
			{
				if (sub->GetId() == id)
				{
					m_SubmenuStack.push(sub.get());
					return;
				}
			}
		}

		void OnTick();
	public:
		std::mutex m_Mutex;
		
		bool m_Opened = false;
		bool m_MouseLocked = false;
		float m_PosX = 0.8f;
		float m_PosY = 0.1f;
		float m_Width = 0.24f;
		std::size_t m_OptionsPerPage = 11;
		bool m_Sounds = true;
		
		// Input
		std::int32_t m_OpenDelay = 350;
		std::int32_t m_BackDelay = 200;
		std::int32_t m_EnterDelay = 200;
		std::int32_t m_VerticalDelay = 150;
		std::int32_t m_HorizontalDelay = 150;

		// Header
		float m_HeaderHeight = 0.1f;
		bool m_HeaderText = false;
		float m_HeaderTextSize = 1.f;
		Font m_HeaderFont = Font::ChaletLondon;
		rage::rgbaColor m_HeaderBackgroundColor{ 2, 130, 53, 255 };
		rage::rgbaColor m_HeaderTextColor{ 255, 255, 255, 255 };
		HeaderType m_HeaderType = HeaderType::Gradient;

		bool m_HeaderGradientTransparent = false;
		bool m_HeaderGradientFlip = false;
		float m_HeaderGradientStretch = 0.f;
		float m_HeaderGradientFiller = 0.f;
		rage::rgbaColor m_HeaderGradientColorLeft{ 0, 186, 255, 255 };
		rage::rgbaColor m_HeaderGradientColorRight{ 252, 0, 255, 255 };

		// Submenu bar
		float m_SubmenuBarHeight = 0.03f;
		float m_SubmenuBarTextSize = 0.26f;
		Font m_SubmenuBarFont = Font::ChaletLondon;
		float m_SubmenuBarPadding = 2.1f;
		rage::rgbaColor m_SubmenuBarBackgroundColor{ 0, 0, 0, 250 };
		rage::rgbaColor m_SubmenuBarTextColor{ 255, 255, 255, 255 };

		// Options
		float m_OptionHeight = 0.029f;
		float m_OptionTextSize = 0.21f;
		Font m_OptionFont = Font::ChaletLondon;
		float m_OptionPadding = 2.1f;
		rage::rgbaColor m_OptionSelectedTextColor{ 10, 10, 10, 255 };
		rage::rgbaColor m_OptionUnselectedTextColor{ 255, 255, 255, 255 };
		rage::rgbaColor m_OptionSelectedBackgroundColor{ 255, 255, 255, 255 };
		rage::rgbaColor m_OptionUnselectedBackgroundColor{ 10, 10, 10, 180 };

		// Footer
		float  m_FooterHeight = 0.020f;
		float  m_FooterSpriteSize = 0.025f;
		rage::rgbaColor  m_FooterBackgroundColor{ 0, 0, 0, 250 };
		rage::rgbaColor  m_FooterSpriteColor{ 255, 255, 255, 250 };

		// Description
		float m_DescriptionHeightPadding = 0.01f;
		float m_DescriptionHeight = 0.033f;
		float m_DescriptionTextSize = 0.25f;
		Font m_DescriptionFont = Font::ChaletLondon;
		rage::rgbaColor m_DescriptionBackgroundColor{ 0, 0, 0, 120 };
		rage::rgbaColor m_DescriptionTextColor{ 255, 255, 255, 255 };
		float m_DescriptionSpriteSize = 0.025f;
		float m_DescriptionPadding = 2.1f;
		rage::rgbaColor m_DescriptionSpriteColor{ 255, 255, 255, 255 };
		static void playerinfo()
		{
			void DrawPlayerInfo();
		}
			
	public:
		bool m_OpenKeyPressed = false;
		bool m_BackKeyPressed = false;
		bool m_EnterKeyPressed = false;
		bool m_UpKeyPressed = false;
		bool m_DownKeyPressed = false;
		bool m_LeftKeyPressed = false;
		bool m_RightKeyPressed = false;
		void CheckForInput();
		void HandleInput();
		void ResetInput();

		float m_DrawBaseY{};
		void DrawHeader();
		void DrawSubmenuBar(AbstractSubmenu* sub);
		void DrawOption(AbstractOption* opt, bool selected);
		void DrawFooter();
		void DrawDescription();
		
		
		void DrawRect(float x, float y, float width, float height, rage::rgbaColor color);
		void DrawSprite(const char* dict, const char* texture, float x, float y, float width, float height, rage::rgbaColor color, float rotation);
		void DrawLeftText(const char* text, float x, float y, float size, Font font, rage::rgbaColor color, bool outline, bool shadow);
		void DrawCenteredText(const char* text, float x, float y, float size, Font font, rage::rgbaColor color, bool outline, bool shadow);
		void DrawRightText(const char* text, float x, float y, float size, Font font, rage::rgbaColor color, bool outline, bool shadow);
		float GetTextHeight(Font font, float size);
		rage::vector2 GetSpriteScale(float size);

		bool IsMouseLocked();
		Rectangle GetMenuRect();
		rage::vector2 GetMousePos();

		std::vector<std::unique_ptr<AbstractSubmenu>> m_AllSubmenus;
		std::stack<AbstractSubmenu*, std::vector<AbstractSubmenu*>> m_SubmenuStack;
	};
}

namespace zyko
{
	inline UserInterface::UIManager* g_UiManager{};
}
