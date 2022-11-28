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

  glBindVertexArray(model->vao);

  const float* p = ctx->camera->getProjectionMatrix();
  GLint pmatLoc = glGetUniformLocation(programId, "Projection");
  glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, p);

  const float* v = ctx->camera->getViewMatrix();
  GLint vmatLoc = glGetUniformLocation(programId, "ViewMatrix");
  glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, v);

  glDepthMask(GL_FALSE);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(programId, "skybox"), 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, model->textures[ctx->skybox->textureIndex]);
  
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);

  glUseProgram(0);
}
