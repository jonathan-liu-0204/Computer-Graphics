#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/vec3.hpp>

Model* Model::fromObjectFile(const char* obj_file) {
	Model* m = new Model();

	std::ifstream ObjFile(obj_file);

	if (!ObjFile.is_open()) {
		std::cout << "Can't open File !" << std::endl;
		return NULL;
	}

	/* TODO#1: Load model data from OBJ file
	*         You only need to handle v, vt, vn, f
	*         Other fields you can directly ignore
	*         Fill data into m->positions, m->texcoords m->normals and m->numVertex
	*         Data format:
	*           For positions and normals
	*         | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | 8    | 9    | 10   | 11   | ...
	*         | face 1                                                       | face 2               ...
	*         | v1x  | v1y  | v1z  | v2x  | v2y  | v2z  | v3x  | v3y  | v3z  | v1x  | v1y  | v1z  | ...
	*         | vn1x | vn1y | vn1z | vn2x | vn2y | vn2z | vn3x | vn3y | vn3z | vn1x | vn1y | vn1z | ...
	*           For texcoords
	*         | 0    | 1    | 2    | 3    | 4    | 5    | 6    | 7    | ...
	*         | face 1                                  | face 2        ...
	*         | v1x  | v1y  | v2x  | v2y  | v3x  | v3y  | v1x  | v1y  | ...
	* Note:
	*        OBJ File Format (https://en.wikipedia.org/wiki/Wavefront_.obj_file)
	*        For simplification, all faces from OBJ file are triangle (vertex per face = 3)
	*/

	std::string line = "";
	std::string prefix = "";
	std::stringstream ss;

	std::vector<glm::vec3> tmp_positions;
	std::vector<glm::vec3> tmp_normals;
	std::vector<glm::vec2> tmp_textcoords;

	glm::vec2 temp_vec2;
	glm::vec3 temp_vec3;

	tmp_positions.push_back(temp_vec3);
	tmp_normals.push_back(temp_vec3);
	tmp_textcoords.push_back(temp_vec2);

	std::vector<GLint> vertex_position_indicies;
	std::vector<GLint> vertex_texcoord_indicies;
	std::vector<GLint> vertex_normal_indicies;

	int tmp_vertexnums = 0;

	GLint temp_glint = 0;

	while (getline(ObjFile, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			tmp_positions.push_back(temp_vec3);
		}
		else if (prefix == "vn") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			tmp_normals.push_back(temp_vec3);
		}
		else if (prefix == "vt") {
			ss >> temp_vec2.x >> temp_vec2.y;
			tmp_textcoords.push_back(temp_vec2);
		}
		else if (prefix == "f") {
			int counter = 0;
			while (ss >> temp_glint) {
				// Pushing indices into correct arrays
				if (counter == 0){
					m->positions.push_back(tmp_positions[temp_glint].x);
					m->positions.push_back(tmp_positions[temp_glint].y);
					m->positions.push_back(tmp_positions[temp_glint].z);
				}
				else if (counter == 1){
					//vertex_texcoord_indicies.push_back(temp_glint);
					m->texcoords.push_back(tmp_textcoords[temp_glint].x);
					m->texcoords.push_back(tmp_textcoords[temp_glint].y);
				}
				else if (counter == 2){
					//vertex_normal_indicies.push_back(temp_glint);
					m->normals.push_back(tmp_normals[temp_glint].x);
					m->normals.push_back(tmp_normals[temp_glint].y);
					m->normals.push_back(tmp_normals[temp_glint].z);
				}

				// Handling characters
				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				}

				// Reset the counter
				if (counter > 2) counter = 0;
			}
		}
	}

	m -> numVertex = int(vertex_position_indicies.size());

	if (ObjFile.is_open()) ObjFile.close();

	return m;
}
