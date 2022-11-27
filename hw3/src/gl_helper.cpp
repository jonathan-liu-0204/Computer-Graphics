#include "gl_helper.h"
#include <fstream>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint quickCreateProgram(const char* vert_shader_filename, const char* frag_shader_filename) {
  GLuint vert = createShader(vert_shader_filename, GL_VERTEX_SHADER);
  if (vert == 0) return 0;

  GLuint frag = createShader(frag_shader_filename, GL_FRAGMENT_SHADER);
  if (frag == 0) {
    glDeleteShader(vert);
    return 0;
  }

  GLuint prog = createProgram(vert, frag);
  if (prog == 0) {
    glDeleteShader(vert);
    glDeleteShader(frag);
    return 0;
  }
  return prog;
}

GLuint createShader(const char* filename, GLenum type) {
  // Read shader code
  char* buffer = 0;
  long length;
  std::ifstream infile(filename, std::ios::binary);
  if (!infile.is_open()) {
    std::cout << "Open file fail: " << filename << std::endl;
    return 0;
  }
  infile.seekg(0, std::ios::end);
  length = (long)infile.tellg();
  infile.seekg(0, std::ios::beg);
  buffer = (char*)malloc(length + 1);
  buffer[length] = 0;
  if (buffer == NULL) {
    std::cout << "Allocate memory fail" << std::endl;
    infile.close();
    return 0;
  }
  infile.read(buffer, length);
  infile.close();

  // Compile shader
  int success;
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar**)&buffer, 0);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    glDeleteShader(shader);
    shader = 0;
  };

  free(buffer);
  return shader;
}

GLuint createProgram(GLuint vert, GLuint frag) {
  // shader Program
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);
  // print linking errors if any
  int success;
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(prog, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    glDeleteProgram(prog);
    return 0;
  }

  // Always detach shaders after a successful link.
  glDetachShader(prog, vert);
  glDetachShader(prog, frag);
  return prog;
}

GLuint createTexture(const char* filename) {
  GLuint texture;
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (data == NULL) {
    std::cout << "Failed to load texture " << filename << std::endl;
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return texture;
}

GLuint createCubemap(char faces[6][30]) {
  /* TODO#1-1: Load skybox cubemap texture
   *           1. Get right, left, bottom, top, front, back texture filename from parameter
   *           2. Genetare texture and bind to cube map
   *           3. Load image with stbi_load and bind to correct cubemap position
   *           4. Set MIN/MAG filter to linear and wrap to "clamp to edge"
   *           5. return the texture id
   * Note: for stbi_load, you can refer to createTexture above
   */

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;

  for (int i = 0; i < 6; i++) {
    unsigned char* data = stbi_load(faces[i], &width, &height, &nrChannels, 0);

    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
