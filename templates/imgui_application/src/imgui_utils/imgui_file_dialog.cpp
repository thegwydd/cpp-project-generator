#include "imgui_file_dialog.h"
#include "imgui_utilities.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <algorithm>
#include <chrono>

//////////////////////////////////////////////////////////////////////////
imgui_file_dialog::imgui_file_dialog()
    {}

//////////////////////////////////////////////////////////////////////////
imgui_file_dialog::~imgui_file_dialog()
    {}

//////////////////////////////////////////////////////////////////////////
void imgui_file_dialog::Reset(bool all)
    {
    if (all)
        {
        m_CurrentFile.clear();
        m_CurrentFolder.clear();
        m_CurrentFilterExt.clear();
        }

    m_FileList.clear();
    FilterIndex = 0;
    IsOk = false;
    }

//////////////////////////////////////////////////////////////////////////
template <typename TP>
std::time_t to_time_t(TP tp)
    {
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
    }
//////////////////////////////////////////////////////////////////////////
void imgui_file_dialog::ChangeFolder(std::string vPath)
    {
    // get currentPath
    std::filesystem::path cur_path(vPath);
    if (m_CurrentFolder != vPath)
        {
        try
            {
            std::vector<file_info> new_list;
            /* Scan files in directory */
            for (auto & entry : std::filesystem::directory_iterator(cur_path))
                {
                file_info finfo;
                finfo.FilePath = entry;
                if (entry.is_regular_file())
                    finfo.FileSize = std::filesystem::file_size(entry);
                else
                    finfo.FileSize = 0;

                auto timeEntry = std::filesystem::last_write_time(entry);
                std::time_t cftime = to_time_t(timeEntry);
                finfo.Modified = std::asctime(std::localtime(&cftime));

                new_list.push_back(finfo);
                }

            m_CurrentFolder = cur_path.string();
            m_FileList = new_list;

            std::sort(m_FileList.begin(), m_FileList.end(), [](file_info & a, file_info & b) {
                if (std::filesystem::is_directory(a.FilePath) && (std::filesystem::is_directory(b.FilePath)))
                    return (a.FilePath < b.FilePath);

                if (std::filesystem::is_directory(a.FilePath) && (!std::filesystem::is_directory(b.FilePath)))
                    return true;

                if (!std::filesystem::is_directory(a.FilePath) && (std::filesystem::is_directory(b.FilePath)))
                    return false;

                return (a.FilePath < b.FilePath);
                });

            }
        catch (...)
            {
            // maybe access denied or something else
            }
        }
    }

//////////////////////////////////////////////////////////////////////////
bool imgui_file_dialog::file_dialog(const char * vName, const char * vFilters, const char * vPath, const char * vDefaultFileName, ImGuiFileDialogFlags flags)
    {
    bool res = false;

    m_Flags = flags;

    IsOk = false;
    ImGui::OpenPopup(vName);
    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_Once);
    ImGui::BeginPopupModal(vName);

    if (vPath == 0) 
        vPath = std::filesystem::current_path().string().c_str();

    // did we cache any file? if not it means we're opening the dialog...
    if (m_RefreshCache)
        {
        Reset(true);
        // scan the given path
        ChangeFolder(vPath);

        if (vDefaultFileName)
            m_CurrentFile = m_CurrentFolder / vDefaultFileName;

        m_RefreshCache = false;
        }

    RenderFoldersRow();
    ImGui::Separator();
    RenderFolderContent();
    ImGui::Separator();
    RenderSelection(vDefaultFileName, vFilters);
    ImGui::Separator();
    res = RenderButtons();

    ImGui::EndPopup();

    return res;
    }

//////////////////////////////////////////////////////////////////////////
void imgui_file_dialog::RenderFoldersRow()
    {
    // show current path
    // <path>/<to>/<current>/<folder>
    bool pathClick = false;
    std::filesystem::path cpath = *m_CurrentFolder.begin();
    for (auto itPathDecomp = m_CurrentFolder.begin(); itPathDecomp != m_CurrentFolder.end(); ++itPathDecomp)
        {
        if (itPathDecomp != m_CurrentFolder.begin())
            {
            ImGui::SameLine();
            cpath /= *itPathDecomp;
            }

        if (ImGui::Button((*itPathDecomp).string().c_str()))
            {
            ChangeFolder(cpath.string());
            pathClick = true;
            break;
            }
        }
    }

