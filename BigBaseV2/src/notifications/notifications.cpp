#include "notifications.h"
namespace zyko
{
	

	void Notify(const char* title, const char* text, uint32_t duration, int level)
	{
		ImGuiToast toast(ImGuiToastType_None, duration);
		switch (level)
		{
		case 1:
			toast.set_type(ImGuiToastType_None);
			toast.set_title(title);
			toast.set_content(text);
			ImGui::InsertNotification(toast);
			
			break;

		case 2:
			toast.set_type(ImGuiToastType_Success);
			toast.set_title(title);
			toast.set_content(text);
			ImGui::InsertNotification(toast);
			break;

		case 3:
			toast.set_type(ImGuiToastType_Warning);
			toast.set_title(title);
			toast.set_content(text);
			ImGui::InsertNotification(toast);
			break;

		case 4:
			toast.set_type(ImGuiToastType_Error);
			toast.set_title(title);
			toast.set_content(text);
			ImGui::InsertNotification(toast);
			break;
		default:
			toast.set_type(ImGuiToastType_None);



		}
	}
}
