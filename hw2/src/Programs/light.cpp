#include <iostream>
#include "context.h"
#include "program.h"

bool LightProgram::load() {
  /* TODO#4-2: Pass model vertex data to vertex buffer
   *           1. Generate and bind vertex array object (VAO) for each model
   *           2. Generate and bind three vertex buffer objects (VBOs) for each model
   *           3. Pass model positions, normals and texcoords to to VBOs
   * Note:
   *           If you implament BasicProgram properly, You might inherent BasicProgram's load function
   */
  
   programId = quickCreateProgram(vertProgramFile, fragProgramFIle);

   int num_model = (int)ctx->models.size();
   VAO = new GLuint[num_model];
   glGenVertexArrays(num_model, VAO);

  for (int i = 0; i < num_model; i++) {
    glBindVertexArray(VAO[i]);
    Model* model = ctx->models[i];

    GLuint VBO[3];
    glGenBuffers(3, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->positions.size(), model->positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->normals.size(), model->normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->texcoords.size(), model->texcoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
  }

  return programId != 0;
   
   //return true;
} 

void LightProgram::doMainLoop() {
    /* TODO#4-3: Render objects with shader
    *           1. use and bind program (BasicProgram::programId)
    *           2. Iterate all objects (ctx->objects)
    *           3. Load currect model VAO for object
    *           4. Pass projection, view, model matrix to shaders
    *           5. Pass light and material parameters to shader
    *           6. Pass model texture to shaders
    *           7. Draw with glDrawArrays
    * Note:
    *           1. light paramters are providered in context.h
    *           2. material parameter for each object get be found in ctx->objects[i]->material
    */

    glUseProgram(programId);
    int obj_num = (int)ctx->objects.size();

    for (int i = 0; i < obj_num; i++) {
        int modelIndex = ctx->objects[i]->modelIndex;
        glBindVertexArray(VAO[modelIndex]);

        Model* model = ctx->models[modelIndex];

        const float* p = ctx->camera->getProjectionMatrix();
        GLint pmatLoc = glGetUniformLocation(programId, "Projection");
        glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, p);

        const float* v = ctx->camera->getViewMatrix();
        GLint vmatLoc = glGetUniformLocation(programId, "ViewMatrix");
        glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, v);

        const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
        GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
        glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

        const float* nm = glm::value_ptr(glm::transpose(glm::inverse(ctx->objects[i]->transformMatrix * model->modelMatrix)));
        GLint nmmatLoc = glGetUniformLocation(programId, "ModelNormalMatrix");
        glUniformMatrix4fv(nmmatLoc, 1, GL_FALSE, nm);

        Context context_data;

        struct DirectionLight {
          int enable;
          glm::vec3 direction;
          glm::vec3 lightColor;
        };

        DirectionLight direction_light{ context_data.directionLightEnable, context_data.directionLightDirection, context_data.directionLightColor};

        //std::cout << "directionLightEnable:  " << context_data.directionLightEnable << std::endl;
        //std::cout << "context_data.directionLightDirection:  " << context_data.directionLightDirection[0] << std::endl;

        GLint DLmatLoc_enable = glGetUniformLocation(programId, "dl.enable");
        glUniform1i(DLmatLoc_enable, context_data.directionLightEnable);

        GLint DLmatLoc_direction = glGetUniformLocation(programId, "dl.direction");
        glUniform3f(DLmatLoc_direction, direction_light.direction.x, direction_light.direction.y, direction_light.direction.z);

        GLint DLmatLoc_lightcolor = glGetUniformLocation(programId, "dl.lightColor");
        glUniform3f(DLmatLoc_lightcolor, direction_light.lightColor.x, direction_light.lightColor.y, direction_light.lightColor.z);

        glDrawArrays(model->drawMode, 0, model->numVertex);
    }
    glUseProgram(0);
}
