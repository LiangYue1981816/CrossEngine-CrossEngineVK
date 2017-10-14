/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	CGLES3Swapchain::CGLES3Swapchain(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
		, m_hDC(NULL)

		, m_width(0)
		, m_height(0)
		, m_format(VK_FORMAT_UNDEFINED)
		
		, m_vao(0)
		, m_vbo(0)
		, m_ibo(0)
		, m_program(0)
		, m_surface(0)
	{

	}

	CGLES3Swapchain::~CGLES3Swapchain(void)
	{

	}

	int CGLES3Swapchain::Create(HDC hDC, uint32_t width, uint32_t height, VkSurfaceTransformFlagBitsKHR transform)
	{
		m_hDC = hDC;

		CALL_GL_FUNCTION_RETURN(CreateSurface(width, height, VK_FORMAT_B8G8R8A8_UNORM));
		CALL_GL_FUNCTION_RETURN(CreateProgram(transform));

		return NO_ERROR;
	}

	void CGLES3Swapchain::Destroy(void)
	{
		DestroySurface();
		DestroyProgram();
	}

	int CGLES3Swapchain::CreateSurface(uint32_t width, uint32_t height, VkFormat format)
	{
		m_width = width;
		m_height = height;
		m_format = format;

		GLenum type;
		GLenum internalFormat;
		GLenum externalFormat;
		CGLES3Helper::glTranslateFormat(format, internalFormat, externalFormat, type);

		glGenTextures(1, &m_surface);
		glBindTexture(GL_TEXTURE_2D, m_surface);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, type, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		return NO_ERROR;
	}

	int CGLES3Swapchain::CreateProgram(VkSurfaceTransformFlagBitsKHR transform)
	{
		static const GLchar *szShaderVertexCode =
		"                                                                         \n\
			uniform mat4 _modelViewProjectionMatrix;                              \n\
			                                                                      \n\
			attribute vec3 _position;                                             \n\
			attribute vec4 _texcoord;                                             \n\
			                                                                      \n\
			varying vec4 texcoord;                                                \n\
			                                                                      \n\
			void main()                                                           \n\
			{                                                                     \n\
				gl_Position = _modelViewProjectionMatrix*vec4(_position, 1.0);    \n\
				texcoord = _texcoord;                                             \n\
			}                                                                     \n\
		";

		static const GLchar *szShaderFragmentCode =
		"                                                                         \n\
			#ifdef GL_ES                                                          \n\
			precision mediump float;                                              \n\
			#endif                                                                \n\
			                                                                      \n\
			uniform sampler2D _texture;                                           \n\
			                                                                      \n\
			varying vec4 texcoord;                                                \n\
			                                                                      \n\
			void main()                                                           \n\
			{                                                                     \n\
				gl_FragColor = texture2D(_texture, texcoord.xy);                  \n\
			}                                                                     \n\
		";

		GLint linked;
		GLint compiled;
		GLuint shaderVertex;
		GLuint shaderFragment;

		shaderVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shaderVertex, 1, &szShaderVertexCode, NULL);
		glCompileShader(shaderVertex);

		glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) return -1;

		shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shaderFragment, 1, &szShaderFragmentCode, NULL);
		glCompileShader(shaderFragment);

		glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) return -1;

		m_program = glCreateProgram();
		glAttachShader(m_program, shaderVertex);
		glAttachShader(m_program, shaderFragment);
		glLinkProgram(m_program);

		glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE) return -1;

		struct vertex {
			float position[3];
			float texcoord[2];
		};

		vertex vertices[4] = {
			{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { 1.0f,  -1.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { 1.0f,   1.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f } },
		};
		unsigned short indices[6] = { 0, 1, 2, 2, 3, 0 };

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		{
			GLint attribLocationPosition = glGetAttribLocation(m_program, "_position");
			GLint attribLocationTexcoord = glGetAttribLocation(m_program, "_texcoord");

			glEnableVertexAttribArray(attribLocationPosition);
			glVertexAttribPointer(attribLocationPosition, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);

			glEnableVertexAttribArray(attribLocationTexcoord);
			glVertexAttribPointer(attribLocationTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)12);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(m_program);
		{
			GLint uniformLocationTexture = glGetUniformLocation(m_program, "_texture");
			GLint uniformLocationModelViewProjection = glGetUniformLocation(m_program, "_modelViewProjectionMatrix");

			glm::mat4 matModeView = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 matProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
			glm::mat4 matModeViewProjection = matProjection * matModeView;

			glUniform1i(uniformLocationTexture, 0);
			glUniformMatrix4fv(uniformLocationModelViewProjection, 1, GL_FALSE, (const float *)&matModeViewProjection);
		}
		glUseProgram(0);

		return NO_ERROR;
	}

	void CGLES3Swapchain::DestroySurface(void)
	{
		glDeleteTextures(1, &m_surface);
		m_surface = 0;
	}

	void CGLES3Swapchain::DestroyProgram(void)
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
		glDeleteProgram(m_program);
	}

	void CGLES3Swapchain::RenderSurface(void) const
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_surface);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		{
			glViewport(0, 0, m_width, m_height);

			glUseProgram(m_program);
			glBindVertexArray(m_vao);
			{
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
			}
			glUseProgram(0);
			glBindVertexArray(0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	BOOL CGLES3Swapchain::Present(void) const
	{
		RenderSurface();

#ifdef PLATFORM_WINDOWS

		::SwapBuffers(m_hDC);
#endif

		return TRUE;
	}

	BOOL CGLES3Swapchain::AcquireNextImage(CGfxFence fence)
	{
		return TRUE;
	}

	CGfxSemaphore CGLES3Swapchain::GetAcquireSemaphore(void) const
	{
		return NULL;
	}

	CGfxSemaphore CGLES3Swapchain::GetRenderDoneSemaphore(void) const
	{
		return NULL;
	}

	uint32_t CGLES3Swapchain::GetImageCount(void) const
	{
		return 1;
	}

	uint32_t CGLES3Swapchain::GetImageIndex(void) const
	{
		return 0;
	}

	HANDLE CGLES3Swapchain::GetImageHandle(int indexImage) const
	{
		return (HANDLE)m_surface;
	}

	uint32_t CGLES3Swapchain::GetWidth(void) const
	{
		return m_width;
	}

	uint32_t CGLES3Swapchain::GetHeight(void) const
	{
		return m_height;
	}

	VkFormat CGLES3Swapchain::GetFormat(void) const
	{
		return m_format;
	}

}
