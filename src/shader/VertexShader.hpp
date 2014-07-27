/*
 * VertexShader.hpp
 *
 *  Created on: Jul 27, 2014
 *      Author: michal
 */

#ifndef VERTEXSHADER_HPP_
#define VERTEXSHADER_HPP_

#include "AbstractShader.hpp"

namespace shader {

typedef AbstractShader<GL_VERTEX_SHADER> VertexShader;

//class VertexShader : public AbstractShader<GL_VERTEX_SHADER> { };

}

#endif /* VERTEXSHADER_HPP_ */
