#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include "Core/AppItems/mvAppItem.h"
#include "Core/mvAppStyle.h"
#include "Core/mvTextEditor.h"
#include "mvStandardWindow.h"

namespace Marvel {

	class mvAppEditor : public mvStandardWindow
	{

		struct StandardWindowEntry
		{
			mvStandardWindow* window;
			bool              show;
		};

	public:

		static mvStandardWindow* GetAppEditor();
		static const char*       getVersion() { return MV_SANDBOX_VERSION; }

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
		void          setProgramName(const char* name) { m_programName = name; }

	private:

		mvAppEditor();

		mvAppEditor(const mvAppEditor& other) = delete;
		mvAppEditor(mvAppEditor&& other) = delete;
		mvAppEditor operator=(const mvAppEditor& other) = delete;
		mvAppEditor operator=(mvAppEditor&& other) = delete;

	private:

		static mvAppEditor*     s_instance;
		mvTextEditor            m_editor;
		std::string             m_file;
		bool                    m_showWhiteSpace = false;
		bool                    m_saved = true;
		std::string             m_complilerflags = "";
		const char*             m_programName;

		std::map < std::string, StandardWindowEntry> m_standardWindows;

	};

}