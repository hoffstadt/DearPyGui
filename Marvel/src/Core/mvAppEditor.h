#pragma once

#define MV_SANDBOX_VERSION "0.1(WIP)"

#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include "Core/AppItems/mvAppItem.h"
#include "mvMouse.h"
#include "mvAppStyle.h"
#include "mvTextEditor.h"

namespace Marvel {

	class mvAppEditor final
	{

	public:

		static mvAppEditor* GetAppEditor();
		static const char* getVersion() { return MV_SANDBOX_VERSION; }

		// actual render loop
		void preRender() {}
		void render();

		//-----------------------------------------------------------------------------
		// 
		//-----------------------------------------------------------------------------
		void          setWindowSize(unsigned width, unsigned height) { m_width = width; m_height = height; }
		unsigned      getWindowWidth() const { return m_width; }
		unsigned      getWindowHeight() const { return m_height; }
		std::string&  getFile() { return m_file; }
		void          setFile(const std::string& file);
		void          closeFile();
		void          openFile();
		void          saveFile();
		void          saveFileAs();
		void          handleKeyEvents();
		void          setProgramName(const char* name) { m_programName = name; }

		//-----------------------------------------------------------------------------
		// Standard Windows
		//-----------------------------------------------------------------------------
		inline void showMetrics() { m_showMetrics = true; }
		inline void showAbout() { m_showAbout = true; }
		inline void showDoc() { m_showDoc = true; }

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
		unsigned                m_width = 1280;
		unsigned                m_height = 800;
		bool                    m_showMetrics = false;
		bool                    m_showAbout = false;
		bool                    m_showDoc = false;
		bool                    m_showWhiteSpace = false;
		bool                    m_saved = true;
		std::string             m_flags = "";
		const char*             m_programName;

	};

}