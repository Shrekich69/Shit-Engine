#include "sh_engine.hpp"

#include <cstdint>
#include <dearimgui/imgui.h>
#include <dearimgui/backends/imgui_impl_sdl3.h>
#include <dearimgui/backends/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

int main()
{
    shrk::EngineOptions engineOptions = shrk::EngineSetup();

    SDL_SetAppMetadata(engineOptions.appName, engineOptions.appVersion, engineOptions.appIdentifier);
    if (!SDL_Init(engineOptions.initFlags))
    {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const char* glsl_version = "#version 410 core";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags winflags = engineOptions.windowFlags;
    SDL_Window* window = SDL_CreateWindow(engineOptions.windowTitle, (int)(engineOptions.windowSize_x*main_scale), (int)(engineOptions.windowSize_y*main_scale), winflags);
    if (!window)
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
    {
        SDL_Log("Failed to create gl_context: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to init GLAD: %u", glGetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    glEnable(GL_DEPTH_TEST);

    // Create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    shrk::AppResult initRes = shrk::AppInit();
    if (initRes == shrk::APP_FAILURE)
        SDL_Log("Error has occurred in AppInit\n");

    // delta time
    uint64_t previousTime = SDL_GetTicksNS();
    float deltaTime = 0.0f;

    bool done = false;
    while (!done)
    {
        // Calculate delta time
        uint64_t currTime = SDL_GetTicksNS();
        deltaTime = static_cast<float>(currTime - previousTime) / 1'000'000'000.0f;

        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                done = true;
                
            shrk::AppResult eventRes = shrk::AppEvent(event);
            if (eventRes == shrk::APP_DONE)
                done = true;
            else if (eventRes == shrk::APP_FAILURE)
            {
                SDL_Log("Failed to process event in AppEvent()\n");
                continue;
            }
        }

        ImGui::GetIO().DeltaTime = deltaTime;

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (shrk::AppTick(deltaTime) != shrk::APP_CONTINUE)
            done = true;

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(
            engineOptions.clear_color.x * engineOptions.clear_color.w,
            engineOptions.clear_color.y * engineOptions.clear_color.w,
            engineOptions.clear_color.z * engineOptions.clear_color.w,
            engineOptions.clear_color.w
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    shrk::AppQuit();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}