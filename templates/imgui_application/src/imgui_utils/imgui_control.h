#ifndef __CONTROL_CONTROL_H__
#define __CONTROL_CONTROL_H__

#include "imgui_utilities.h"
#include "common_includes.h"

#include <format>

namespace ImGuiUtils
	{

    //////////////////////////////////////////////////////////////////////////
    class ui_control : public dialog_manager
        {
        public:
            POINTER_TYPEDEFS(ui_control)

            ui_control() : m_ThisName(std::format("{:#08X}", (size_t)this)) {}
            virtual ~ui_control() {}

        public:
            virtual void render() = 0;

        protected:
            std::string                 m_ThisName;
        };

	}


#endif //__CONTROL_CONTROL_H__
