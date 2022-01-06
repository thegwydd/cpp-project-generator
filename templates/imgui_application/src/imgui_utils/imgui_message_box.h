#ifndef __IMGUI_UTILS_MESSAGE_BOX_H__
#define __IMGUI_UTILS_MESSAGE_BOX_H__

#include "imgui_dialogs.h"

namespace ImGuiUtils
    {
    //////////////////////////////////////////////////////////////////////////
    enum class MessageBoxIcon
        {
        None,
        Info,
        Question,
        Warning,
        Error,
        Stop,
        };

    //////////////////////////////////////////////////////////////////////////
    enum class MessageBoxButton
        {
        Ok,
        OkCancel,
        YesNo,
        YesNoCancel,
        AbortRetry,
        AbortRetryIgnore,
        };

    //////////////////////////////////////////////////////////////////////////
    enum class MessageBoxResult
        {
        None,
        Ok,
        Cancel,
        Yes,
        No,
        Abort,
        Retry,
        Ignore,
        };

    //////////////////////////////////////////////////////////////////////////
    using message_box_icons = std::unordered_map<MessageBoxIcon, std::string>;

    //////////////////////////////////////////////////////////////////////////
    class message_box_dialog : public dialog
        {
        public:
            message_box_dialog(const char * title, const char * text, MessageBoxIcon icon, MessageBoxButton buttons, const message_box_icons & icons, dialog_close_action action = nullptr);
            virtual ~message_box_dialog();

        public:
            virtual DialogResult show() override;
            virtual MessageBoxResult result() { return m_Result; }

        public:
            std::string m_Filename;

        protected:
            std::string         m_Title;
            std::string         m_Text;
            MessageBoxIcon      m_Icon;
            MessageBoxButton    m_Buttons;
            MessageBoxResult    m_Result;
            message_box_icons   m_Icons;
        };

    //////////////////////////////////////////////////////////////////////////
    class message_box
        {
        public:
            static MessageBoxResult show(const char * title, const char * text, MessageBoxIcon icon, MessageBoxButton buttons, const message_box_icons & icons);
        };


    }





#endif //__IMGUI_UTILS_MESSAGE_BOX_H__
