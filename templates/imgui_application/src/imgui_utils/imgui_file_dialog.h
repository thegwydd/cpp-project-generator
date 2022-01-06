#ifndef __IMGUI_FILE_DIALOG_H_
#define __IMGUI_FILE_DIALOG_H_

#include <vector>
#include <string>
#include <filesystem>

#define MAX_FILE_DIALOG_NAME_BUFFER 1024

//////////////////////////////////////////////////////////////////////////
enum class ImGuiFileDialogFlags
    {
    SelectFiles,
    SelectFolder,
    };

//////////////////////////////////////////////////////////////////////////
struct file_info
    {
    std::filesystem::path           FilePath;
    std::size_t                     FileSize;
    std::string                     Modified;
    };

//////////////////////////////////////////////////////////////////////////
class imgui_file_dialog
    {
    public:
        static imgui_file_dialog * Instance()
            {
            static imgui_file_dialog * _instance = new imgui_file_dialog();
            return _instance;
            }

    public:
        imgui_file_dialog();
        virtual ~imgui_file_dialog();

        bool file_dialog(const char * vName, const char * vFilters = nullptr, const char * vPath = std::filesystem::current_path().string().c_str(), const char * vDefaultFileName = nullptr, ImGuiFileDialogFlags flags = ImGuiFileDialogFlags::SelectFiles);
        std::filesystem::path GetCurrentFolder() { return m_CurrentFolder; }
        std::filesystem::path GetCurrentFile() { return m_CurrentFile; }
        std::string GetCurrentFilter() { return m_CurrentFilterExt; }

    protected:
        virtual void RenderFoldersRow();
        virtual void RenderFolderContent();
        virtual void RenderSelection(const char * vDefaultFileName, const char * vFilters);
        virtual bool RenderButtons();

        void Reset(bool all);
        void ChangeFolder(std::string vPath);

    private:
        std::vector<file_info>       m_FileList;
        std::filesystem::path       m_CurrentFolder;
        std::filesystem::path       m_CurrentFile;
        std::string                 m_CurrentFilterExt;
        ImGuiFileDialogFlags        m_Flags = ImGuiFileDialogFlags::SelectFiles;

    public:
        int                         FilterIndex = 0;
        bool                        IsOk = false;
        bool                        m_RefreshCache = true;

    };


#include "imgui_dialogs.h"

namespace ImGuiUtils
    {

    //////////////////////////////////////////////////////////////////////////
    class file_dialog : public dialog
        {
        public:
            file_dialog(const std::string & title, const std::string & filters, dialog_close_action action = nullptr);
            virtual ~file_dialog();

        public:
            virtual DialogResult show() override;

        public:
            std::string m_Filename;

        protected:
            std::string m_Title;
            std::string m_Filters;
        };


    }



#endif // __IMGUI_FILE_DIALOG_H_