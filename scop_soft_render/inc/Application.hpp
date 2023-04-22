#pragma once

namespace Scop
{

	class Application
	{
	private:
		class Window*	m_pWindow;

	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(const Application&&) = delete;

		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);

	};

}
