#include <iostream>
#include "context.h"
#include "program.h"
#include "opengl_context.h"

FilterProgram::FilterProgram(Context *ctx) : Program(ctx) {
  vertProgramFile = "../assets/shaders/filter.vert";
  fragProgramFIle = "../assets/shaders/filter.frag";

  // TODO#3-1: Generate Framebuffer and VAO/VBO for filter
  // Note:     You need to design proper position/texcoord data for filter program (NDC)


  updateFrameBuffer(OpenGLContext::getWidth(), OpenGLContext::getHeight());
}

void FilterProgram::updateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) {
  /* TODO#3-1: generate color/depth buffer for frame buffer
   *           (this function will also be trigger when windown resize)
   *           1. Generate color buffer texture and store in colorBuffer
   *           2. Set texture size to SCR_WIDTH*SCR_HEIGHT
   *           3. Set MIN/MAG filter to linear
   *           4. Generate/Bind a render buffer inad store in rboDepth
   *           5. Set Render buffer size to SCR_WIDTH*SCR_HEIGHT
   *           6. Attach colorBuffer and rboDepth to filterFBO
   * Hint:
   *           - glGenRenderbuffers
   *           - glBindRenderbuffer
   *           - glRenderbufferStorage
   *           - glBindFramebuffer
   *           - glFramebufferTexture2D
   *           - glFramebufferRenderbuffer
   */

}

void FilterProgram::bindFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, filterFBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FilterProgram::doMainLoop() {
  glUseProgram(programId);

  /* TODO#3-1: pass VAO, enableEdgeDetection, eanbleGrayscale, colorBuffer to shader and render
   */


  glUseProgram(0);
}