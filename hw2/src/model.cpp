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

	glm::vec2 temp_vec2;
	glm::vec3 temp_vec3;

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
			m->positions.push_back(temp_vec3.x);
			m->positions.push_back(temp_vec3.y);
			m->positions.push_back(temp_vec3.z);
		}
		else if (prefix == "vn") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			m->normals.push_back(temp_vec3.x);
			m->normals.push_back(temp_vec3.y);
			m->normals.push_back(temp_vec3.z);
		}
		else if (prefix == "vt") {
			ss >> temp_vec2.x >> temp_vec2.y;
			m->texcoords.push_back(temp_vec2.x);
			m->texcoords.push_back(temp_vec2.y);
		}
		else if (prefix == "f") {
			int counter = 0;
			while (ss >> temp_glint) {
				// Pushing indices into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_glint);
				else if (counter == 1)
					vertex_texcoord_indicies.push_back(temp_glint);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_glint);

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
