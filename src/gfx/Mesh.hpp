/*
 * Mesh.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: michal
 */

#ifndef MESH_HPP_
#define MESH_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <fstream>

#include "../Common.hpp"
#include "GenericBuffer.hpp"

#include "glm/glm.hpp"
#include "tinyobjloader/tiny_obj_loader.hpp"

namespace gfx {

class Mesh {

public:

	class Material {

	public:
		glm::vec4 	Ka;		// ambient
		glm::vec4 	Kd;		// diffuse
		glm::vec4 	Ks;		// specular
		glm::vec4 	Tr;		// transmitance
		glm::vec4 	Ke;		// emission
		float 		Ns;		// shininess
	};

	class Component {
		friend class Mesh;

	private:

		GenericBuffer vertexBuffer;
		GenericBuffer indexBuffer;

		GLuint vertexArrayObject;

		Material material;

		unsigned int _sizeIndex = 0;
		unsigned int _sizeVertex = 0;

		Component(
				const vector<float>& vertexData,
				const vector<float>& normalData,
				const vector<unsigned int>& indexData,
				const Material& componentMaterial) {

			material = componentMaterial;

			vector<float> vbData;
			std::copy(vertexData.begin(), vertexData.end(), std::back_inserter(vbData));
			std::copy(normalData.begin(), normalData.end(), std::back_inserter(vbData));
//			std::fill_n(std::back_inserter(vbData), 4*vertexData.size()/3, 1.0f);			// ignore material and use lit diffuse color

			_sizeIndex = indexData.size();
			_sizeVertex = vertexData.size();

			vertexBuffer.realloc(vbData);
			indexBuffer.realloc(indexData);

			glGenVertexArrays(1, &vertexArrayObject);
			glBindVertexArray(vertexArrayObject);

			vertexBuffer.bind(GL_ARRAY_BUFFER);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
//			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*vertexData.size()));
//			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*(vertexData.size()+normalData.size())));

			indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);

			glBindVertexArray(0);
		}

	public:

		unsigned int size() const {
			return _sizeIndex;
		}

		void bindVAO() const {
			glBindVertexArray(vertexArrayObject);
		}

		void unbindVAO() const {
			glBindVertexArray(0);
		}

		void draw(shared_ptr<shader::Program> program) const {
//			program->uniform("matKa") = material.Ka;
			program->uniform("matKd") = material.Kd;
			program->uniform("matKs") = material.Ks;
//			program->uniform("matTr") = material.Tr;
//			program->uniform("matKe") = material.Ke;
			program->uniform("matNs") = material.Ns;
			glDrawElements(GL_TRIANGLES, size(), GL_UNSIGNED_INT, 0);
		}
	};

private:

	vector<pair<string, Component>> components;

	Mesh(const vector<pair<string, Component>>& meshComponents)
		: components(meshComponents) { }

public:

	vector<pair<string, Component>>::const_iterator begin() {
		return components.cbegin();
	}

	vector<pair<string, Component>>::const_iterator end() {
		return components.cend();
	}

	unsigned int size() {
		return components.size();
	}

	void drawAll(shared_ptr<shader::Program> program) {
		for (auto& c : components) {
			c.second.bindVAO();
			c.second.draw(program);
			c.second.unbindVAO();
		}
	}

	const pair<string, Component>& operator[](unsigned int index) {
		return at(index);
	}

	const pair<string, Component>& at(unsigned int index) {
		return components.at(index);
	}

	const pair<string, Component>& operator[](const string& name) {
		return at(name);
	}

	const pair<string, Component>& at(const string& name) {
		return *std::find_if(components.begin(), components.end(),
				[&](pair<string, Component>& p){ return p.first == name; });
	}

//	static shared_ptr<Mesh> fromFile(const string& fileName) {
//
//		std::ifstream file(fileName);
//
//		unsigned int numVertices = 0;
//		unsigned int numIndices = 0;
//
//		file >> numVertices >> numIndices;
//
//		vector<float> vertexData;
//		vector<unsigned int> indexData;
//
//		std::copy_n(std::istream_iterator<float>(file), 3*numVertices + 4*numVertices,
//				std::back_inserter(vertexData));
//
//		std::copy_n(std::istream_iterator<unsigned int>(file), 3*numIndices,
//				std::back_inserter(indexData));
//
//		return shared_ptr<Mesh>(new Mesh({
//			{"mesh", Component(vertexData, vector<float>(), indexData)}
//		}));
//	}

	static shared_ptr<Mesh> fromObjFile(const string& fileName) {

		string inputfile = fileName;
		vector<tinyobj::shape_t> shapes;
		string basename = inputfile.substr(0, inputfile.rfind('/')+1);
		string err = tinyobj::LoadObj(shapes, inputfile.c_str(), basename.c_str());

		if (!err.empty())
			throw logic_error("failed to load model " + fileName);

		vector<pair<string, Component>> components;

		auto toVec = [](float* v) { return glm::vec4(v[0], v[1], v[2], 0.0f); };

		for (auto& s : shapes) {
			Material mat;
			mat.Ka = glm::vec4(toVec(s.material.ambient));
			mat.Kd = glm::vec4(toVec(s.material.diffuse));
			mat.Ks = glm::vec4(toVec(s.material.specular));
			mat.Tr = glm::vec4(toVec(s.material.transmittance));
			mat.Ke = glm::vec4(toVec(s.material.emission));
			mat.Ns = s.material.shininess;
			components.emplace_back(s.name,Component(s.mesh.positions, s.mesh.normals, s.mesh.indices, mat));
		}
		return shared_ptr<Mesh>(new Mesh(components));
	}
};

}

#endif /* MESH_HPP_ */
