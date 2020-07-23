#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include <memory>
#include "mvStandardWindow.h"

namespace Marvel {

	class mvTextEditor;

	class mvAppEditor : public mvStandardWindow
	{

	public:

		static const char*       getVersion() { return MV_SANDBOX_VERSION; }

		mvAppEditor();
		~mvAppEditor();

		mvAppEditor(const mvAppEditor& other) = delete;
		mvAppEditor(mvAppEditor&& other) = delete;
		mvAppEditor operator=(const mvAppEditor& other) = delete;
		mvAppEditor operator=(mvAppEditor&& other) = delete;

		// actual render loop
		void         preRender() {}
		virtual void render(bool& show) override;

		//-----------------------------------------------------------------------------
		// 
		//-----------------------------------------------------------------------------
		std::string&  getFile() { return m_file; }
		void          setFile(const std::string& file);
		void          closeFile();
		void          openFile();
		void          saveFile();
		void          saveFileAs();
		void          handleKeyEvents();

	private:

		mvTextEditor*                 m_editor = nullptr;
		std::string                   m_file;
		bool                          m_showWhiteSpace = false;
		bool                          m_saved = true;
		std::string                   m_complilerflags = "";

	};

}