#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_progress_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvProgressBar : public mvFloatPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
	public:

		MV_APPITEM_TYPE(mvAppItemType::ProgressBar, "add_progress_bar")

		mvProgressBar(const std::string& name, float default_value, const std::string& dataSource);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::string m_overlay;

	};

}
