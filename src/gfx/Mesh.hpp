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

#include "tinyobjloader/tiny_obj_loader.hpp"

namespace gfx {

// TODO allow more shapes in one mesh

class Mesh {

public:

	class Component {
		friend class Mesh;

	private:

		GenericBuffer vertexBuffer;
		GenericBuffer indexBuffer;

		GLuint vertexArrayObject;

		unsigned int _sizeIndex = 0;
		unsigned int _sizeVertex = 0;

		Component(const vector<float>& vertexData, const vector<unsigned int>& indexData) {

			// magic numbers
			// 3 components for position
			// 4 components for color

			_sizeIndex = indexData.size();
			_sizeVertex = vertexData.size();

			vertexBuffer.realloc(vertexData);
			indexBuffer.realloc(indexData);

			glGenVertexArrays(1, &vertexArrayObject);
			glBindVertexArray(vertexArrayObject);

			size_t colorDataOffset = sizeof(float) * 3 * vertexData.size()/7;

			vertexBuffer.bind(GL_ARRAY_BUFFER);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) colorDataOffset);

			indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);

			glBindVertexArray(0);
		}

	public:

		unsigned int size() {
			return _sizeIndex;
		}

		void bindVAO() {
			glBindVertexArray(vertexArrayObject);
		}

		void unbindVAO() {
			glBindVertexArray(0);
		}

		void draw() {
			glDrawElements(GL_TRIANGLES, size(), GL_UNSIGNED_INT, 0);
		}
	};

private:

	map<string, Component> components;

	Mesh(const map<string, Component>& meshComponents)
		: components(meshComponents) { }

public:

	unsigned int size() {
		return components.size();
	}

	void drawAll() {
		for (auto& c : components) {
			c.second.bindVAO();
			c.second.draw();
			c.second.unbindVAO();
		}
	}

	const Component& operator[](unsigned int index) {
		auto iter = components.begin();
		std::advance(iter, index);
		return iter->second;
	}

	const Component& at(unsigned int index) {
		return (*this)[index];
	}

	const Component& operator[](const string& name) {
		return components.at(name);
	}

	const Component& at(const string& name) {
		return (*this)[name];
	}

	static shared_ptr<Mesh> fromFile(const string& fileName) {

		std::ifstream file(fileName);

		unsigned int numVertices = 0;
		unsigned int numIndices = 0;

		file >> numVertices >> numIndices;

		vector<float> vertexData;
		vector<unsigned int> indexData;

		std::copy_n(std::istream_iterator<float>(file), 3*numVertices + 4*numVertices,
				std::back_inserter(vertexData));

		std::copy_n(std::istream_iterator<unsigned int>(file), 3*numIndices,
				std::back_inserter(indexData));

		return shared_ptr<Mesh>(new Mesh({
			{"mesh", Component(vertexData, indexData)}
		}));
	}

	static shared_ptr<Mesh> fromObjFile(const string& fileName) {

		string inputfile = fileName;
		vector<tinyobj::shape_t> shapes;

		string err = tinyobj::LoadObj(shapes, inputfile.c_str());

		if (!err.empty())
			throw logic_error("failed to load model " + fileName);

		map<string, Component> components;

		for (auto& s : shapes)
			components.emplace(s.name,Component(s.mesh.positions, s.mesh.indices));

		return shared_ptr<Mesh>(new Mesh(components));
	}
};

}

#endif /* MESH_HPP_ */
