#include "engine/graphics.hpp"
#include "engine/utiles.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

#include <cstddef>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint shrk::compileShader(const char* vertShaderPath, const char* fragShaderPath)
{
   std::string vertShaderCode = shrk::readEntireFile(vertShaderPath);
   std::string fragShaderCode = shrk::readEntireFile(fragShaderPath);
   const char* vertShaderCCode = vertShaderCode.c_str();
   const char* fragShaderCCode = fragShaderCode.c_str();

   int success = 0;
   char infoLog[512];

   GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertShader, 1, &vertShaderCCode, nullptr);
   glCompileShader(vertShader);
   // Check for vertex shader compilation errors if any
   glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
      SDL_Log("Vertex shader compilation error: %s", infoLog);
   }

   GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragShader, 1, &fragShaderCCode, nullptr);
   glCompileShader(fragShader);
   // Check for fragment shader compilation errors if any
   glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
      SDL_Log("Fragment shader compilation error: %s", infoLog);
   }

   GLuint id = glCreateProgram();
   glAttachShader(id, vertShader);
   glAttachShader(id, fragShader);
   glLinkProgram(id);
   // Get program errors
   glGetProgramiv(id, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(id, 512, nullptr, infoLog);
      SDL_Log("Program linking error: %s", infoLog);
   }

   glDeleteShader(vertShader);
   glDeleteShader(fragShader);

   return id;
}

GLuint shrk::loadTexture(const char* path)
{
   int width, height, channels;
   u_char* data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
   if (!data)
   {
      std::cerr << "Failed to load image at: " << path << std::endl;
      stbi_image_free(data);
   }

   GLuint texID;
   glGenTextures(1, &texID);
   glBindTexture(GL_TEXTURE_2D, texID);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

   stbi_image_free(data);

   return texID;
}

shrk::Entity::Entity(GLuint shaderID, GLuint textureID)
{
   sprite.shaderID = shaderID;
   sprite.textureID = textureID;

   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(shrk::Vertex), &vertices[0], GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint8_t), &indices[0], GL_STATIC_DRAW);

   // position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(shrk::Vertex), (void*)0);
   glEnableVertexAttribArray(0);
   // texture coords
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(shrk::Vertex), (void*)offsetof(shrk::Vertex, texCoord));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
}

void shrk::Entity::render() const
{
   glUseProgram(sprite.shaderID);
   glActiveTexture(GL_TEXTURE);
   glBindTexture(GL_TEXTURE_2D, sprite.textureID);

   glm::mat4 model = glm::translate(glm::mat4(1.0f), sprite.pos);
   model *= glm::scale(glm::mat4(1.0f), glm::vec3{sprite.scale, 0.0f});
   model *= glm::rotate(glm::mat4(1.0f), sprite.rotation, glm::vec3{0.0f, 0.0f, 1.0f});
   GLint uni_model = glGetUniformLocation(sprite.shaderID, "model");
   glUniformMatrix4fv(uni_model, 1, GL_FALSE, &model[0][0]);

   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
   glBindVertexArray(0);
}