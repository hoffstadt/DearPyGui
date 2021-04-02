#include "mvModule_Core.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvAppLog.h"
#include "mvAppItemCommons.h"
#include "mvWindow.h"
#include "mvFontManager.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>

namespace Marvel {

	PyMethodDef* mvModule_Core::GetSubMethods()
	{

		static std::vector<PyMethodDef> methods;
		methods.clear();

		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {
				using item_type = typename mvItemTypeMap<i>::type;
				methods.push_back(item_type::GetMethodDefinition());
				item_type::FillExtraCommands(methods);
			});

		mvApp::FillExtraCommands(methods);
		mvAppItem::FillExtraCommands(methods);
		mvItemRegistry::FillExtraCommands(methods);
		mvThemeManager::FillExtraCommands(methods);
		mvFontManager::FillExtraCommands(methods);
		mvTextureStorage::FillExtraCommands(methods);
		mvCallbackRegistry::FillExtraCommands(methods);
		mvInput::FillExtraCommands(methods);

		methods.push_back({ NULL, NULL, 0, NULL });

		return methods.data();
	}

	const std::map<std::string, mvPythonParser>& mvModule_Core::GetSubModuleParsers()
	{

		static auto parsers = std::map<std::string, mvPythonParser>();

		if (parsers.empty())
		{
			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {
					using item_type = typename mvItemTypeMap<i>::type;
					item_type::InsertParser(&parsers);
				});

			mvApp::InsertParser(&parsers);
			mvAppItem::InsertParser(&parsers);
			mvItemRegistry::InsertParser(&parsers);
			mvThemeManager::InsertParser(&parsers);
			mvFontManager::InsertParser(&parsers);
			mvTextureStorage::InsertParser(&parsers);
			mvCallbackRegistry::InsertParser(&parsers);
			mvInput::InsertParser(&parsers);
		}
		return parsers;
	}

	const std::vector<std::pair<std::string, long>>& mvModule_Core::GetSubModuleConstants()
	{
		static bool First_Run = true;
		static std::vector<std::pair<std::string, long>> ModuleConstants;

		if (First_Run)
		{
			mvInput::InsertConstants(ModuleConstants);
			mvTextureStorage::InsertConstants(ModuleConstants);

			auto decodeType = [](long encoded_constant, mvAppItemType* type)
			{
				*type = (mvAppItemType)(encoded_constant / 1000);
			};

			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {

					using item_type = typename mvItemTypeMap<i>::type;

					// color constants
					for (const auto& item : item_type::GetColorConstants())
					{
						ModuleConstants.push_back({ std::get<0>(item), std::get<1>(item) });

						static mvAppItemType type;
						long mvThemeConstant = std::get<1>(item);
						decodeType(mvThemeConstant, &type);
						mvColor color = std::get<2>(item);
						mvColor color_disable = std::get<3>(item);
						const std::string& name = std::get<0>(item);

						mvThemeManager::GetColors()[type][mvThemeConstant].first = color;
						mvThemeManager::GetColors()[type][mvThemeConstant].second = color_disable;
						mvThemeManager::GetColorsPtr().push_back({name, mvThemeConstant, &mvThemeManager::GetColors()[type][mvThemeConstant].first, &mvThemeManager::GetColors()[type][mvThemeConstant].second });

					}

					// style constants
					for (const auto& item : item_type::GetStyleConstants())
					{
						ModuleConstants.push_back({ std::get<0>(item), std::get<1>(item) });

						static mvAppItemType type;
						long mvThemeConstant = std::get<1>(item);
						decodeType(mvThemeConstant, &type);
						float default_val = std::get<2>(item);
						float max_val = std::get<3>(item);
						const std::string& name = std::get<0>(item);

						mvThemeManager::GetStyles()[type][mvThemeConstant] = default_val;
						mvThemeManager::GetStylesPtr().push_back({ name, mvThemeConstant,
							&mvThemeManager::GetStyles()[type][mvThemeConstant] , max_val});

					}

					// general constants
					for (const auto& item : item_type::GetGeneralConstants())
						ModuleConstants.push_back({ item.first, item.second });

				});
		}

		First_Run = false;
		return ModuleConstants;
	}

	PyMODINIT_FUNC PyInit_core(void)
	{
		static PyModuleDef dearpyguiModule = {
			PyModuleDef_HEAD_INIT, "core", NULL, -1, mvModule_Core::GetMethods(),
			NULL, NULL, NULL, NULL
		};

		PyObject* m;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		const auto& constants = mvModule_Core::GetModuleConstants();

		// handled in the stub file
		for (auto& item : constants)
			PyModule_AddIntConstant(m, item.first.c_str(), item.second);

		auto MarvelError = PyErr_NewException("dearpygui.error", NULL, NULL);
		Py_XINCREF(MarvelError);
		if (PyModule_AddObject(m, "error", MarvelError) < 0) {
			Py_XDECREF(MarvelError);
			Py_CLEAR(MarvelError);
			Py_DECREF(m);
			return NULL;
		}

		return m;
	}
}