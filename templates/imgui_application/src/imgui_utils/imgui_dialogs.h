#ifndef __IMGUI_UTILS_DIALOGS_H__
#define __IMGUI_UTILS_DIALOGS_H__

#include "imgui_utilities.h"

#include <functional>
#include <stack>

namespace ImGuiUtils
    {

    //////////////////////////////////////////////////////////////////////////
    enum class DialogResult
        {
        None,
        Ok, 
        Cancel,
        Abort,
        Retry,
        Ignore,
        Yes,
        No,
        };

    class dialog;
    using dialog_ptr = dialog *;
    using dialog_sptr = std::shared_ptr<dialog>;
    using dialog_close_action = std::function<void(dialog_ptr, DialogResult)>;

    //////////////////////////////////////////////////////////////////////////
    class dialog_manager
        {
        public:
            virtual void manage_dialogs();

        protected:
            std::stack<dialog_sptr>      m_dialogs;
        };

    //////////////////////////////////////////////////////////////////////////
    class dialog: public dialog_manager
        {
        public:
            dialog(dialog_close_action action = nullptr);
            virtual ~dialog();

        public:
            virtual DialogResult show_modal();
            virtual void execute_dialog_close_action(DialogResult res);

        protected:
            virtual DialogResult show() = 0;

        protected:
            dialog_close_action           m_close_action;

        };



    }





#endif //__IMGUI_UTILS_DIALOGS_H__
