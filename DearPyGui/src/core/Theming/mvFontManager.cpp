#include "mvFontManager.h"
#include <imgui.h>
#include <assert.h>
#include <array>
#include "mvApp.h"
#include "mvAppItem.h"
#include "mvCore.h"
#include "mvItemRegistry.h"
#include "mvWindow.h"

namespace Marvel {

	mvFontManager::mvFontManager()
	{
		mvEventBus::Subscribe(this, SID("add_font"), mvEVT_CATEGORY_THEMES);
		mvEventBus::Subscribe(this, SID("set_font"), mvEVT_CATEGORY_THEMES);
	}

	void mvFontManager::addFont(const std::string& font, const std::string& file, int size)
	{
		std::string key = font + std::to_string(size);

		if (m_fonts.count(key) != 0)
			delete m_fonts[key];

		m_fonts[key] = nullptr;
		m_fontFile[key] = file;
		m_fontSize[key] = size;
		rebuild = true;
	}

	ImFont* mvFontManager::getFont(const std::string& font, int size)
	{
		std::string key = font + std::to_string(size);

		if (m_fonts.count(key) == 0)
		{
			assert(false);
			return nullptr;
		}

		return m_fonts[key];
	}

	void mvFontManager::rebuildAtlas()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.FontDefault = io.Fonts->AddFontDefault();

		for (auto& font : m_fonts)
		{
			font.second = io.Fonts->AddFontFromFileTTF(m_fontFile[font.first].c_str(), m_fontSize[font.first]);
		}

		io.Fonts->Build();
	}

	bool mvFontManager::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvFontManager::onSetFont), SID("set_font"));
		return event.handled;
	};

	bool mvFontManager::onSetFont(mvEvent& event)
	{
		const std::string& widget = GetEString(event, "WIDGET");
		const std::string& font = GetEString(event, "FONT");
		int size = GetEInt(event, "SIZE");

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item)
		{
			item->setFont(getFont(font, size));
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					ThrowPythonException("Item can not be found");
				});
		}
		return true;
	}

	void AddFontCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_font", mvPythonParser({
			{mvPythonDataType::String, "font", "ttf or otf file"},
			{mvPythonDataType::String, "file", "ttf or otf file"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "size", "", "13.0"},
			{mvPythonDataType::String, "glyph_ranges", "options: korean, japanese, chinese_full, chinese_simplified_common, cryillic, thai, vietnamese", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::IntList, "custom_glyph_chars", "", "()"},
			{mvPythonDataType::Object, "custom_glyph_ranges", "list of ranges", "List[List[int]]"},
		}, "Adds additional font.", "None", "Themes and Styles") });

		parsers->insert({ "set_font", mvPythonParser({
			{mvPythonDataType::String, "font", "ttf or otf file"},
			{mvPythonDataType::Integer, "size", "ttf or otf file"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "item", "", ""},
		}, "Adds additional font.", "None", "Themes and Styles") });

	}

	PyObject* add_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* font;
		const char* file;
		float size = 13.0f;
		const char* glyph_ranges = "";
		PyObject* custom_glyph_ranges = nullptr;
		PyObject* custom_glyph_chars = nullptr;

		if (!(mvApp::GetApp()->getParsers())["add_font"].parse(args, kwargs, __FUNCTION__,
			&font, &file, &size, &glyph_ranges, &custom_glyph_chars, &custom_glyph_ranges))
			return GetPyNone();

		std::vector<int> custom_chars = ToIntVect(custom_glyph_chars);
		std::vector<std::pair<int, int>> custom_ranges = ToVectInt2(custom_glyph_ranges);

		mvApp::GetApp()->getFontManager().addFont(font, file, size);

		return GetPyNone();
	}

	PyObject* set_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* font = "";
		int size = 0;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_font"].parse(args, kwargs, __FUNCTION__, &font, &size, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		mvEventBus::Publish
		(
			mvEVT_CATEGORY_THEMES,
			SID("set_font"),
			{
				CreateEventArgument("WIDGET", std::string(item)),
				CreateEventArgument("FONT", std::string(font)),
				CreateEventArgument("SIZE", size)
			}
		);

		return GetPyNone();
	}
}