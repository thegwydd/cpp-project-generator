/*
The MIT License (MIT)
Copyright (c) 2016 Andre Leiradella
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "imgui_message_box.h"
#include "imgui_utilities.h"

namespace ImGuiUtils
    {

    //////////////////////////////////////////////////////////////////////////
    message_box_dialog::message_box_dialog(const char * title, const char * text, MessageBoxIcon icon, MessageBoxButton buttons, const message_box_icons & icons, dialog_close_action action)
        : dialog(action)
        , m_Title(title)
        , m_Text(text)
        , m_Icon(icon)
        , m_Buttons(buttons)
        , m_Icons(icons)
        {
        }

    //////////////////////////////////////////////////////////////////////////
    message_box_dialog::~message_box_dialog()
        {
        }

    //////////////////////////////////////////////////////////////////////////
    DialogResult message_box_dialog::show()
        {
        DialogResult ret = DialogResult::None;

        m_Result = message_box::show(m_Title.c_str(), m_Text.c_str(), m_Icon, m_Buttons, m_Icons);
        if (m_Result != MessageBoxResult::None)
            ret = DialogResult::Ok;

        return ret;
        }


    //////////////////////////////////////////////////////////////////////////
    MessageBoxResult message_box::show(const char * title, const char * text, MessageBoxIcon icon, MessageBoxButton buttons, const message_box_icons & icons)
        {
        MessageBoxResult ret = MessageBoxResult::None;

        ImVec2 cur_wnd_size = ImGui::GetIO().DisplaySize;
        ImVec2 center(cur_wnd_size.x / 2, cur_wnd_size.y / 2);

        ImGui::OpenPopup(title);
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_Always);
        if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
            std::string icon_char;

            auto fnd = icons.find(icon);
            if (fnd != icons.end())
                icon_char = fnd->second;

            if (!icon_char.empty())
                {
                ImVec2 size = ImGui::CalcTextSize(icon_char.c_str());
                ImVec2 pos = ImGui::GetCursorPos();
                float save_y = pos.y;
                pos.y += size.y;

                ImGui::SetCursorPos(pos);
                ImGui::Text("%s", icon_char.c_str());

                pos.x += size.x + pos.x;
                pos.y = save_y;

                ImGui::SetCursorPos(pos);
                ImGui::TextWrapped("%s", text);
                }
            else
                {
                ImGui::TextWrapped("%s", text);
                }

            ImGui::Separator();

            ImVec2 size = ImVec2(50.0f, 0.0f);
            int count = 0;

            switch (buttons)
                {
                case MessageBoxButton::Ok:
                    if (ImGui::Button("Ok", size)) { ret = MessageBoxResult::Ok; }
                    count = 1;
                    break;
                case MessageBoxButton::OkCancel:
                    if (ImGui::Button("Ok", size)) { ret = MessageBoxResult::Ok; }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", size)) { ret = MessageBoxResult::Cancel; }
                    count = 2;
                    break;
                case MessageBoxButton::YesNo:
                    if (ImGui::Button("Yes", size)) { ret = MessageBoxResult::Yes; }
                    ImGui::SameLine();
                    if (ImGui::Button("No", size)) { ret = MessageBoxResult::No; }
                    count = 2;
                    break;
                case MessageBoxButton::YesNoCancel:
                    if (ImGui::Button("Yes", size)) { ret = MessageBoxResult::Yes; }
                    ImGui::SameLine();
                    if (ImGui::Button("No", size)) { ret = MessageBoxResult::No; }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", size)) { ret = MessageBoxResult::Cancel; }
                    count = 3;
                    break;
                case MessageBoxButton::AbortRetry:
                    if (ImGui::Button("Abort", size)) { ret = MessageBoxResult::Abort; }
                    ImGui::SameLine();
                    if (ImGui::Button("Retry", size)) { ret = MessageBoxResult::Retry; }
                    count = 2;
                    break;
                case MessageBoxButton::AbortRetryIgnore:
                    if (ImGui::Button("Abort", size)) { ret = MessageBoxResult::Abort; }
                    ImGui::SameLine();
                    if (ImGui::Button("Retry", size)) { ret = MessageBoxResult::Retry; }
                    ImGui::SameLine();
                    if (ImGui::Button("Ignore", size)) { ret = MessageBoxResult::Ignore; }
                    count = 3;
                    break;
                default:
                    break;
                }

            if (count)
                {
                size = ImVec2((4 - count) * 50.0f, 1.0f);
                ImGui::Dummy(size);
                }

            ImGui::EndPopup();
            }

        return ret;
        }

    }