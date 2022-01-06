#pragma once

#include "imgui_control.h"
#include <filesystem>

//////////////////////////////////////////////////////////////////////////
class main_frame : public ImGuiUtils::ui_control
    {
    public:
        POINTER_TYPEDEFS(main_frame)

        main_frame();
        ~main_frame();

    public:
        void render() override;

    private:
        void render_menu_bar();
        void render_page();
    
    };