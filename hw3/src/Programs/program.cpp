#include "program.h"

bool Program::load() {
  programId = quickCreateProgram(vertProgramFile, fragProgramFIle);
  return programId != 0;
}