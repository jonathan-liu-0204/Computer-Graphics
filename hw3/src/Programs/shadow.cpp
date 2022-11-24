#include <iostream>
#include "context.h"
#include "opengl_context.h"
#include "program.h"

GLfloat borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

ShadowProgram::ShadowProgram(Context* ctx) : Program(ctx) {
  vertProgramFile = "../assets/shaders/shadow.vert";
  fragProgramFIle = "../assets/shaders/shadow.frag";

  // TODO#2-0: comment this line if your computer is poor
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &SHADOW_MAP_SIZE);
  std::cout << "Current depth map size is " << SHADOW_MAP_SIZE << std::endl;

  /* TODO#2-1 Generate frame buffer and depth map for shadow program
   *          1. Generate frame buffer and store to depthMapFBO
   *          2. Generate depthmap texture and store to  ctx->shadowMapTexture
   *             - texure size is SHADOW_MAP_SIZE * SHADOW_MAP_SIZE
   *          3. properly setup depthmap's texture paremters
   *             - Set texture wrap to "clamp to border" and use border color provider above
   *          4. bind texture to framebuffer's depth buffer and disable color buffer read and write
   * Hint:
   *          - glGenFramebuffers
   *          - glGenTextures
   *          - glBindTexture
   *          - glTexImage2D
   *          - glTexParameteri
   *          - GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T
   *          - glTexParameterfv
   *          - GL_TEXTURE_BORDER_COLOR
   *          - glBindFramebuffer
   *          - glFramebufferTexture2D
   *          - glDrawBuffer
   *          - glReadBuffer
   */


}

void ShadowProgram::doMainLoop() {
  glUseProgram(programId);
  /* TODO#2-2: Render depth map with shader
   *           1. Change viewport to depth map size
   *           2. Bind out framebuffer
   *           3. A directional light doesn't have a source position. 
   *              However, for shadown map we need to render the scene from a light's perspective 
   *              and thus render the scene from a position somewhere along the lines of the light direction.
   *              The position of the light need to be "ctx->lightDirection * -10.0f"
   *           4. Properly set LightViewMatrix (projection matrix * view matrux)
   *           5. Rander all scene models as usual
   *           6. restore viewport and framebuffer
                  (get screen size from OpenGLContext::getWidth, OpenGLContext::getHeight)
   * Note:     1. You can consider the light as a camera, so you can use glm::lookAt to generate correct lightView matrix
   *           2. For the direction light we need orthogonal projection rather than perspective projection
   *              (the near plane, far plane value is provided, the image size is [-10~10], [-10~10]
   */


  glUseProgram(0);
}