#include "main_frame.h"
#include <imgui_utilities.h>
#include "application.h"

//////////////////////////////////////////////////////////////////////////
main_frame::main_frame()
    {
    }

//////////////////////////////////////////////////////////////////////////
main_frame::~main_frame()
    {
    }

//////////////////////////////////////////////////////////////////////////
void main_frame::render()
    {
    /* Create main docking window */
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoTitleBar;

    const float oldWindowRounding = ImGui::GetStyle().WindowRounding;
    ImGui::GetStyle().WindowRounding = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainFrame", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    render_page();

    render_menu_bar();

    // restore rouding
    ImGui::GetStyle().WindowRounding = oldWindowRounding;
    ImGui::End();
    }

//////////////////////////////////////////////////////////////////////////
void main_frame::render_menu_bar()
    {
    if (ImGui::BeginMainMenuBar())
        {
        if (ImGui::BeginMenu("File"))
            {
            if (ImGui::MenuItemWithIcon((char *)ICON_MDI_ROCKET, "New Project..."))
                {
                }

            ImGui::Separator();

            if (ImGui::MenuItemWithIcon((char *)ICON_MDI_FOLDER_OPEN, "Open Project..."))
                {
                }

            if (ImGui::MenuItemWithIcon((char *)ICON_MDI_CONTENT_SAVE, "Save Project..."))
                {
                }

            if (ImGui::MenuItemWithIcon((char *)ICON_MDI_COGS, "Settings..."))
                {
                }

            if (ImGui::MenuItemWithIcon((char *)ICON_MDI_EXIT_TO_APP, "Exit"))
                {
                }

            ImGui::EndMenu();
            }

        ImGui::EndMainMenuBar();
        }
    }

//////////////////////////////////////////////////////////////////////////
void main_frame::render_page()
    {
    ImGuiWindowFlags child_window_flags = ImGuiWindowFlags_NoResize;
    if (ImGui::BeginChild("##filter", ImVec2(-1, 0), true, child_window_flags))
        {
        }
    ImGui::EndChild();
    }





