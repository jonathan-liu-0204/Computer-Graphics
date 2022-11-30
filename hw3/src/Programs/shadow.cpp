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
  
    // Our ids
    GLuint depthMapFBO;
    GLuint depthMap;

    // bind framebuffer
    glGenFramebuffers(1, &depthMapFBO);

    // create the texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    ctx->shadowMapTexture = depthMap;

    // set it up as simply a depth component
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // check if it is correct
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::printf("Error building Framebuffer!\n");
    }
}

void ShadowProgram::doMainLoop() {
  //glUseProgram(programId);
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

  int obj_num = (int)ctx->objects.size();

  float near_plane = 1.0f;
  float far_plane = 7.5f;

  glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT); 

  glUseProgram(programId);

  for (int i = 0; i < obj_num; i++) {

    int modelIndex = ctx->objects[i]->modelIndex;
    Model* model = ctx->models[modelIndex];
    glBindVertexArray(model->vao);

    const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
    GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    glm::mat4 light_pos = glm::lookAt(ctx->lightDirection * (-10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 light_project = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 LightViewMatrix = light_project * light_pos;

    const float* lvm = glm::value_ptr(LightViewMatrix);
    GLint lvmmatLoc = glGetUniformLocation(programId, "LightViewMatrix");
    glUniformMatrix4fv(lvmmatLoc, 1, GL_FALSE, lvm);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, OpenGLContext::getWidth(), OpenGLContext::getHeight());
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glUseProgram(programId);
  glBindTexture(GL_TEXTURE_2D, ctx->shadowMapTexture);

  for (int i = 0; i < obj_num; i++) {
    int modelIndex = ctx->objects[i]->modelIndex;
    Model* model = ctx->models[modelIndex];
    glBindVertexArray(model->vao);

    const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
    GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    glm::mat4 light_pos =
        glm::lookAt(ctx->lightDirection * (-10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 light_project = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 LightViewMatrix = light_project * light_pos;

    const float* lvm = glm::value_ptr(LightViewMatrix);
    GLint lvmmatLoc = glGetUniformLocation(programId, "LightViewMatrix");
    glUniformMatrix4fv(lvmmatLoc, 1, GL_FALSE, lvm);
  }
  glUseProgram(0);
}