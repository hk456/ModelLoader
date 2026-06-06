#pragma once

#include "../window/window.h"
#include "../pch.h"

namespace nrender
{
	class VertexIndexBuffer
	{
	public:
		VertexIndexBuffer() : mVBO{ 0 }, mVAO{ 0 }, mIBO{ 0 }
		{ }

		virtual void create_buffers(std::vector<unsigned int>& indices) = 0;

		virtual void delete_buffers() = 0;

		virtual void bind() = 0;

		virtual void unbind() = 0;

		virtual void draw(int index_count) = 0;

	protected:
		GLuint mVBO;
		GLuint mVAO;
		GLuint mIBO;
	};

	class Framebuffer
	{
	public:
		Framebuffer() : mFBO{ 0 }, mDepthId{ 0 }
		{ }

		virtual void create_buffers(uint32_t width, uint32_t height) = 0;

		virtual void delete_buffers() = 0;

		virtual void bind() = 0;

		virtual void unbind() = 0;

		virtual uint32_t get_texture() = 0;

	protected:
		uint32_t mFBO = 0;
		uint32_t mTexId = 0;
		uint32_t mDepthId = 0;
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};

	class RenderContext
	{
	public:
		RenderContext() : mWindow(nullptr) {}

		virtual bool init(nwindow::IWindow* window)
		{
			mWindow = window;
			return true;
		}

		virtual void pre_render() = 0;

		virtual void post_render() = 0;

		virtual void end() = 0;

	protected:
		nwindow::IWindow* mWindow;
	};

}
