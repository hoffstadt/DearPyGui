#pragma once

#include "core/mvWindow.h"

class GLFWwindow;

namespace Marvel {

	class mvLinuxWindow : public mvWindow
	{

	public:

        mvLinuxWindow(unsigned width, unsigned height, bool error = false);
		~mvLinuxWindow();

		void prerender  () override;
		void postrender () override;
		void renderFrame() override;
		void cleanup    () override;
		void run        () override;
		void setWindowText(const std::string& name) override;


	private:
        float             m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};
		GLFWwindow*       m_window;

	};

}
