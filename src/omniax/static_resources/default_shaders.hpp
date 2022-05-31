#ifndef __DEFAULT_SHADERS_HPP__
#define __DEFAULT_SHADERS_HPP__

#include <omniax/utils/Types.hpp>

namespace ox
{
	const String defaultShader_vert =
		"#version 420 core\n"

		"layout(location = 0) in vec3    a_position;\n"
		"layout(location = 1) in vec4    a_color;\n"
		"layout(location = 2) in vec2    a_texCoords;\n"
		"layout(location = 3) in float   a_texIndex;\n"

		"out vec4     v_vertexColor;\n"
		"out vec2     v_texCoords;\n"
		"out float    v_texIndex;\n"

		"uniform mat4     u_modelMatrix;\n"
		"uniform mat4     u_viewProjMatrix;\n"

		"void main()\n"
		"{\n"
			"gl_Position = u_viewProjMatrix * u_modelMatrix * vec4(a_position, 1.0);\n"
			"v_vertexColor = a_color;\n"
			"v_texCoords = a_texCoords;\n"
			"v_texIndex = a_texIndex;\n"
		"}\n"
	;
	const String defaultShader_frag =
		"#version 420 core\n"

		"out vec4 out_FragColor;\n"
		"uniform sampler2D u_textures[16];\n"

		"in vec4 v_vertexColor;\n"
		"in vec2 v_texCoords;\n"
		"in float v_texIndex;\n"

		"void main()\n"
		"{\n"
			"int iTexID = int(v_texIndex);\n"
			"out_FragColor = texture(u_textures[iTexID], v_texCoords) * v_vertexColor;\n"
		"}\n"
	;
}

#endif