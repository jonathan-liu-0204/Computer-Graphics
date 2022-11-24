#include <iostream>
#include "context.h"
#include "program.h"

void SkyboxProgram::doMainLoop() {
  glUseProgram(programId);
  Model* model = ctx->models[ctx->skybox->modelIndex];

  /* TODO#1-2: Render skybox with shader
   *         1. Load skybox vao
   *         2. Pass projection, view matrix to shader
   *         3. Pass skybox shader to shader
   *         4. Properly enable or disable depth check with glDepthMask
   *         5. Draw with glDrawArrays
   * Note:   
   *         1. You might call glDepthMask before and after calling glDrawArrays
   *         2. parameter for glBindTexture is different from HW2
   *         3. you might call ctx->camera->getViewMatrixGLM to get original 
   *            view matrix to do some modification before pass to shader            
   */

  glUseProgram(0);
}
