#include "mvModule_DearPyGui.h"
#include "mvInput.h"
#include "mvAppItemCommons.h"
#include "mvViewport.h"
#include "mvFontManager.h"
#include "mvItemRegistry.h"
#include <ImGuiFileDialog.h>
#include <cstdlib>
#include "mvToolManager.h"
#include "mvBuffer.h"

namespace Marvel {

	PyMethodDef* mvModule_DearPyGui::GetSubMethods()
	{

		static std::vector<PyMethodDef> methods;
		methods.clear();

		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {
				using item_type = typename mvItemTypeMap<i>::type;
				methods.push_back(item_type::GetMethodDefinition());
				item_type::FillExtraCommands(methods);
			});

		mvToolManager::FillExtraCommands(methods);
		mvViewport::FillExtraCommands(methods);
		mvApp::FillExtraCommands(methods);
		mvAppItem::FillExtraCommands(methods);
		mvItemRegistry::FillExtraCommands(methods);
		mvFontManager::FillExtraCommands(methods);
		mvCallbackRegistry::FillExtraCommands(methods);
		mvInput::FillExtraCommands(methods);

		methods.push_back({ NULL, NULL, 0, NULL });

		return methods.data();
	}

	const std::map<std::string, mvPythonParser>& mvModule_DearPyGui::GetSubModuleParsers()
	{

		static auto parsers = std::map<std::string, mvPythonParser>();

		if (parsers.empty())
		{
			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {
					using item_type = typename mvItemTypeMap<i>::type;
					item_type::InsertParser(&parsers);
				});

			mvToolManager::InsertParser(&parsers);
			mvViewport::InsertParser(&parsers);
			mvApp::InsertParser(&parsers);
			mvAppItem::InsertParser(&parsers);
			mvItemRegistry::InsertParser(&parsers);
			mvFontManager::InsertParser(&parsers);
			mvCallbackRegistry::InsertParser(&parsers);
			mvInput::InsertParser(&parsers);
		}
		return parsers;
	}

	const std::vector<std::pair<std::string, long>>& mvModule_DearPyGui::GetSubModuleConstants()
	{
		static bool First_Run = true;
		static std::vector<std::pair<std::string, long>> ModuleConstants;

		if (First_Run)
		{
			mvInput::InsertConstants(ModuleConstants);
			mvToolManager::InsertConstants(ModuleConstants);

			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {

					using item_type = typename mvItemTypeMap<i>::type;
					ModuleConstants.push_back({ std::string(mvItemTypeMap<i>::s_class), item_type::s_internal_type });
					for (const auto& item : item_type::GetGeneralConstants())
						ModuleConstants.push_back({ item.first, item.second });

				});

			int j = 0;
			for (int i = MV_RESERVED_UUID_start; i < MV_RESERVED_UUID_start+MV_RESERVED_UUIDs; i++)
			{
				ModuleConstants.push_back({"mvReservedUUID_" + std::to_string(j), (long)i});
				j++;
			}

		}

		First_Run = false;
		return ModuleConstants;
	}

	PyMODINIT_FUNC PyInit__dearpygui(void)
	{
		static PyModuleDef dearpyguiModule = {
			PyModuleDef_HEAD_INIT, "_dearpygui", NULL, -1, mvModule_DearPyGui::GetMethods(),
			NULL, NULL, NULL, NULL
		};

		PyObject* m;
		PymvBufferType.tp_new = PyType_GenericNew;
		if (PyType_Ready(&PymvBufferType) < 0)
			return NULL;

		m = PyModule_Create(&dearpyguiModule);
		if (m == NULL)
			return NULL;

		const auto& constants = mvModule_DearPyGui::GetModuleConstants();

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

		Py_INCREF(&PymvBufferType);
		PyModule_AddObject(m, "mvBuffer", (PyObject*)&PymvBufferType);

		return m;
	}
}