#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace shrk
{
   /* 
   *  Compiles shaders and links a program by giving it path to the vertex and fragment shader
   */
   GLuint compileShader(const char* vertShaderPath, const char* fragShaderPath);

   /* 
   *  Loads texture
   */
   GLuint loadTexture(const char* path);

   /* 
   *  
   */
   struct Renderable
   {
      virtual void render() const;
   };

   /* 
   *
   */
   struct Transform
   {
      glm::vec3 pos  {0.0f};
      glm::vec2 scale{1.0f};
      float rotation = 0.0f;
   };
   /*
   *  
   */
   struct Hitbox
   {
      glm::vec2 hitbox_offset{0.0f};
      glm::vec2 hitbox_scale {1.0f};
   };

   /* 
   *  Usual sprite.
   *  Contains position and hitbox
   */
   struct Sprite : public Transform, public Hitbox
   {
      GLuint shaderID = 0;
      GLuint textureID = 0;

      inline SDL_FRect getHitbox() const
      {
         return SDL_FRect {
            // hitbox offset
            pos.x + hitbox_offset.x,
            pos.y + hitbox_offset.y,
            // hitbox size
            hitbox_scale.x,
            hitbox_scale.y
         };
      }
   };

   struct Vertex
   {
      glm::vec3 pos;
      glm::vec2 texCoord;
   };

   class Entity : public Renderable
   {
      // buffers
      GLuint VBO, EBO;
      // arrays
      Vertex vertices[4] = {
         {{-1.0f,  1.0f, 0.0f}, {0.f, 0.f}}, {{ 1.0f, 1.0f, 0.0f}, {1.f, 0.f}},
         {{ 1.0f, -1.0f, 0.0f}, {1.f, 1.f}}, {{-1.0f, 1.0f, 0.0f}, {0.f, 1.f}}
      };
      uint8_t indices[6] = {
         0, 1, 2,
         0, 2, 3
      };
   public:
      GLuint VAO;
      shrk::Sprite sprite;

      Entity(GLuint shaderID, GLuint textureID);

      void render() const override;
   };

}

#endif // GRAPHICS_HPP