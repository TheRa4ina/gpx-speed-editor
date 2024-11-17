#pragma once
#include "toolkit.h"
#include "gpxadapter.h"
#include "gpxspeedmodifier.h"
#include "imgui\extensions\filebrowser\imfilebrowser.h"
#include <vector>


class App
{
public:
	App(bool* is_open);
	void getGui();
	bool isValidExt(std::string file_name);
private:
	GpxAdapter m_file_adapter;
	GpxSpeedModifier m_speed_modifier;

	std::string m_error_info;
	ImGui::FileBrowser m_file_dialog;
	
	// Needed, so glfw could close window, when this window is closed
	bool* m_is_open;
};

