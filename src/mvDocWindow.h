#pragma once

#include <unordered_map>
#include <vector>
#include "mvToolWindow.h"

class mvDocWindow final : public mvToolWindow
{

public:

    mvDocWindow();

	[[nodiscard]] mvUUID getUUID() const override { return MV_TOOL_DOC_UUID; }
    [[nodiscard]] const char* getTitle() const override { return "Internal Documentation"; }

protected:

    void drawWidgets() override;

private:

	void setup();

	int categorySelection = 0;
	const char* m_doc = "None";

	std::vector<const char*> m_categories;

	std::unordered_map<std::string, std::vector<const char*>> m_commandCategories;
	std::unordered_map<std::string, std::vector<const char*>> m_docCategories;

	std::vector<const char*> m_cconstants;

	std::vector<std::pair<std::string, std::string>> m_commands;

	std::vector<std::string> m_constantsValues;

};