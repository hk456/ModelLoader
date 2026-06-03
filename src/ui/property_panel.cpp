#include "property_panel.h"

namespace nui
{
	void Property_Panel::render(nui::SceneView* mScene)
	{
		auto model = mScene->get_model();

		ImGui::Begin("Properties");
		if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Open..."))
			{
				mFileDialog.Open();
			}
			ImGui::SameLine(0, 5.0f);
			ImGui::Text(mCurrentFile.c_str());
		}

		ImGui::End();

		mFileDialog.Display();
		if (mFileDialog.HasSelected())
		{
			auto file_path = mFileDialog.GetSelected().string();
			mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

			if (mModelLoadCallback)
			{
				mModelLoadCallback(file_path);
			}

			mFileDialog.ClearSelected();
		}



	}
}