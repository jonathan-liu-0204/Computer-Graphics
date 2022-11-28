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
  /*
    // Our ids
    GLuint depthMapFBO;
    GLuint texDepthBuffer;

    // bind framebuffer
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    // create the texture
    glGenTextures(1, &texDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, texDepthBuffer);

    // set it up as simply a depth component
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthBuffer, 0);

    // framebuffer by default needs at least a color component, but we wont use it, so we need to state that
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // check if it is correct
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::printf("Error building Framebuffer!\n");
    }
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

  /*
  // 1. first render to depth map
  glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
  RenderScene(simpleDepthShader);
  ConfigureShaderAndMatrices();
  RenderScene();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // 2. then render scene as normal with shadow mapping (using depth map)
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ConfigureShaderAndMatrices();
  glBindTexture(GL_TEXTURE_2D, depthMap);
  RenderScene();

  */
  glUseProgram(0);
}