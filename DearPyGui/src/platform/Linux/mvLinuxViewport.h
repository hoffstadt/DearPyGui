#pragma once

#include "mvViewport.h"

class GLFWwindow;

namespace Marvel {

	class mvLinuxViewport : public mvViewport
	{

	public:

        mvLinuxViewport(unsigned width, unsigned height);
		~mvLinuxViewport();

		void show(bool minimized, bool maximized) override;

		void prerender  ();
		void postrender ();
		void renderFrame() override;
		void run        () override;
        void maximize   () override;
        void minimize   () override;
        void restore    () override;
		void fullscreen () override;

	private:

		GLFWwindow*       _window = nullptr;

	};

}
