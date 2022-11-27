#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/vec3.hpp>


void attachGeneralObjectVAO(Model* model) {
  GLuint* VAO = new GLuint[1];

  glGenVertexArrays(1, VAO);
  glBindVertexArray(VAO[0]);
  model->vao = VAO[0];

  GLuint VBO[3];
  glGenBuffers(3, VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->positions.size(), model->positions.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->normals.size(), model->normals.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->texcoords.size(), model->texcoords.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void attachSkyboxVAO(Model* model) {
  /* TODO#1: create VAO&VBO for skybox model and bind buffer data 
             (you can refer to attachGeneralObjectVAO above)
             (for skybox only one VBO for vertex position)
   */

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO); 
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model->positions.size(), model->positions.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindVertexArray(0);
}


Model* Model::fromObjectFile(const char* obj_file) {
  Model* m = new Model();

  std::ifstream ObjFile(obj_file);

  if (!ObjFile.is_open()) {
    std::cout << "Can't open File !" << std::endl;
    return NULL;
  }

  std::string line = "";
  std::string prefix = "";
  std::stringstream ss;
  std::vector<glm::vec3> vList(1), tList(1), nList(1);
  float vec[3];

  while (getline(ObjFile, line)) {
    ss.clear();
    ss.str(line);
    ss >> prefix;
    if (prefix == "v") {
      for (int i = 0; i < 3; i++) {
        ss >> prefix;
        vec[i] = stof(prefix);
      }
      vList.push_back(glm::vec3(vec[0], vec[1], vec[2]));
    } else if (prefix == "vt") {
      for (int i = 0; i < 2; i++) {
        ss >> prefix;
        vec[i] = stof(prefix);
      }
      tList.push_back(glm::vec3(vec[0], vec[1], vec[2]));
    } else if (prefix == "vn") {
      for (int i = 0; i < 3; i++) {
        ss >> prefix;
        vec[i] = stof(prefix);
      }
      nList.push_back(glm::vec3(vec[0], vec[1], vec[2]));
    } else if (prefix == "f") {
      m->numVertex += 3;
      std::string tmp = "";
      while (ss >> prefix) {
        size_t i = 0;
        bool Istexture = false;
        while (i < prefix.size()) {
          if (prefix[i] == '/') {
            if (Istexture) {
              m->texcoords.push_back(tList[atoi(tmp.c_str())][0]);
              m->texcoords.push_back(tList[atoi(tmp.c_str())][1]);
            } else {
              m->positions.push_back(vList[atoi(tmp.c_str())][0]);
              m->positions.push_back(vList[atoi(tmp.c_str())][1]);
              m->positions.push_back(vList[atoi(tmp.c_str())][2]);
            }
            tmp = "";
            Istexture = !Istexture;
          } else
            tmp += prefix[i];
          i++;
        }
        m->normals.push_back(nList[atoi(tmp.c_str())][0]);
        m->normals.push_back(nList[atoi(tmp.c_str())][1]);
        m->normals.push_back(nList[atoi(tmp.c_str())][2]);
        tmp = "";
      }
    }
  }
  if (ObjFile.is_open()) ObjFile.close();

  return m;
}
