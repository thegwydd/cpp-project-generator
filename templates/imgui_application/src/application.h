#pragma once

#include "imgui_control.h"
#include "imgui_style.h"
#include "app_settings.h"

#include <filesystem>

//////////////////////////////////////////////////////////////////////////
class application
    {
    private:
        application();
        application(application const &) = delete;    // Don't Implement
        void operator=(application const &) = delete; // Don't implement

    public:
        static application & get_insance();
        app_settings::ptr get_settings();

        int run(int argc, char ** argv);

    protected:
        void initialize_logger();
        void deinitialize_logger();

        void initialize_gui_system();
        void deinitialize_gui_system();

        void initialize_imgui();
        void deinitialize_imgui();

        void initialize_fonts();
        void deinitialize_fonts();

        void load_settings();
        void save_settings();

        void initialize_main_frame();
        void deinitialize_main_frame();

        void render_cycle();

    protected:
        std::filesystem::path           m_executable_path;
        void *                          m_window;
        const char *                    m_glsl_version;
        app_settings::uptr              m_app_settings;
        ImGuiUtils::ui_control::sptr    m_main_frame;

    };
