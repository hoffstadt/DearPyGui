#pragma once

namespace Marvel {

	class mvMetricsWindow
	{

	public:

		static mvMetricsWindow* GetWindow();

		void render(bool& show);

	private:

		mvMetricsWindow() = default;

	private:

		static mvMetricsWindow* s_instance;

	};

}