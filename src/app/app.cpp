#include "app.h"
#include "imgui/imgui.h"
#include <iostream>

App::App(bool* is_open)
	: m_is_open{ is_open }, m_file_dialog(ImGuiFileBrowserFlags_EditPathString)
{
	m_file_dialog.SetTypeFilters({ ".gpx" });
}

void App::getGui()
{
	if (!ImGui::Begin("Gpx Speed Editor", m_is_open)) {
		ImGui::End();// If window is closed, m_is_open will be set to false
		return;
	}

	if (ImGui::Button("Choose File")) {
		m_file_dialog.Open();
	}
	ImGui::SameLine();
	std::string selected_path = m_file_dialog.GetSelected().string();
	if (ImGui::Button("Load File")) {
		if (isValidExt(selected_path)) {
			m_speed_modifier.loadPoints(m_file_adapter.readFile(selected_path));
		}
	}
	m_file_dialog.Display();

	ImGui::TextWrapped("%s",selected_path.c_str());
	ImGui::SetItemTooltip("Selected file path");
	if (ImGui::BeginPopup("Wrong file extension")) {
		ImGui::Text("File extention is not .gpx");
		ImGui::EndPopup();
	}

	ImGui::SeparatorText("Graph");
	m_speed_modifier.getGui();
	if (ImGui::Button("Save file")) {
		m_file_adapter.changeFile(m_speed_modifier.getPoints(), selected_path);
	}
	ImGui::End();
}

bool App::isValidExt(std::string file_name)
{
	if (file_name.ends_with(".gpx")) {
		return true;
	}
	ImGui::OpenPopup("Wrong file extension");
	return false;
}
