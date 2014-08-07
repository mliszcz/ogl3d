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
#include "Material.hpp"
#include "Texture.hpp"

#include "glm/glm.hpp"
#include "tinyobjloader/tiny_obj_loader.hpp"

namespace gfx {

class Mesh {

public:

	class Component {
		friend class Mesh;

	private:

		GenericBuffer vertexBuffer;
		GenericBuffer indexBuffer;

		GLuint vertexArrayObject;

		shared_ptr<Material> objMaterial;

		unsigned int _sizeIndex = 0;
		unsigned int _sizeVertex = 0;

		Component(
				const vector<float>& vertexData,
				const vector<float>& normalData,
				const vector<float>& textureData,
				const vector<unsigned int>& indexData,
				shared_ptr<Material> componentMaterial) {

			objMaterial = componentMaterial;

			vector<float> vbData;
			std::copy(vertexData.begin(),  vertexData.end(),  std::back_inserter(vbData));
			std::copy(normalData.begin(),  normalData.end(),  std::back_inserter(vbData));
			std::copy(textureData.begin(), textureData.end(), std::back_inserter(vbData));

			_sizeIndex = indexData.size();
			_sizeVertex = vertexData.size();

			vertexBuffer.realloc(vbData);
			indexBuffer.realloc(indexData);

			glGenVertexArrays(1, &vertexArrayObject);
			glBindVertexArray(vertexArrayObject);

			vertexBuffer.bind(GL_ARRAY_BUFFER);

			if (!vertexData.empty())  glEnableVertexAttribArray(0);
			if (!normalData.empty())  glEnableVertexAttribArray(1);
			if (!textureData.empty()) glEnableVertexAttribArray(2);

			if (!vertexData.empty())  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			if (!normalData.empty())  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*vertexData.size()));
			if (!textureData.empty()) glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*(vertexData.size()+normalData.size())));

			indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);

			glBindVertexArray(0);
		}

	public:

		unsigned int size() const {
			return _sizeIndex;
		}

		shared_ptr<Material> material() const {
			return objMaterial;
		}

		void bind() const {
			glBindVertexArray(vertexArrayObject);
		}

		void unbind() const {
			glBindVertexArray(0);
		}

		void draw() const {
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

	void drawAll() {
		for (auto& c : components) {
			with component(&c.second);
			c.second.draw();
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

	static shared_ptr<Mesh> fromObjFile(const string& fileName) {

		string inputfile = fileName;
		vector<tinyobj::shape_t> shapes;
		string basename = inputfile.substr(0, inputfile.rfind('/')+1);
		string err = tinyobj::LoadObj(shapes, inputfile.c_str(), basename.c_str());

		if (!err.empty())
			throw logic_error("failed to load model " + fileName);

		vector<pair<string, Component>> components;

		float eps = 0.0000f;
		auto toVecPtr = [=](float* v) {
			return (fabs(v[0])>eps || fabs(v[1])>eps || fabs(v[2])>eps)
					? make_shared<glm::vec4>(v[0], v[1], v[2], 1.0f)
					: nullptr;
		};
		auto toFloatPtr = [=](float v) {
			return (fabs(v)>eps) ? make_shared<float>(v) : nullptr;
		};
		auto toTexPtr = [=](string fileName) {
			return (fileName.empty()) ? nullptr
					: Texture::fromDdsFile(fileName);
		};

		for (auto& s : shapes) {

			auto mat = make_shared<Material>();
			mat->Ka = toVecPtr(s.material.ambient);
			mat->Kd = toVecPtr(s.material.diffuse);
			mat->Ks = toVecPtr(s.material.specular);
			mat->Tr = toVecPtr(s.material.transmittance);
			mat->Ke = toVecPtr(s.material.emission);
			mat->Ns = toFloatPtr(s.material.shininess);
			mat->d = toFloatPtr(s.material.dissolve);
			mat->Ni = toFloatPtr(s.material.ior);

			mat->mapKa = toTexPtr(s.material.ambient_texname);
			mat->mapKd = toTexPtr(s.material.diffuse_texname);
			mat->mapKs = toTexPtr(s.material.specular_texname);

			components.emplace_back(s.name,Component(
					s.mesh.positions, s.mesh.normals,
					s.mesh.texcoords, s.mesh.indices, mat));
		}
		return shared_ptr<Mesh>(new Mesh(components));
	}
};

}

#endif /* MESH_HPP_ */
