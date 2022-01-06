#include "imgui_utilities.h"
#include <imgui_internal.h>


#include "IconsMaterialDesignIcons.h"

const unsigned short Icons::MinIcon = ICON_MIN_MDI;
const unsigned short Icons::MaxIcon = ICON_MAX_MDI;

const std::string Icons::FontsFilename = FONT_ICON_FILE_NAME_MDI;
// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

#if defined(__cpp_char8_t)
#   define CONVERT_CHAR(x) reinterpret_cast<const char *>(x)
#else
#   define CONVERT_CHAR(x) x
#endif

const std::string Icons::Project = CONVERT_CHAR(ICON_MDI_ROCKET);
const std::string Icons::Folder = CONVERT_CHAR(ICON_MDI_FOLDER);
const std::string Icons::File = CONVERT_CHAR(ICON_MDI_FILE_DOCUMENT);
const std::string Icons::ImageFile = CONVERT_CHAR(ICON_MDI_FILE_IMAGE);
const std::string Icons::SoundFile = CONVERT_CHAR(ICON_MDI_AUDIO_VIDEO);
const std::string Icons::AudioFile = CONVERT_CHAR(ICON_MDI_AUDIO_VIDEO);
const std::string Icons::ShaderFile = CONVERT_CHAR(ICON_MDI_CODE_BRACES);
const std::string Icons::FontFile = CONVERT_CHAR(ICON_MDI_FORMAT_FONT);
const std::string Icons::ConfigFile = CONVERT_CHAR(ICON_MDI_COGS);
const std::string Icons::Include = CONVERT_CHAR(ICON_MDI_COMMENT);
const std::string Icons::Model = CONVERT_CHAR(ICON_MDI_CODE_BRACKETS);
const std::string Icons::DataModel = CONVERT_CHAR(ICON_MDI_FLASH);
const std::string Icons::Link = CONVERT_CHAR(ICON_MDI_LINK);
const std::string Icons::Add = CONVERT_CHAR(ICON_MDI_PLUS);
const std::string Icons::Remove = CONVERT_CHAR(ICON_MDI_DELETE);
const std::string Icons::MoveUp = CONVERT_CHAR(ICON_MDI_CHEVRON_UP);
const std::string Icons::MoveDown = CONVERT_CHAR(ICON_MDI_CHEVRON_DOWN);
const std::string Icons::Config = CONVERT_CHAR(ICON_MDI_COGS);
const std::string Icons::Import = CONVERT_CHAR(ICON_MDI_DATABASE_IMPORT);
const std::string Icons::Rename = CONVERT_CHAR(ICON_MDI_RENAME_BOX);
const std::string Icons::Open = CONVERT_CHAR(ICON_MDI_FOLDER_OPEN);
const std::string Icons::Save = CONVERT_CHAR(ICON_MDI_CONTENT_SAVE);
const std::string Icons::Exit = CONVERT_CHAR(ICON_MDI_EXIT_TO_APP);
const std::string Icons::Redo = CONVERT_CHAR(ICON_MDI_REDO);
const std::string Icons::Undo = CONVERT_CHAR(ICON_MDI_UNDO);
const std::string Icons::Cut = CONVERT_CHAR(ICON_MDI_CONTENT_CUT);
const std::string Icons::Copy = CONVERT_CHAR(ICON_MDI_CONTENT_COPY);
const std::string Icons::Paste = CONVERT_CHAR(ICON_MDI_CONTENT_PASTE);
const std::string Icons::Information = CONVERT_CHAR(ICON_MDI_INFORMATION);
const std::string Icons::Question = CONVERT_CHAR(ICON_MDI_CLOUD_QUESTION);
const std::string Icons::Warning = CONVERT_CHAR(ICON_MDI_EXCLAMATION);
const std::string Icons::Error = CONVERT_CHAR(ICON_MDI_EMOTICON_SAD);
const std::string Icons::Start = CONVERT_CHAR(ICON_MDI_PLAY);
const std::string Icons::Stop = CONVERT_CHAR(ICON_MDI_STOP);
const std::string Icons::Number = CONVERT_CHAR(ICON_MDI_NUMERIC);
const std::string Icons::Discover = CONVERT_CHAR(ICON_MDI_PERIODIC_TABLE);

namespace ImGui
    {

    //////////////////////////////////////////////////////////////////////////
    bool MenuItemWithIcon(const char * icon, const char * label, const char * shortcut /*= NULL*/, bool selected /*= false*/, bool enabled /*= true*/)
        {
        ImGui::Text(icon); 
        ImGui::SameLine(); 
        return ImGui::MenuItem(label, shortcut, selected, enabled);
        }

    //////////////////////////////////////////////////////////////////////////
    bool MenuItemWithIcon(const char * icon, const char * label, const char * shortcut, bool * p_selected, bool enabled /*= true*/)
        {
        ImGui::Text(icon);
        ImGui::SameLine();
        return ImGui::MenuItem(label, shortcut, p_selected, enabled);
        }

    //////////////////////////////////////////////////////////////////////////
    bool TreeNodeWithIcon(const char * icon, const char * label)
        {
        return TreeNodeWithIcon(icon, label, label);
        }

    //////////////////////////////////////////////////////////////////////////
    bool TreeNodeWithIcon(const char * icon, const char * id, const char * label)
        {
        std::string nn = icon;
        nn += " ";
        nn += label;
        return ImGui::TreeNode(nn.c_str());
        }

    //////////////////////////////////////////////////////////////////////////
    bool BeginButtonDropDown(const char * button_label, const char * label)
        {
        ImGui::SameLine(0.f, 0.f);

        auto window = ImGui::GetCurrentWindow();
//         const ImGuiStyle & style = ImGui::Styg.Style;

        float x = ImGui::GetCursorPosX();
        float y = ImGui::GetCursorPosY();

        bool pressed = ImGui::Button(button_label);

        // Popup

        ImVec2 popupPos = GetCurrentContext()->LastItemData.Rect.GetBL();
        ImGui::SetNextWindowPos(popupPos);

        if (pressed)
            {
            ImGui::OpenPopup(label);
            }

        if (ImGui::BeginPopup(label))
            {
//             ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_Button]);
//             ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Colors[ImGuiCol_Button]);
//             ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_Button]);
            return true;
            }

        return false;
        }

    //////////////////////////////////////////////////////////////////////////
    void EndButtonDropDown()
        {
//        ImGui::PopStyleColor(3);
        ImGui::EndPopup();
        }


    }

