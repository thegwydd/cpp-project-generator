#include "imgui_dialogs.h"

namespace ImGuiUtils
    {

    //////////////////////////////////////////////////////////////////////////
    void dialog_manager::manage_dialogs()
        {
        dialog_sptr dlg;
        if (!m_dialogs.empty())
            dlg = m_dialogs.top();

        if (dlg) 
            {
            DialogResult dlg_res = dlg->show_modal();
            if (dlg_res != DialogResult::None)
                {
                dlg->execute_dialog_close_action(dlg_res);
                m_dialogs.pop();
                }
            }
        }




    //////////////////////////////////////////////////////////////////////////
    dialog::dialog(dialog_close_action action)
        : m_close_action(action)
        {}

    //////////////////////////////////////////////////////////////////////////
    dialog::~dialog()
        {}

    //////////////////////////////////////////////////////////////////////////
    DialogResult dialog::show_modal()
        {
        DialogResult ret = DialogResult::None;

        if (!m_dialogs.empty())
            manage_dialogs();
        else
            ret = show();

        return ret;
        };

    //////////////////////////////////////////////////////////////////////////
    void dialog::execute_dialog_close_action(DialogResult res)
        { 
        if (m_close_action)
            m_close_action(this, res);
        }



    }