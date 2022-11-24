#include <iostream>
#include "context.h"
#include "program.h"

void ShadowLightProgram::doMainLoop() {
  glUseProgram(programId);

  /* TODO#2-3: Render scene with shadow mapping
   *           1. Copy from LightProgram
   *           2. Pass LightViewMatrix, fakeLightPos, shadowMap, enableShadow to shader program
   * Note:     LightViewMatrix and fakeLightPos are the same as what we used is ShadowProgram
   */ 


  glUseProgram(0);
}
