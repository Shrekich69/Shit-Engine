#ifndef SH_ENGINE_HPP
#define SH_ENGINE_HPP

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

namespace shrk
{

   enum AppResult
   {
      APP_CONTINUE = 0,
      APP_FAILURE,
      APP_DONE
   };

   struct EngineOptions
   {
      // App metadata
      const char* appName       = "Shit Engine";
      const char* appVersion    = "0.0";
      const char* appIdentifier = "shitengine";
      
      // SDL option
      SDL_InitFlags initFlags = SDL_INIT_VIDEO;

      // Window options
      SDL_WindowFlags windowFlags = 0;
      int windowSize_x            = 1280;
      int windowSize_y            = 800;
      const char* windowTitle     = "Shit Engine 0.0";
      
      glm::vec4 clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
   };
   /* 
   *  Setup app metadata, window flags, window size, window title and clear color
   */
   EngineOptions EngineSetup();

   /* 
   *  Initialize here textures or models or something else
   */
   AppResult AppInit();
   /* 
   *  Process events
   */
   AppResult AppEvent(const SDL_Event &event);
   /* 
   *  The loop stuff
   */
   AppResult AppTick(const float &deltaTime);
   /* 
   *  Called on the exit of programm
   */
   AppResult AppQuit();

}

#endif // SH_ENGINE_HPP