#pragma once

#include "common_includes.h"
#include <string_view>

//////////////////////////////////////////////////////////////////////////
struct app_settings
    {
    POINTER_TYPEDEFS(app_settings)

    struct ui_settings 
        {
        POINTER_TYPEDEFS(ui_settings)

        std::string     m_text_font_name;
        float           m_text_font_size;
        }   m_ui;

    };

//////////////////////////////////////////////////////////////////////////
struct app_settings_loader
    {
    static app_settings::uptr load(const std::string_view & name);
    };

//////////////////////////////////////////////////////////////////////////
struct app_settings_saver
    {
    static void save(const std::string_view & name, const app_settings & value);
    };
