#ifndef __IMGUI_UTILITIES_H__
#define __IMGUI_UTILITIES_H__

#include <imgui.h>

#include <IconsMaterialDesignIcons.h>
#include "imgui_toolbar.h"
#include "imgui_stdlib.h"
#include "imgui_dialogs.h"

#include <functional>

struct Icons
    {
    static const unsigned short MinIcon;
    static const unsigned short MaxIcon;
    static const std::string FontsFilename;

    static const std::string Project;
    static const std::string Folder;
    static const std::string File;
    static const std::string ImageFile;
    static const std::string SoundFile;
    static const std::string AudioFile;
    static const std::string ShaderFile;
    static const std::string FontFile;
    static const std::string ConfigFile;
    static const std::string Include;
    static const std::string Model;
    static const std::string DataModel;
    static const std::string Link;
    static const std::string Add;
    static const std::string Remove;
    static const std::string MoveUp;
    static const std::string MoveDown;
    static const std::string Config;
    static const std::string Import;
    static const std::string Rename;
    static const std::string Open;
    static const std::string Save;
    static const std::string Exit;
    static const std::string Redo;
    static const std::string Undo;
    static const std::string Cut;
    static const std::string Copy;
    static const std::string Paste;
    static const std::string Information;
    static const std::string Question;
    static const std::string Warning;
    static const std::string Error;
    static const std::string Start;
    static const std::string Stop;
    static const std::string Number;
    static const std::string Discover;
    };

namespace ImGui
    {
    // Shows a menu item with icon drawn as a text item
    bool MenuItemWithIcon(const char * icon, const char * label, const char * shortcut, bool * p_selected, bool enabled = true);
    // Shows a menu item with icon drawn as a text item
    bool MenuItemWithIcon(const char * icon, const char * label, const char * shortcut = NULL, bool selected = false, bool enabled = true);

    using TreeItemOpenHandler = std::function<void()>;
    using TreeItemContextMenuHandler = std::function<void()>;

    bool TreeNodeWithIcon(const char * icon, const char * label);
    bool TreeNodeWithIcon(const char * icon, const char * id, const char * label);

    bool BeginButtonDropDown(const char * button_label, const char * label);
    void EndButtonDropDown();


    }





#endif //__IMGUI_UTILITIES_H__
