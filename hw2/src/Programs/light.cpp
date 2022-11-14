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

        const float* nm = glm::value_ptr(glm::transpose(glm::inverse(model->modelMatrix)));
        GLint nmmatLoc = glGetUniformLocation(programId, "ModelNormalMatrix");
        glUniformMatrix4fv(nmmatLoc, 1, GL_FALSE, nm); 

        glUniform3f(glGetUniformLocation(programId, "viewPos"), 
                            ctx->camera->getPosition()[0],
                            ctx->camera->getPosition()[1], 
                            ctx->camera->getPosition()[2]);

        glActiveTexture(GL_TEXTURE0 + ctx->objects[i]->textureIndex);
        glBindTexture(GL_TEXTURE_2D, model->textures[ctx->objects[i]->textureIndex]);
        glUniform1i(glGetUniformLocation(programId, "ourTexture"), ctx->objects[i]->textureIndex);

        //======================================
        // Material Parameters
        glUniform3f(glGetUniformLocation(programId, "material.ambient"), 
                            ctx->objects[i]->material.ambient.x, 
                            ctx->objects[i]->material.ambient.y,
                            ctx->objects[i]->material.ambient.z);

        glUniform3f(glGetUniformLocation(programId, "material.diffuse"),
                            ctx->objects[i]->material.diffuse.x,
                            ctx->objects[i]->material.diffuse.y,
                            ctx->objects[i]->material.diffuse.z);

        glUniform3f(glGetUniformLocation(programId, "material.specular"), 
                            ctx->objects[i]->material.specular.x,
                            ctx->objects[i]->material.specular.y,
                            ctx->objects[i]->material.specular.z);

        glUniform1f(glGetUniformLocation(programId, "material.shininess"), 
                            ctx->objects[i]->material.shininess);

        //======================================
        // Direction Light Parameters

        glUniform1i(glGetUniformLocation(programId, "dl.enable"),
                            ctx->directionLightEnable);

        glUniform3f(glGetUniformLocation(programId, "dl.direction"),
                            ctx->directionLightDirection.x,
                            ctx->directionLightDirection.y, 
                            ctx->directionLightDirection.z);

        glUniform3f(glGetUniformLocation(programId, "dl.lightColor"), 
                            ctx->directionLightColor.r,
                            ctx->directionLightColor.g,
                            ctx->directionLightColor.b);

        //======================================
        //Point Light Parameters

        glUniform1i(glGetUniformLocation(programId, "pl.enable"),
                            ctx->pointLightEnable);

        glUniform3f(glGetUniformLocation(programId, "pl.position"),
                            ctx->pointLightPosition.x,
                            ctx->pointLightPosition.y,
                            ctx->pointLightPosition.z);

        glUniform3f(glGetUniformLocation(programId, "pl.lightColor"),
                            ctx->pointLightColor.r,
                            ctx->pointLightColor.g,
                            ctx->pointLightColor.b);

        glUniform1f(glGetUniformLocation(programId, "pl.constant"),
                            ctx->pointLightConstant);

        glUniform1f(glGetUniformLocation(programId, "pl.linear"),
                            ctx->pointLightLinear);

        glUniform1f(glGetUniformLocation(programId, "pl.quadratic"), 
                            ctx->pointLightQuardratic);

        //======================================
        //Spotlight Parameters

        glUniform1i(glGetUniformLocation(programId, "sl.enable"),
                            ctx->spotLightEnable);

        glUniform3f(glGetUniformLocation(programId, "sl.position"), 
                            ctx->spotLightPosition.x, 
                            ctx->spotLightPosition.y,
                            ctx->spotLightPosition.z);

        glUniform3f(glGetUniformLocation(programId, "sl.direction"), 
                            ctx->spotLightDirection.x,
                            ctx->spotLightDirection.y, 
                            ctx->spotLightDirection.z);

        glUniform3f(glGetUniformLocation(programId, "sl.lightColor"), 
                            ctx->spotLightColor.r, 
                            ctx->spotLightColor.g,
                            ctx->spotLightColor.b);

        glUniform1f(glGetUniformLocation(programId, "sl.cutOff"), 
                            ctx->spotLightCutOff);

        glUniform1f(glGetUniformLocation(programId, "sl.constant"), 
                            ctx->spotLightConstant);

        glUniform1f(glGetUniformLocation(programId, "sl.linear"), 
                            ctx->spotLightLinear);

        glUniform1f(glGetUniformLocation(programId, "sl.quadratic"),
                            ctx->spotLightQuardratic);

        //======================================

        glDrawArrays(model->drawMode, 0, model->numVertex);
    }
    glUseProgram(0);
}
