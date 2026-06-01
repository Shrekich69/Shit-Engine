#include "utiles.hpp"

#include <unistd.h>

shrk::Config* shrk::loadConfig(const char* path)
{
   try
   {
      return nullptr;
   }
   catch(const std::exception &e)
   {
      std::cerr << "Failed to read config: " << e.what() << '\n';
      return nullptr;
   }
}

std::string shrk::readEntireFile(const char* path)
{
   try
   {
      std::ifstream file(path);

      std::stringstream data;
      data << file.rdbuf();
   
      file.close();
      return data.str();
   }
   catch(std::exception &e)
   {
      std::cerr << "Failed to read file: " << e.what() << std::endl;
      return "";
   }
}