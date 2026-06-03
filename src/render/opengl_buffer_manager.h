#pragma once

#include "../pch.h"
#include "render_base.h"

namespace nrender
{
	class OpenGL_Framebuffer : public Framebuffer
	{
	public:

		void create_buffers(uint32_t width, uint32_t height) override;

		void delete_buffers() override;

		void bind() override;

		void unbind() override;

		uint32_t get_texture() override;
	};
}
