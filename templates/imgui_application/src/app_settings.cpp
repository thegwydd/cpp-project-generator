#include "app_settings.h"
#include <json/json.h>
#include <fstream>

#define DEFAULT_TEXT_FONT_FILENAME	    "CONSOLA.ttf"
#define DEFAULT_TEXT_FONT_SIZE		    14


//////////////////////////////////////////////////////////////////////////
app_settings::uptr app_settings_loader::load(const std::string_view & name)
    {
    app_settings::uptr ret = std::make_unique<app_settings>();

    Json::Value root;
    std::ifstream file(name.data());
    file >> root;

    ret->m_ui.m_text_font_name = root["ui"]["text_font_name"].asString();
    ret->m_ui.m_text_font_size = root["ui"]["text_font_size"].asFloat();

    return ret;
    }

//////////////////////////////////////////////////////////////////////////
void app_settings_saver::save(const std::string_view & name, const app_settings & value)
    {
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = ""; //The JSON document is written in a single line

    Json::Value root;
    root["ui"]["text_font_name"] = value.m_ui.m_text_font_name;
    root["ui"]["text_font_size"] = value.m_ui.m_text_font_size;

    std::ofstream file(name.data());
    file << root;

    }

