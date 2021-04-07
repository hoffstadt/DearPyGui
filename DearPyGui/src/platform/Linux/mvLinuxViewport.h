#pragma once

#include "mvViewport.h"

class GLFWwindow;

namespace Marvel {

	class mvLinuxViewport : public mvViewport
	{

	public:

        mvLinuxViewport(unsigned width, unsigned height, bool error = false);
		~mvLinuxViewport();

		void show(bool minimized, bool maximized) override;

		void prerender  ();
		void postrender ();
		void renderFrame() override;
		void run        () override;
        void maximize   () override;
        void minimize   () override;
        void restore    () override;


	private:
        float             m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};
		GLFWwindow*       m_window = nullptr;

	};

}
