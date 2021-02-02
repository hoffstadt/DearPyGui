#pragma once
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvAppItems.h"

namespace Marvel {

	
	//void mv_configure_item(const char* item, mvWindowAppItemConfig config)
	template <typename T>
	void mv_configure_item(const char* item, T config)
	{
		std::shared_ptr<mvAppItem> aitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		aitem->updateConfig(config);
	}

	void mv_start_dearpygui()
	{
		mvApp::GetApp()->start("");

		mvApp::DeleteApp();
		mvEventBus::Reset();
		mvAppLog::Clear();
	}

	void mv_end()
	{
		std::lock_guard<std::mutex> lk(Marvel::mvApp::GetApp()->GetApp()->getMutex());
		Marvel::mvApp::GetApp()->getItemRegistry().popParent();
	}

}