//////////////////////////////////////////////////////////////////////////
void imgui_file_dialog::RenderFolderContent()
    {
    ImVec2 size = ImGui::GetContentRegionMax() - ImVec2(0.0f, 120.0f);

    ImGui::BeginChild("##FileDialog_FileList", size);

    ImGui::Columns(3, "mycolumns"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Size"); ImGui::NextColumn();
    ImGui::Text("Modified"); ImGui::NextColumn();
    ImGui::Separator();

    // show a row of folders
    if (ImGui::Selectable(".."))
        ChangeFolder(m_CurrentFolder.parent_path().string());
    ImGui::NextColumn();
    ImGui::NextColumn();
    ImGui::NextColumn();

    /* show all files */
    for (auto & finfo : m_FileList)
        {
        bool show = false;

        std::string str;

        auto & path = finfo.FilePath;
        if (std::filesystem::is_directory(path))
            {
            str = Icons::Folder + path.filename().string();
            show = true;
            }
        if (std::filesystem::is_regular_file(path) && (m_Flags != ImGuiFileDialogFlags::SelectFolder))
            {
            str = Icons::File + path.filename().string();
            if (m_CurrentFilterExt != ".*")
                show = !((m_CurrentFilterExt.size() > 0 && path.extension().string() != m_CurrentFilterExt));
            else
                show = true;
            }

        if (show == true)
            {
            // show a row of folders
            if (ImGui::Selectable(str.c_str(), (path == m_CurrentFile)))
                {
                // if we clicked a folder, we change to see it
                if (std::filesystem::is_directory(path))
                    ChangeFolder(path.string());
                else
                    m_CurrentFile = path;
                break;
                }

            ImGui::NextColumn();
            if (std::filesystem::is_regular_file(path))
                {
                ImGui::Text("%d bytes", finfo.FileSize);
                ImGui::NextColumn();
                }
            else
                ImGui::NextColumn();

            ImGui::Text(finfo.Modified.c_str()); ImGui::NextColumn();
            }
        }
    ImGui::EndChild();
    }

//////////////////////////////////////////////////////////////////////////
void imgui_file_dialog::RenderSelection(const char * vDefaultFileName, const char * vFilters)
    {
    ImGui::Text("File Name : ");

    ImGui::SameLine();

    std::string filename;
    if (m_Flags == ImGuiFileDialogFlags::SelectFiles)
        {
        if (m_CurrentFile.empty())
            {
            if (vDefaultFileName)
                filename = vDefaultFileName;
            }
        else
            filename = m_CurrentFile.filename().string();
        }
    else
        {
        if (!m_CurrentFolder.empty())
            filename = m_CurrentFolder.string();
        vFilters = nullptr;
        }

    float width = ImGui::GetContentRegionAvailWidth();
    
    // make room for filters
    if (vFilters != 0) 
        width -= 120.0f;

    ImGui::PushItemWidth(width);
    if (ImGui::InputText("##FileName", &filename))
        m_CurrentFile = m_CurrentFolder / filename;

    ImGui::PopItemWidth();

    if (vFilters != 0)
        {
        ImGui::SameLine();

        ImGui::PushItemWidth(100.0f);
        bool comboClick = ImGui::Combo("##Filters", &FilterIndex, vFilters) || m_CurrentFilterExt.size() == 0;
        ImGui::PopItemWidth();
        if (comboClick == true)
            {
            int itemIdx = 0;
            const char * p = vFilters;
            while (*p)
                {
                if (FilterIndex == itemIdx)
                    {
                    m_CurrentFilterExt = std::string(p);
                    break;
                    }
                p += strlen(p) + 1;
                itemIdx++;
                }
            }
        }
    }

//////////////////////////////////////////////////////////////////////////
bool imgui_file_dialog::RenderButtons()
    {
    if (ImGui::Button("Cancel"))
        {
        Reset(false);
        IsOk = false;
        m_RefreshCache = true;
        return true;
        }

    ImGui::SameLine();

    if (ImGui::Button("Ok"))
        {
        Reset(false);
        IsOk = true;
        m_RefreshCache = true;
        return true;
        }

    return false;
    }





#include <filesystem>

namespace ImGuiUtils
    {

    //////////////////////////////////////////////////////////////////////////
    file_dialog::file_dialog(const std::string & title, const std::string & filters, dialog_close_action action)
        : dialog(action)
        , m_Title(title)
        , m_Filters(filters)
        {

        }

    //////////////////////////////////////////////////////////////////////////
    file_dialog::~file_dialog()
        {

        }

    //////////////////////////////////////////////////////////////////////////
    DialogResult file_dialog::show()
        {
        DialogResult ret = DialogResult::None;

//         ImGui::PushStyleColor(ImGuiCol_WindowBg, ApplicationData::Instance().m_OriginalThemeWindowBgColor);
//         ImGui::PushStyleColor(ImGuiCol_ChildBg, ApplicationData::Instance().m_OriginalThemeChildBgColor);

        if (imgui_file_dialog::Instance()->file_dialog(m_Title.c_str(), m_Filters.c_str()))
            {
            if (imgui_file_dialog::Instance()->IsOk)
                {
                m_Filename = imgui_file_dialog::Instance()->GetCurrentFile().string();
                ret = DialogResult::Ok;
                }
            else
                ret = DialogResult::Cancel;
            }

//         ImGui::PopStyleColor();
//         ImGui::PopStyleColor();

        return ret;
        }

    }