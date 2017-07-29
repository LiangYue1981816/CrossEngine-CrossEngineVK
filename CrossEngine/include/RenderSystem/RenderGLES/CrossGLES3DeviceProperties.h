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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	typedef struct GLPhysicalDeviceLimits {
		GLint MAX_VERTEX_UNIFORM_COMPONENTS;
		GLint MAX_VERTEX_UNIFORM_BLOCKS;
		GLint MAX_VERTEX_UNIFORM_VECTORS;
		GLint MAX_VERTEX_ATTRIBS;
		GLint MAX_VERTEX_OUTPUT_COMPONENTS;
		GLint MAX_VERTEX_TEXTURE_IMAGE_UNITS;
		GLint MAX_VERTEX_ATOMIC_COUNTERS;
		GLint MAX_VERTEX_SHADER_STORAGE_BLOCKS;
		GLint MAX_FRAGMENT_UNIFORM_COMPONENTS;
		GLint MAX_FRAGMENT_UNIFORM_BLOCKS;
		GLint MAX_FRAGMENT_UNIFORM_VECTORS;
		GLint MAX_FRAGMENT_INPUT_COMPONENTS;
		GLint MAX_FRAGMENT_ATOMIC_COUNTERS;
		GLint MAX_FRAGMENT_SHADER_STORAGE_BLOCKS;
		GLint MAX_COMPUTE_UNIFORM_COMPONENTS;
		GLint MAX_COMPUTE_UNIFORM_BLOCKS;
		GLint MAX_COMPUTE_TEXTURE_IMAGE_UNITS;
		GLint MAX_COMPUTE_ATOMIC_COUNTERS;
		GLint MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS;
		GLint MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
		GLint MAX_UNIFORM_BLOCK_SIZE;
		GLint MAX_TEXTURE_IMAGE_UNITS;
		GLint MAX_UNIFORM_BUFFER_BINDINGS;
		GLint MAX_SHADER_STORAGE_BUFFER_BINDINGS;
		GLint MAX_ATOMIC_COUNTER_BUFFER_BINDINGS;
		GLint MAX_COMBINED_TEXTURE_IMAGE_UNITS;
		GLint MAX_COMBINED_UNIFORM_BLOCKS;
		GLint MAX_COMBINED_ATOMIC_COUNTERS;
		GLint MAX_COMBINED_SHADER_STORAGE_BLOCKS;
		GLint MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS;
		GLint MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS;
		GLint MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS;
		GLint MAX_TEXTURE_LOD_BIAS;
		GLint MAX_TEXTURE_SIZE;
		GLint MAX_CUBE_MAP_TEXTURE_SIZE;
		GLint MAX_3D_TEXTURE_SIZE;
		GLint MAX_ARRAY_TEXTURE_LAYERS;
		GLint MAX_COLOR_ATTACHMENTS;
		GLint MAX_DRAW_BUFFERS;
		GLint MAX_RENDERBUFFER_SIZE;
		GLint MAX_VIEWPORT_DIMS;
		GLint MAX_SAMPLES;
		GLint MAX_VARYING_VECTORS;
		GLint SUBPIXEL_BITS;
		GLint MAX_ELEMENTS_INDICES;
		GLint MAX_ELEMENTS_VERTICES;
		GLint MIN_PROGRAM_TEXEL_OFFSET;
		GLint MAX_PROGRAM_TEXEL_OFFSET;
		GLint MIN_ALIASED_LINE_WIDTH;
		GLint MAX_ALIASED_LINE_WIDTH;
		GLint MIN_ALIASED_POINT_SIZE;
		GLint MAX_ALIASED_POINT_SIZE;
	} GLPhysicalDeviceLimits;

	class CROSS_EXPORT CGLES3DeviceProperties
	{
		friend class CGLES3Device;


	protected:
		CGLES3DeviceProperties(void);
		virtual ~CGLES3DeviceProperties(void);


	protected:
		void Init(void);
		const GLPhysicalDeviceLimits& GetPhysicalDeviceLimits(void) const;


	protected:
		GLPhysicalDeviceLimits m_limits;
	};

}
