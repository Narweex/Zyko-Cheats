//#pragma once
//#include "BaseOption.hpp"
//#include "UIManager.hpp"
//
//namespace zyko::UserInterface
//{
//	
//
//	template <typename BoolType = bool>
//	class BetterBoolOption : public BaseOption<BoolOption<BoolType>>
//	{
//	public:
//		explicit BoolOption(const char* text, const char* description, BoolType* b00l, BoolDisplay displayType, float level, float min, float max, std::function<void()> action = [] {}) :
//			m_Bool(b00l),
//			m_DisplayInverted(displayInverted),
//			m_DisplayType(displayType)
//		{
//			Base::SetLeftText(text);
//			if (description)
//				Base::SetDescription(description);
//			Base::SetAction(std::move(action));
//		}
//
//		~BoolOption() noexcept = default;
//		BoolOption(BoolOption const&) = default;
//		BoolOption& operator=(BoolOption const&) = default;
//		BoolOption(BoolOption&&) = default;
//		BoolOption& operator=(BoolOption&&) = default;
//
//		const char* GetRightText() override
//		{
//				Base::
//				Base::SetRightText((*m_Bool ^ m_DisplayInverted) ? "~g~ON" : "~r~OFF");
//			
//
//			return Base::GetRightText();
//		}
//
//		void HandleAction(OptionAction action) override
//		{
//			if (action == OptionAction::EnterPress)
//			{
//				*m_Bool = !*m_Bool;
//			}
//
//			Base::HandleAction(action);
//		}
//	private:
//		BoolType* m_Bool;
//		BoolDisplay m_DisplayType;
//		BoolType m_DisplayInverted = false;
//
//		using Base = BaseOption<BoolOption<BoolType>>;
//	};
//}
