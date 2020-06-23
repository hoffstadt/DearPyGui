#pragma once

#include "mvApp.h"
#include "mvAppLog.h"

namespace Marvel {

	class mvWindow
	{

	public:

		mvWindow(unsigned width, unsigned height) : m_width(width), m_height(height){}

		virtual void show      () = 0;
		virtual void setup     () {}
		virtual void prerender () {}
		virtual void postrender() {}
		virtual void cleanup   () {}

		void run();

	protected:

		bool           m_running = true;
		Marvel::mvApp* m_app = nullptr;
		unsigned       m_width;
		unsigned       m_height;

	};

}
