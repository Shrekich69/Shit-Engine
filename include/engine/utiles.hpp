#ifndef UTILES_HPP
#define UTILES_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace shrk
{

   struct Config
   {
      int width = 1280;
      int height = 800;
   };
   Config* loadConfig(const char* path);

   std::string readEntireFile(const char* path);

}

#endif // UTILES_HPP
