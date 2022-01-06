#include "application.h"
#include "imgui_app_version.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#ifdef _MSC_VER
#include <spdlog/sinks/msvc_sink.h>
#endif

#include "main_frame.h"


//////////////////////////////////////////////////////////////////////////
static void glfw_error_callback(int error, const char * description)
    {
    spdlog::error("Glfw Error {}: {}", error, description);
    }

//////////////////////////////////////////////////////////////////////////
application::application()
    {
    }

//////////////////////////////////////////////////////////////////////////
application & application::get_insance()
	{
	static application _the_app;
	return _the_app;
	}

//////////////////////////////////////////////////////////////////////////
app_settings::ptr application::get_settings()
	{
	return m_app_settings.get();
	}

//////////////////////////////////////////////////////////////////////////
int application::run(int argc, char ** argv)
    {
    if (argc)
		m_executable_path = std::filesystem::path(argv[0]);
	else
		return -1;

	using run_raii_item = raii_init_deinit_wrapper<std::function<void()>, std::function<void()>>;
	
	try
		{
		run_raii_item logger([this](){ return initialize_logger(); }, [this](){ return deinitialize_logger(); });
		run_raii_item settings([this]() { return load_settings(); }, [this]() { return save_settings(); });
		run_raii_item gui_system([this]() { return initialize_gui_system(); }, [this]() { return deinitialize_gui_system(); });
		run_raii_item imgui_library([this]() { return initialize_imgui(); }, [this]() { return deinitialize_imgui(); });
		run_raii_item fonts([this]() { return initialize_fonts(); }, [this]() { return deinitialize_fonts(); });
		run_raii_item main_frame([this]() { return initialize_main_frame(); }, [this]() { return deinitialize_main_frame(); });
		render_cycle();
		}
	catch (std::exception & ex)
		{
		spdlog::critical("Exception in {}: {}", FUNCTION_NAME, ex.what());
		return 1;
		}

	return 0;
	}

//////////////////////////////////////////////////////////////////////////
void application::initialize_logger()
	{
#ifdef _MSC_VER
	auto default_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
	spdlog::set_default_logger(std::make_shared<spdlog::logger>("<PNAME>", spdlog::sinks_init_list() = { default_sink }));
#endif

	spdlog::set_level(spdlog::level::debug);
	}

//////////////////////////////////////////////////////////////////////////
void application::deinitialize_logger()
	{
	// Under VisualStudio, this must be called before main finishes to workaround a known VS issue
	spdlog::drop_all();
	}

//////////////////////////////////////////////////////////////////////////
void application::initialize_gui_system()
	{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		throw std::runtime_error("Failed to call glfwInit");

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	m_glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#else
	// GL 3.0 + GLSL 130
	m_glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
    std::string glfw_win_caption = std::format("{} {}.{}.{}", "<PNAME>", <#PNAME#>_VERSION_MAJOR, <#PNAME#>_VERSION_MINOR, <#PNAME#>_VERSION_PATCH);
	m_window = glfwCreateWindow(1280, 720, glfw_win_caption.c_str(), NULL, NULL);
	if (m_window == nullptr)
		throw std::runtime_error("Failed to call glfwCreateWindow");

	glfwMakeContextCurrent(reinterpret_cast<GLFWwindow *>(m_window));
	glfwSwapInterval(1); // Enable vsync

	bool err = glewInit() != GLEW_OK;
	if (err)
		throw std::runtime_error("Failed to initialize OpenGL loader");

	int screen_width, screen_height;
	glfwGetFramebufferSize(reinterpret_cast<GLFWwindow *>(m_window), &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);
	}

//////////////////////////////////////////////////////////////////////////
void application::deinitialize_gui_system()
	{
	glfwDestroyWindow(reinterpret_cast<GLFWwindow *>(m_window));
	glfwTerminate();
	}

//////////////////////////////////////////////////////////////////////////
void application::initialize_imgui()
	{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow *>(m_window), true);
	ImGui_ImplOpenGL3_Init(m_glsl_version);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	apply_style();
	}

//////////////////////////////////////////////////////////////////////////
void application::deinitialize_imgui()
	{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	}

//////////////////////////////////////////////////////////////////////////
void application::initialize_fonts()
	{
	ImGuiIO & io = ImGui::GetIO();
	//        io.Fonts->AddFontDefault();
	std::string font_file = (m_executable_path.parent_path() / "fonts" / m_app_settings->m_ui.m_text_font_name).string();
	io.Fonts->AddFontFromFileTTF(font_file.c_str(), m_app_settings->m_ui.m_text_font_size);

	// merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	font_file = (m_executable_path.parent_path() / "fonts" / FONT_ICON_FILE_NAME_MDI).string();
	io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16.0f, &icons_config, icons_ranges);
	}

//////////////////////////////////////////////////////////////////////////
void application::deinitialize_fonts()
	{
	}

//////////////////////////////////////////////////////////////////////////
void application::load_settings()
	{
	m_app_settings = app_settings_loader::load("./settings.json");
	}

//////////////////////////////////////////////////////////////////////////
void application::save_settings()
	{
	app_settings_saver::save("./settings.json", *m_app_settings);
	}

//////////////////////////////////////////////////////////////////////////
void application::initialize_main_frame()
	{
	m_main_frame = std::make_shared<main_frame>();
	}

//////////////////////////////////////////////////////////////////////////
void application::deinitialize_main_frame()
	{
	m_main_frame = nullptr;
	}

//////////////////////////////////////////////////////////////////////////
void application::render_cycle()
	{
	ImGuiIO & io = ImGui::GetIO();
	auto wnd = reinterpret_cast<GLFWwindow *>(m_window);
	while (!glfwWindowShouldClose(wnd))
		{
		glfwPollEvents();
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(io.DisplaySize);

		if (m_main_frame)
			m_main_frame->render();

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		int display_w, display_h;
		glfwGetFramebufferSize(wnd, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glfwSwapBuffers(wnd);
		}
	}
