#pragma once

#include "../ui/scene_view.h"

#include <imgui/imgui.h>
#include <imgui/imfilebrowser.h>

namespace nui
{
	class Property_Panel
	{
	public:
		Property_Panel()
		{			
			mCurrentFile = "< ... >";

			mFileDialog.SetTitle("Open Mesh");
			mFileDialog.SetTypeFilters({ ".fbx", ".obj" });	
		}

		void render(nui::SceneView* mScene);

		void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
		{
			mModelLoadCallback = callback;
		}

	private:

		// create a file browser instance
		ImGui::FileBrowser mFileDialog;

		std::function<void(const std::string&)> mModelLoadCallback;

		std::string mCurrentFile;

	};
}
