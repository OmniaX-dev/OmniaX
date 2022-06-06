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

		"uniform mat4     u_viewProjMatrix;\n"

		"void main()\n"
		"{\n"
			"gl_Position = u_viewProjMatrix * vec4(a_position, 1.0);\n"
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
			"vec4 color = texture(u_textures[iTexID], v_texCoords) * v_vertexColor;\n"
			"out_FragColor = color;"
		"}\n"
	;



	const String defaultLightShader_vert =
		"#version 420 core\n"

		"layout(location = 0) in vec3    a_position;\n"
		"layout(location = 1) in vec4    a_color;\n"
		"layout(location = 2) in vec2    a_texCoords;\n"
		"layout(location = 3) in float   a_texIndex;\n"

		"out vec4     v_vertexColor;\n"
		"out vec2     v_texCoords;\n"
		"out float    v_texIndex;\n"

		"uniform mat4     u_viewProjMatrix;\n"

		"void main()\n"
		"{\n"
			"gl_Position = u_viewProjMatrix * vec4(a_position, 1.0);\n"
			"v_vertexColor = a_color;\n"
			"v_texCoords = a_texCoords;\n"
			"v_texIndex = a_texIndex;\n"
		"}\n"
	;
	const String defaultLightShader_frag =
		"#version 420 core\n"

		"out vec4 out_FragColor;\n"
		"uniform sampler2D  u_textures[16];\n"
		"uniform int	    u_lightPasses;\n"

		"in vec4 v_vertexColor;\n"
		"in vec2 v_texCoords;\n"
		"in float v_texIndex;\n"

		"void main()\n"
		"{\n"
			"vec4 color = texture(u_textures[1], v_texCoords);\n"
			"for (int i = 2; i <= u_lightPasses + 1; i++)\n"
			"{\n"
				"color *= texture(u_textures[i], v_texCoords);\n"
			"}\n"
			"out_FragColor = color;\n"
		"}\n"
	;
}

#endif