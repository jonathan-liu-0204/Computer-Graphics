#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include <glm/ext/matrix_transform.hpp>

#include "camera.h"
#include "context.h"
#include "gl_helper.h"
#include "model.h"
#include "opengl_context.h"
#include "program.h"
#include "utils.h"
#include "constants.h"

void initOpenGL();
void resizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int, int action, int);

Context ctx;
FilterProgram* fp;

void loadPrograms() {
  /* TODO#1~3 uncoment lines to enable shader programs
   * Notes:
   *   SkyboxProgram for TODO#1
   *   ShadowProgram, ShadowLightProgram for TODO#2
   *     - also comment out default LightProgram
   *   FilterProgramBindFrameAdapter, FilterProgram for TODO#3
   *     - FilterProgramBindFrameAdapter is used to change render buffer for skybox and light program
   */
  // fp = new FilterProgram(&ctx);
  // ctx.programs.push_back(new ShadowProgram(&ctx));
  // ctx.programs.push_back(new FilterProgramBindFrameAdapter(&ctx, fp));
  ctx.programs.push_back(new SkyboxProgram(&ctx));
  ctx.programs.push_back(new LightProgram(&ctx));
  // ctx.programs.push_back(new ShadowLightProgram(&ctx));
  // ctx.programs.push_back(fp);

  // TODO#0: You can trace light program before doing hw to know how this template work and difference from hw2
  for (auto iter = ctx.programs.begin(); iter != ctx.programs.end(); iter++) {
    if (!(*iter)->load()) {
      std::cout << "Load program fail, force terminate" << std::endl;
      exit(1);
    }
  }
  glUseProgram(0);
}

void loadModels() {
  // TODO#0: You can trace light program before doing hw to know how this template work and difference from hw2
  Model* m = Model::fromObjectFile("../assets/models/cube/cube.obj");
  m->textures.push_back(createTexture("../assets/models/cube/texture.bmp"));
  m->modelMatrix = glm::scale(m->modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
  attachGeneralObjectVAO(m);
  ctx.models.push_back(m);

  m = Model::fromObjectFile("../assets/models/Mugs/Models/Mug_obj3.obj");
  m->textures.push_back(createTexture("../assets/models/Mugs/Textures/Mug_C.png"));
  m->textures.push_back(createTexture("../assets/models/Mugs/Textures/Mug_T.png"));
  m->modelMatrix = glm::scale(m->modelMatrix, glm::vec3(6.0f, 6.0f, 6.0f));
  attachGeneralObjectVAO(m);
  ctx.models.push_back(m);

  m = new Model();
  float pos[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.12f, 8.192f, 0.0f, 5.12f, 8.192f, 0.0f, 0.0f};
  float nor[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
  float tx[] = {0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f, 2.0f, 0.0f};
  for (int i = 0; i < 12; i++) {
    m->positions.push_back(pos[i]);
    m->normals.push_back(nor[i]);
    if (i < 8) m->texcoords.push_back(tx[i]);
  }
  m->textures.push_back(createTexture("../assets/models/Wood_maps/AT_Wood.jpg"));
  m->numVertex = 4;
  m->drawMode = GL_QUADS;
  attachGeneralObjectVAO(m);
  ctx.models.push_back(m);

  /* TODO#1-1: Add skybox mode
   *         1. Create a model and manually set box positions 
   *            (you can get positions data from variable skyboxVertices)
   *         2. Add texture cubemap to model->textures with createCubemap(blueSkyboxfaces) 
   *            (you need to implement createCubemap)
   *         3. implement attachSkyboxVAO to create VAO from skybox 
   *            (you can refer to attachGeneralObjectVAO above)
   *         4. Set m->numVertex
   *         5. put model to ctx.models
   * 
   */

  m = new Model();

  for (int i = 0; i < 6; i++) {
    for (int j = i * 18; j < i * 18 + 18; j++) {
      m->positions.push_back(skyboxVertices[j]);
    } 
  }
  m->textures.push_back(createCubemap(blueSkyboxfaces));
  attachSkyboxVAO(m);
  m->numVertex = 6;
  ctx.models.push_back(m);
}

void setupObjects() {
  ctx.objects.push_back(new Object(0, glm::translate(glm::identity<glm::mat4>(), glm::vec3(-1, 0.2, -1))));
  ctx.objects.push_back(new Object(0, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0.2, -1))));
  ctx.objects.push_back(new Object(0, glm::translate(glm::identity<glm::mat4>(), glm::vec3(1, 0.2, -1))));
  ctx.objects.push_back(new Object(1, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0.3, 0))));
  ctx.objects.push_back(new Object(1, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.8, 0.3, 0))));
  (*ctx.objects.rbegin())->textureIndex = 1;


  ctx.objects.push_back(new Object(2, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 0))));
  ctx.objects.push_back(new Object(2, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, -5.12f))));
  ctx.objects.push_back(new Object(2, glm::translate(glm::identity<glm::mat4>(), glm::vec3(-8.192f, 0, -5.12f))));
  ctx.objects.push_back(new Object(2, glm::translate(glm::identity<glm::mat4>(), glm::vec3(-8.192f, 0, 0))));

  /* TODO#1-1: Uncomment to create skybox Object
   * Note:     Skybox object is put in Context::skybox rather than Context::objects 
   */
   ctx.skybox = new Object(3, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 0)));
}

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();
  /* TODO#0: Change window title to "HW3 - `your student id`"
   *         Ex. HW3 - 311550000
   */
  glfwSetWindowTitle(window, "HW3-311605004");

  // Init Camera helper
  Camera camera(glm::vec3(0, 2, 5));
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);
  ctx.camera = &camera;
  ctx.window = window;

  loadModels();
  loadPrograms();
  setupObjects();

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    camera.move(window);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);

    ctx.lightDegree = glm::clamp(ctx.lightDegree, 30.0f, 160.0f);
    ctx.lightDirection =
        glm::vec3(-0.3, -0.3 * sinf(glm::radians(ctx.lightDegree)), -0.3 * cosf(glm::radians(ctx.lightDegree)));

    // TODO#0: You can trace light program before doing hw to know how this template work and difference from hw2
    size_t sz = ctx.programs.size();
    for (size_t i = 0; i < sz; i++) {
      ctx.programs[i]->doMainLoop();
    }
    

#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  if (action == GLFW_REPEAT) {
    switch (key) {
      case GLFW_KEY_K:
        ctx.lightDegree += 1.0f;
        break;
      case GLFW_KEY_L:
        ctx.lightDegree -= 1.0f;
        break;
      default:
        break;
    }
    return;
  }
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_F9: {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
          // Show the mouse cursor
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
          // Hide the mouse cursor
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        break;
      }
      case GLFW_KEY_Y:
        ctx.enableShadow = !ctx.enableShadow;
        break;
      case GLFW_KEY_U:
        ctx.enableEdgeDetection = !ctx.enableEdgeDetection;
        break;
      case GLFW_KEY_I:
        ctx.eanbleGrayscale = !ctx.eanbleGrayscale;
        break;
      default:
        break;
    }
  }
}

void resizeCallback(GLFWwindow* window, int width, int height) {
  // TODO#3 uncomment this to update frame buffer size when window size chnage
  // fp->updateFrameBuffer(width, height);
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
//  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
}