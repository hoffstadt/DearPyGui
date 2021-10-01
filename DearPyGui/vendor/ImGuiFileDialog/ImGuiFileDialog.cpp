// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
MIT License

Copyright (c) 2019-2020 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "ImGuiFileDialog.h"

#ifdef __cplusplus

#include <cfloat>
#include <cstring> // stricmp / strcasecmp
#include <cstdarg> // variadic
#include <sstream>
#include <iomanip>
#include <ctime>
#include <sys/stat.h>
#include <cstdio>
// this option need c++17
#ifdef USE_STD_FILESYSTEM
	#include <filesystem>
#endif
#if defined (__EMSCRIPTEN__) // EMSCRIPTEN
	#include <emscripten.h>
#endif // EMSCRIPTEN
#if defined(__WIN32__) || defined(_WIN32)
	#ifndef WIN32
		#define WIN32
	#endif // WIN32
	#define stat _stat
	#define stricmp _stricmp
	#include <cctype>
	// this option need c++17
	#ifdef USE_STD_FILESYSTEM
		#include <Windows.h>
	#else
		#include "dirent/dirent.h" // directly open the dirent file attached to this lib
	#endif // USE_STD_FILESYSTEM
	#define PATH_SEP '\\'
	#ifndef PATH_MAX
		#define PATH_MAX 260
	#endif // PATH_MAX
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__APPLE__) || defined (__EMSCRIPTEN__)
	#define UNIX
	#define stricmp strcasecmp
	#include <sys/types.h>
	// this option need c++17
	#ifndef USE_STD_FILESYSTEM
		#include <dirent.h> 
	#endif // USE_STD_FILESYSTEM
	#define PATH_SEP '/'
#endif // defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__APPLE__)

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
	#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <cstdlib>
#include <algorithm>
#include <iostream>

#ifdef USE_THUMBNAILS
#ifndef DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
#endif // DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#ifndef DONT_DEFINE_AGAIN__STB_IMAGE_RESIZE_IMPLEMENTATION
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#endif // STB_IMAGE_RESIZE_IMPLEMENTATION
#endif // DONT_DEFINE_AGAIN__STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"
#endif // USE_THUMBNAILS

namespace IGFD
{
// float comparisons
#ifndef IS_FLOAT_DIFFERENT
#define IS_FLOAT_DIFFERENT(a,b) (fabs((a) - (b)) > FLT_EPSILON)
#endif // IS_FLOAT_DIFFERENT
#ifndef IS_FLOAT_EQUAL
#define IS_FLOAT_EQUAL(a,b) (fabs((a) - (b)) < FLT_EPSILON)
#endif // IS_FLOAT_EQUAL
// width of filter combobox
#ifndef FILTER_COMBO_WIDTH
#define FILTER_COMBO_WIDTH 150.0f
#endif // FILTER_COMBO_WIDTH
// for lets you define your button widget
// if you have like me a special bi-color button
#ifndef IMGUI_PATH_BUTTON
#define IMGUI_PATH_BUTTON ImGui::Button
#endif // IMGUI_PATH_BUTTON
#ifndef IMGUI_BUTTON
#define IMGUI_BUTTON ImGui::Button
#endif // IMGUI_BUTTON
// locales
#ifndef createDirButtonString
#define createDirButtonString "+"
#endif // createDirButtonString
#ifndef okButtonString
#define okButtonString "OK"
#endif // okButtonString
#ifndef cancelButtonString
#define cancelButtonString "Cancel"
#endif // cancelButtonString
#ifndef resetButtonString
#define resetButtonString "R"
#endif // resetButtonString
#ifndef drivesButtonString
#define drivesButtonString "Drives"
#endif // drivesButtonString
#ifndef editPathButtonString
#define editPathButtonString "E"
#endif // editPathButtonString
#ifndef searchString
#define searchString "Search :"
#endif // searchString
#ifndef dirEntryString
#define dirEntryString "[Dir]"
#endif // dirEntryString
#ifndef linkEntryString
#define linkEntryString "[Link]"
#endif // linkEntryString
#ifndef fileEntryString
#define fileEntryString "[File]"
#endif // fileEntryString
#ifndef fileNameString
#define fileNameString "File Name :"
#endif // fileNameString
#ifndef dirNameString
#define dirNameString "Directory Path :"
#endif // dirNameString
#ifndef buttonResetSearchString
#define buttonResetSearchString "Reset search"
#endif // buttonResetSearchString
#ifndef buttonDriveString
#define buttonDriveString "Drives"
#endif // buttonDriveString
#ifndef buttonEditPathString
#define buttonEditPathString "Edit path\nYou can also right click on path buttons"
#endif // buttonEditPathString
#ifndef buttonResetPathString
#define buttonResetPathString "Reset to current directory"
#endif // buttonResetPathString
#ifndef buttonCreateDirString
#define buttonCreateDirString "Create Directory"
#endif // buttonCreateDirString
#ifndef tableHeaderAscendingIcon
#define tableHeaderAscendingIcon "A|"
#endif // tableHeaderAscendingIcon
#ifndef tableHeaderDescendingIcon
#define tableHeaderDescendingIcon "D|"
#endif // tableHeaderDescendingIcon
#ifndef tableHeaderFileNameString
#define tableHeaderFileNameString "File name"
#endif // tableHeaderFileNameString
#ifndef tableHeaderFileTypeString
#define tableHeaderFileTypeString "Type"
#endif // tableHeaderFileTypeString
#ifndef tableHeaderFileSizeString
#define tableHeaderFileSizeString "Size"
#endif // tableHeaderFileSizeString
#ifndef tableHeaderFileDateString
#define tableHeaderFileDateString "Date"
#endif // tableHeaderFileDateString
#ifndef OverWriteDialogTitleString
#define OverWriteDialogTitleString "The file Already Exist !"
#endif // OverWriteDialogTitleString
#ifndef OverWriteDialogMessageString
#define OverWriteDialogMessageString "Would you like to OverWrite it ?"
#endif // OverWriteDialogMessageString
#ifndef OverWriteDialogConfirmButtonString
#define OverWriteDialogConfirmButtonString "Confirm"
#endif // OverWriteDialogConfirmButtonString
#ifndef OverWriteDialogCancelButtonString
#define OverWriteDialogCancelButtonString "Cancel"
#endif // OverWriteDialogCancelButtonString
// see strftime functionin <ctime> for customize
#ifndef DateTimeFormat
#define DateTimeFormat "%Y/%m/%d %H:%M"
#endif // DateTimeFormat
#ifdef USE_THUMBNAILS
#ifndef tableHeaderFileThumbnailsString
#define tableHeaderFileThumbnailsString "Thumbnails"
#endif // tableHeaderFileThumbnailsString
#ifndef DisplayMode_FilesList_ButtonString
#define DisplayMode_FilesList_ButtonString "FL"
#endif // DisplayMode_FilesList_ButtonString
#ifndef DisplayMode_FilesList_ButtonHelp
#define DisplayMode_FilesList_ButtonHelp "File List"
#endif // DisplayMode_FilesList_ButtonHelp
#ifndef DisplayMode_ThumbailsList_ButtonString
#define DisplayMode_ThumbailsList_ButtonString "TL"
#endif // DisplayMode_ThumbailsList_ButtonString
#ifndef DisplayMode_ThumbailsList_ButtonHelp
#define DisplayMode_ThumbailsList_ButtonHelp "Thumbnails List"
#endif // DisplayMode_ThumbailsList_ButtonHelp
#ifndef DisplayMode_ThumbailsGrid_ButtonString
#define DisplayMode_ThumbailsGrid_ButtonString "TG"
#endif // DisplayMode_ThumbailsGrid_ButtonString
#ifndef DisplayMode_ThumbailsGrid_ButtonHelp
#define DisplayMode_ThumbailsGrid_ButtonHelp "Thumbnails Grid"
#endif // DisplayMode_ThumbailsGrid_ButtonHelp
#ifndef DisplayMode_ThumbailsList_ImageHeight 
#define DisplayMode_ThumbailsList_ImageHeight 32.0f
#endif // DisplayMode_ThumbailsList_ImageHeight
#ifndef IMGUI_RADIO_BUTTON
	inline bool inRadioButton(const char* vLabel, bool vToggled)
	{
		bool pressed = false;

		if (vToggled)
		{
			ImVec4 bua = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
			ImVec4 te = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			ImGui::PushStyleColor(ImGuiCol_Button, te);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, te);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, te);
			ImGui::PushStyleColor(ImGuiCol_Text, bua);
		}

		pressed = IMGUI_BUTTON(vLabel);

		if (vToggled)
		{
			ImGui::PopStyleColor(4); //-V112
		}

		return pressed;
	}
#define IMGUI_RADIO_BUTTON inRadioButton
#endif // IMGUI_RADIO_BUTTON
#endif  // USE_THUMBNAILS
#ifdef USE_BOOKMARK
#ifndef defaultBookmarkPaneWith
#define defaultBookmarkPaneWith 150.0f
#endif // defaultBookmarkPaneWith
#ifndef bookmarksButtonString
#define bookmarksButtonString "Bookmark"
#endif // bookmarksButtonString
#ifndef bookmarksButtonHelpString
#define bookmarksButtonHelpString "Bookmark"
#endif // bookmarksButtonHelpString
#ifndef addBookmarkButtonString
#define addBookmarkButtonString "+"
#endif // addBookmarkButtonString
#ifndef removeBookmarkButtonString
#define removeBookmarkButtonString "-"
#endif // removeBookmarkButtonString
#ifndef IMGUI_TOGGLE_BUTTON
	inline bool inToggleButton(const char* vLabel, bool* vToggled)
	{
		bool pressed = false;

		if (vToggled && *vToggled)
		{
			ImVec4 bua = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
			//ImVec4 buh = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
			//ImVec4 bu = ImGui::GetStyleColorVec4(ImGuiCol_Button);
			ImVec4 te = ImGui::GetStyleColorVec4(ImGuiCol_Text);
			ImGui::PushStyleColor(ImGuiCol_Button, te);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, te);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, te);
			ImGui::PushStyleColor(ImGuiCol_Text, bua);
		}

		pressed = IMGUI_BUTTON(vLabel);

		if (vToggled && *vToggled)
		{
			ImGui::PopStyleColor(4); //-V112
		}

		if (vToggled && pressed)
			*vToggled = !*vToggled;

		return pressed;
	}
#define IMGUI_TOGGLE_BUTTON inToggleButton
#endif // IMGUI_TOGGLE_BUTTON
#endif // USE_BOOKMARK

	/////////////////////////////////////////////////////////////////////////////////////
	//// INLINE FUNCTIONS ///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

#ifndef USE_STD_FILESYSTEM
	inline int inAlphaSort(const struct dirent** a, const struct dirent** b)
	{
		return strcoll((*a)->d_name, (*b)->d_name);
	}
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILE EXTENTIONS INFOS //////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	IGFD::FileExtentionInfos::FileExtentionInfos() : color(0, 0, 0, 0)
	{ 

	}

	IGFD::FileExtentionInfos::FileExtentionInfos(const ImVec4& vColor, const std::string& vIcon)
	{ 
		color = vColor; 
		icon = vIcon; 
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILE INFOS /////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	// https://github.com/ocornut/imgui/issues/1720
	bool IGFD::Utils::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
	{
		using namespace ImGui;
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiID id = window->GetID("##Splitter");
		ImRect bb;
		bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
		bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
		return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 1.0f);
	}

#ifdef WIN32
	bool IGFD::Utils::WReplaceString(std::wstring& str, const std::wstring& oldStr, const std::wstring& newStr)
	{
		bool found = false;
		size_t pos = 0;
		while ((pos = str.find(oldStr, pos)) != std::wstring::npos)
		{
			found = true;
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		return found;
	}

	std::vector<std::wstring> IGFD::Utils::WSplitStringToVector(const std::wstring& text, char delimiter, bool pushEmpty)
	{
		std::vector<std::wstring> arr;
		if (!text.empty())
		{
			std::wstring::size_type start = 0;
			std::wstring::size_type end = text.find(delimiter, start);
			while (end != std::wstring::npos)
			{
				std::wstring token = text.substr(start, end - start);
				if (!token.empty() || (token.empty() && pushEmpty)) //-V728
					arr.push_back(token);
				start = end + 1;
				end = text.find(delimiter, start);
			}
			std::wstring token = text.substr(start);
			if (!token.empty() || (token.empty() && pushEmpty)) //-V728
				arr.push_back(token);
		}
		return arr;
	}

	std::wstring IGFD::Utils::string_to_wstring(const std::string& str)
	{
		std::wstring ret;
		if (!str.empty())
		{
			size_t sz = std::mbstowcs(nullptr, str.c_str(), str.size());
			if (sz)
			{
				ret.resize(sz);
				std::mbstowcs((wchar_t*)ret.data(), str.c_str(), sz);
			}
		}
		return ret;
	}

	std::string IGFD::Utils::wstring_to_string(const std::wstring& str)
	{
		std::string ret;
		if (!str.empty())
		{
			size_t sz = std::wcstombs(nullptr, str.c_str(), str.size());
			if (sz)
			{
				ret.resize(sz);
				std::wcstombs((char*)ret.data(), str.c_str(), sz);
			}
		}
		return ret;
	}
#endif // WIN32

	bool IGFD::Utils::ReplaceString(std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		bool found = false;
		size_t pos = 0;
		while ((pos = str.find(oldStr, pos)) != std::string::npos)
		{
			found = true;
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		return found;
	}

	std::vector<std::string> IGFD::Utils::SplitStringToVector(const std::string& text, char delimiter, bool pushEmpty)
	{
		std::vector<std::string> arr;
		if (!text.empty())
		{
			size_t start = 0;
			size_t end = text.find(delimiter, start);
			while (end != std::string::npos)
			{
				auto token = text.substr(start, end - start);
				if (!token.empty() || (token.empty() && pushEmpty)) //-V728
					arr.push_back(token);
				start = end + 1;
				end = text.find(delimiter, start);
			}
			auto token = text.substr(start);
			if (!token.empty() || (token.empty() && pushEmpty)) //-V728
				arr.push_back(token);
		}
		return arr;
	}

	std::vector<std::string> IGFD::Utils::GetDrivesList()
	{
		std::vector<std::string> res;

#ifdef WIN32
		const DWORD mydrives = 2048;
		char lpBuffer[2048];
#define mini(a,b) (((a) < (b)) ? (a) : (b))
		const DWORD countChars = mini(GetLogicalDriveStringsA(mydrives, lpBuffer), 2047);
#undef mini
		if (countChars > 0)
		{
			std::string var = std::string(lpBuffer, (size_t)countChars);
			IGFD::Utils::ReplaceString(var, "\\", "");
			res = IGFD::Utils::SplitStringToVector(var, '\0', false);
		}
#endif // WIN32

		return res;
	}

	bool IGFD::Utils::IsDirectoryExist(const std::string& name)
	{
		bool bExists = false;

		if (!name.empty())
		{
#ifdef USE_STD_FILESYSTEM
			namespace fs = std::filesystem;
#ifdef WIN32
			std::wstring wname = IGFD::Utils::string_to_wstring(name.c_str());
			fs::path pathName = fs::path(wname);
#else
			fs::path pathName = fs::path(name);
#endif
			bExists = fs::is_directory(pathName);
#else
			DIR* pDir = nullptr;
			pDir = opendir(name.c_str());
			if (pDir != nullptr)
			{
				bExists = true;
				(void)closedir(pDir);
			}
#endif // USE_STD_FILESYSTEM
		}

		return bExists;    // this is not a directory!
	}

	bool IGFD::Utils::CreateDirectoryIfNotExist(const std::string& name)
	{
		bool res = false;

		if (!name.empty())
		{
			if (!IsDirectoryExist(name))
			{
#ifdef WIN32
#ifdef USE_STD_FILESYSTEM
				namespace fs = std::filesystem;
				std::wstring wname = IGFD::Utils::string_to_wstring(name.c_str());
				fs::path pathName = fs::path(wname);
				res = fs::create_directory(pathName);
#else
				std::wstring wname = IGFD::Utils::string_to_wstring(name);
				if (CreateDirectoryW(wname.c_str(), nullptr))
				{
					res = true;
				}
#endif // USE_STD_FILESYSTEM
#elif defined(__EMSCRIPTEN__)
				std::string str = std::string("FS.mkdir('") + name + "');";
				emscripten_run_script(str.c_str());
				res = true;
#elif defined(UNIX)
				char buffer[PATH_MAX] = {};
				snprintf(buffer, PATH_MAX, "mkdir -p %s", name.c_str());
				const int dir_err = std::system(buffer);
				if (dir_err != -1)
				{
					res = true;
				}
#endif // WIN32
				if (!res) {
					std::cout << "Error creating directory " << name << std::endl;
				}
			}
		}

		return res;
	}

#ifdef USE_STD_FILESYSTEM
	// https://github.com/aiekick/ImGuiFileDialog/issues/54
	IGFD::Utils::PathStruct IGFD::Utils::ParsePathFileName(const std::string& vPathFileName)
	{
		namespace fs = std::filesystem;
		PathStruct res;
		if (vPathFileName.empty())
			return res;

		auto fsPath = fs::path(vPathFileName);

		if (fs::is_regular_file(fsPath)) {
			res.name = fsPath.string();
			res.path = fsPath.parent_path().string();
			res.isOk = true;
		}

		return res;
	}
#else
	IGFD::Utils::PathStruct IGFD::Utils::ParsePathFileName(const std::string& vPathFileName)
	{
		PathStruct res;

		if (!vPathFileName.empty())
		{
			std::string pfn = vPathFileName;
			std::string separator(1u, PATH_SEP);
			IGFD::Utils::ReplaceString(pfn, "\\", separator);
			IGFD::Utils::ReplaceString(pfn, "/", separator);

			size_t lastSlash = pfn.find_last_of(separator);
			if (lastSlash != std::string::npos)
			{
				res.name = pfn.substr(lastSlash + 1);
				res.path = pfn.substr(0, lastSlash);
				res.isOk = true;
			}

			size_t lastPoint = pfn.find_last_of('.');
			if (lastPoint != std::string::npos)
			{
				if (!res.isOk)
				{
					res.name = pfn;
					res.isOk = true;
				}
				res.ext = pfn.substr(lastPoint + 1);
				IGFD::Utils::ReplaceString(res.name, "." + res.ext, "");
			}

			if (!res.isOk)
			{
				res.name = pfn;
				res.isOk = true;
			}
		}

		return res;
	}
#endif // USE_STD_FILESYSTEM
	void IGFD::Utils::AppendToBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr)
	{
		std::string st = vStr;
		size_t len = vBufferLen - 1u;
		size_t slen = strlen(vBuffer);

		if (!st.empty() && st != "\n")
		{
			IGFD::Utils::ReplaceString(st, "\n", "");
			IGFD::Utils::ReplaceString(st, "\r", "");
		}
		vBuffer[slen] = '\0';
		std::string str = std::string(vBuffer);
		//if (!str.empty()) str += "\n";
		str += vStr;
		if (len > str.size()) len = str.size();
#ifdef MSVC
		strncpy_s(vBuffer, vBufferLen, str.c_str(), len);
#else // MSVC
		strncpy(vBuffer, str.c_str(), len);
#endif // MSVC
		vBuffer[len] = '\0';
	}

	void IGFD::Utils::ResetBuffer(char* vBuffer)
	{
		vBuffer[0] = '\0';
	}

	void IGFD::Utils::SetBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr)
	{
		ResetBuffer(vBuffer);
		AppendToBuffer(vBuffer, vBufferLen, vStr);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILE INFOS /////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	bool IGFD::FileInfos::IsTagFound(const std::string& vTag) const
	{
		if (!vTag.empty())
		{
			if (fileName_optimized == "..") return true;

			return
				fileName_optimized.find(vTag) != std::string::npos ||	// first try wihtout case and accents
				fileName.find(vTag) != std::string::npos;				// second if searched with case and accents
		}

		// if tag is empty => its a special case but all is found
		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// SEARCH MANAGER /////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	void IGFD::SearchManager::Clear()
	{
		puSearchTag.clear();
		IGFD::Utils::ResetBuffer(puSearchBuffer);
	}

	void IGFD::SearchManager::DrawSearchBar(FileDialogInternal& vFileDialogInternal)
	{
		// search field
		if (IMGUI_BUTTON(resetButtonString "##BtnImGuiFileDialogSearchField"))
		{
			Clear();
			vFileDialogInternal.puFileManager.ApplyFilteringOnFileList(vFileDialogInternal);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(buttonResetSearchString);
		ImGui::SameLine();
		ImGui::Text(searchString);
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		bool edited = ImGui::InputText("##InputImGuiFileDialogSearchField", puSearchBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
		if (ImGui::GetItemID() == ImGui::GetActiveID())
			puSearchInputIsActive = true;
		ImGui::PopItemWidth();
		if (edited)
		{
			puSearchTag = puSearchBuffer;
			vFileDialogInternal.puFileManager.ApplyFilteringOnFileList(vFileDialogInternal);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILTER INFOS ///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	void IGFD::FilterManager::FilterInfosStruct::clear()
	{
		filter.clear(); 
		collectionfilters.clear();
	}

	bool IGFD::FilterManager::FilterInfosStruct::empty() const
	{
		return filter.empty() && collectionfilters.empty();
	}

	bool IGFD::FilterManager::FilterInfosStruct::exist(const std::string& vFilter) const
	{
		return filter == vFilter || (collectionfilters.find(vFilter) != collectionfilters.end());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILTER MANAGER /////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	void IGFD::FilterManager::ParseFilters(const char* vFilters)
	{
		prParsedFilters.clear();

		if (vFilters)
			puDLGFilters = vFilters;			// file mode
		else
			puDLGFilters.clear();				// directory mode

		if (!puDLGFilters.empty())
		{
			// ".*,.cpp,.h,.hpp"
			// "Source files{.cpp,.h,.hpp},Image files{.png,.gif,.jpg,.jpeg},.md"

			bool currentFilterFound = false;

			size_t nan = std::string::npos;
			size_t p = 0, lp = 0;
			while ((p = puDLGFilters.find_first_of("{,", p)) != nan)
			{
				FilterInfosStruct infos;

				if (puDLGFilters[p] == '{') // {
				{
					infos.filter = puDLGFilters.substr(lp, p - lp);
					p++;
					lp = puDLGFilters.find('}', p);
					if (lp != nan)
					{
						std::string fs = puDLGFilters.substr(p, lp - p);
						auto arr = IGFD::Utils::SplitStringToVector(fs, ',', false);
						for (auto a : arr)
						{
							infos.collectionfilters.emplace(a);
						}
					}
					p = lp + 1;
				}
				else // ,
				{
					infos.filter = puDLGFilters.substr(lp, p - lp);
					p++;
				}

				if (!currentFilterFound && prSelectedFilter.filter == infos.filter)
				{
					currentFilterFound = true;
					prSelectedFilter = infos;
				}

				lp = p;
				if (!infos.empty())
					prParsedFilters.emplace_back(infos);
			}

			std::string token = puDLGFilters.substr(lp);
			if (!token.empty())
			{
				FilterInfosStruct infos;
				infos.filter = token;
				prParsedFilters.emplace_back(infos);
			}

			if (!currentFilterFound)
				if (!prParsedFilters.empty())
					prSelectedFilter = *prParsedFilters.begin();
		}
	}

	void IGFD::FilterManager::SetSelectedFilterWithExt(const std::string& vFilter)
	{
		if (!prParsedFilters.empty())
		{
			if (!vFilter.empty())
			{
				// std::map<std::string, FilterInfosStruct>
				for (const auto& infos : prParsedFilters)
				{
					if (vFilter == infos.filter)
					{
						prSelectedFilter = infos;
					}
					else
					{
						// maybe this ext is in an extention so we will 
						// explore the collections is they are existing
						for (const auto& filter : infos.collectionfilters)
						{
							if (vFilter == filter)
							{
								prSelectedFilter = infos;
							}
						}
					}
				}
			}

			if (prSelectedFilter.empty())
				prSelectedFilter = *prParsedFilters.begin();
		}
	}

	void IGFD::FilterManager::SetExtentionInfos(const std::string& vFilter, const FileExtentionInfos& vInfos)
	{
		prFileExtentionInfos[vFilter] = vInfos;
	}

	void IGFD::FilterManager::SetExtentionInfos(const std::string& vFilter, const ImVec4& vColor, const std::string& vIcon)
	{
		prFileExtentionInfos[vFilter] = FileExtentionInfos(vColor, vIcon);
	}

	bool IGFD::FilterManager::GetExtentionInfos(const std::string& vFilter, ImVec4* vOutColor, std::string* vOutIcon)
	{
		if (vOutColor)
		{
			if (prFileExtentionInfos.find(vFilter) != prFileExtentionInfos.end()) // found
			{
				*vOutColor = prFileExtentionInfos[vFilter].color;
				if (vOutIcon)
				{
					*vOutIcon = prFileExtentionInfos[vFilter].icon;
				}
				return true;
			}
		}
		return false;
	}

	void IGFD::FilterManager::ClearExtentionInfos()
	{
		prFileExtentionInfos.clear();
	}

	bool IGFD::FilterManager::IsCoveredByFilters(const std::string& vTag) const
	{
		if (!puDLGFilters.empty() && !prSelectedFilter.empty())
		{
			// check if current file extention is covered by current filter
			// we do that here, for avoid doing that during filelist display
			// for better fps
			if (prSelectedFilter.exist(vTag) || prSelectedFilter.filter == ".*")
			{
				return true;
			}
		}

		return false;
	}

	bool IGFD::FilterManager::DrawFilterComboBox(FileDialogInternal& vFileDialogInternal)
	{
		// combobox of filters
		if (!puDLGFilters.empty())
		{
			ImGui::SameLine();

			bool needToApllyNewFilter = false;

			ImGui::PushItemWidth(FILTER_COMBO_WIDTH);
			if (ImGui::BeginCombo("##Filters", prSelectedFilter.filter.c_str(), ImGuiComboFlags_None))
			{
				intptr_t i = 0;
				for (const auto& filter : prParsedFilters)
				{
					const bool item_selected = (filter.filter == prSelectedFilter.filter);
					ImGui::PushID((void*)(intptr_t)i++);
					if (ImGui::Selectable(filter.filter.c_str(), item_selected))
					{
						prSelectedFilter = filter;
						needToApllyNewFilter = true;
					}
					ImGui::PopID();
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			if (needToApllyNewFilter)
			{
				vFileDialogInternal.puFileManager.OpenCurrentPath(vFileDialogInternal);
			}

			return needToApllyNewFilter;
		}

		return false;
	}

	IGFD::FilterManager::FilterInfosStruct IGFD::FilterManager::GetSelectedFilter()
	{
		return prSelectedFilter;
	}

	std::string IGFD::FilterManager::ReplaceExtentionWithCurrentFilter(const std::string vFile) const
	{
		auto result = vFile;

		if (!result.empty())
		{
			// if not a collection we can replace the filter by the extention we want
			if (prSelectedFilter.collectionfilters.empty())
			{
				size_t lastPoint = vFile.find_last_of('.');
				if (lastPoint != std::string::npos)
				{
					result = result.substr(0, lastPoint);
				}

				result += prSelectedFilter.filter;
			}
		}

		return result;
	}
	
	void IGFD::FilterManager::SetDefaultFilterIfNotDefined()
	{
		if (prSelectedFilter.empty() && // no filter selected
			!prParsedFilters.empty()) // filter exist
			prSelectedFilter = *prParsedFilters.begin(); // we take the first filter
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILE MANAGER ///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	IGFD::FileManager::FileManager()
	{
		puFsRoot = std::string(1u, PATH_SEP);
	}

	void IGFD::FileManager::OpenCurrentPath(const FileDialogInternal& vFileDialogInternal)
	{
		puShowDrives = false;
		ClearComposer();
		ClearFileLists();
		if (puDLGDirectoryMode) // directory mode
			SetDefaultFileName(".");
		else
			SetDefaultFileName(puDLGDefaultFileName);
		ScanDir(vFileDialogInternal, GetCurrentPath());
	}

	void IGFD::FileManager::SortFields(const FileDialogInternal& vFileDialogInternal, const SortingFieldEnum& vSortingField, const bool& vCanChangeOrder)
	{
		if (vSortingField != SortingFieldEnum::FIELD_NONE)
		{
			puHeaderFileName = tableHeaderFileNameString;
			puHeaderFileType = tableHeaderFileTypeString;
			puHeaderFileSize = tableHeaderFileSizeString;
			puHeaderFileDate = tableHeaderFileDateString;
#ifdef USE_THUMBNAILS
			puHeaderFileThumbnails = tableHeaderFileThumbnailsString;
#endif // #ifdef USE_THUMBNAILS
		}

		if (vSortingField == SortingFieldEnum::FIELD_FILENAME)
		{
			if (vCanChangeOrder && puSortingField == vSortingField)
				puSortingDirection[0] = !puSortingDirection[0];

			if (puSortingDirection[0])
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileName = tableHeaderDescendingIcon + puHeaderFileName;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						// this code fail in c:\\Users with the link "All users". got a invalid comparator
						/*
						// use code from https://github.com/jackm97/ImGuiFileDialog/commit/bf40515f5a1de3043e60562dc1a494ee7ecd3571
						// strict ordering for file/directory types beginning in '.'
						// common on Linux platforms
						if (a->fileName[0] == '.' && b->fileName[0] != '.')
							return false;
						if (a->fileName[0] != '.' && b->fileName[0] == '.')
							return true;
						if (a->fileName[0] == '.' && b->fileName[0] == '.')
						{
							return (stricmp(a->fileName.c_str(), b->fileName.c_str()) < 0); // sort in insensitive case
						}
						*/
						if (a->fileType != b->fileType) return (a->fileType == 'd'); // directory in first
						return (stricmp(a->fileName.c_str(), b->fileName.c_str()) < 0); // sort in insensitive case
					});
			}
			else
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileName = tableHeaderAscendingIcon + puHeaderFileName;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						// this code fail in c:\\Users with the link "All users". got a invalid comparator
						/*
						// use code from https://github.com/jackm97/ImGuiFileDialog/commit/bf40515f5a1de3043e60562dc1a494ee7ecd3571
						// strict ordering for file/directory types beginning in '.'
						// common on Linux platforms
						if (a->fileName[0] == '.' && b->fileName[0] != '.')
							return false;
						if (a->fileName[0] != '.' && b->fileName[0] == '.')
							return true;
						if (a->fileName[0] == '.' && b->fileName[0] == '.')
						{
							return (stricmp(a->fileName.c_str(), b->fileName.c_str()) > 0); // sort in insensitive case
						}
						*/
						return (stricmp(a->fileName.c_str(), b->fileName.c_str()) > 0); // sort in insensitive case
					});
			}
		}
		else if (vSortingField == SortingFieldEnum::FIELD_TYPE)
		{
			if (vCanChangeOrder && puSortingField == vSortingField)
				puSortingDirection[1] = !puSortingDirection[1];

			if (puSortingDirection[1])
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileType = tableHeaderDescendingIcon + puHeaderFileType;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType == 'd'); // directory in first
						return (a->fileExt < b->fileExt); // else
					});
			}
			else
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileType = tableHeaderAscendingIcon + puHeaderFileType;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType != 'd'); // directory in last
						return (a->fileExt > b->fileExt); // else
					});
			}
		}
		else if (vSortingField == SortingFieldEnum::FIELD_SIZE)
		{
			if (vCanChangeOrder && puSortingField == vSortingField)
				puSortingDirection[2] = !puSortingDirection[2];

			if (puSortingDirection[2])
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileSize = tableHeaderDescendingIcon + puHeaderFileSize;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType == 'd'); // directory in first
						return (a->fileSize < b->fileSize); // else
					});
			}
			else
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileSize = tableHeaderAscendingIcon + puHeaderFileSize;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType != 'd'); // directory in last
						return (a->fileSize > b->fileSize); // else
					});
			}
		}
		else if (vSortingField == SortingFieldEnum::FIELD_DATE)
		{
			if (vCanChangeOrder && puSortingField == vSortingField)
				puSortingDirection[3] = !puSortingDirection[3];

			if (puSortingDirection[3])
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileDate = tableHeaderDescendingIcon + puHeaderFileDate;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType == 'd'); // directory in first
						return (a->fileModifDate < b->fileModifDate); // else
					});
			}
			else
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileDate = tableHeaderAscendingIcon + puHeaderFileDate;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType != 'd'); // directory in last
						return (a->fileModifDate > b->fileModifDate); // else
					});
			}
		}
#ifdef USE_THUMBNAILS
		else if (vSortingField == SortingFieldEnum::FIELD_THUMBNAILS)
		{
			if (vCanChangeOrder && puSortingField == vSortingField)
				puSortingDirection[4] = !puSortingDirection[4];

			// we will compare thumbnails by :
			// 1) width 
			// 2) height

			if (puSortingDirection[4])
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileThumbnails = tableHeaderDescendingIcon + puHeaderFileThumbnails;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType == 'd'); // directory in first
						if (a->thumbnailInfo.textureWidth == b->thumbnailInfo.textureWidth)
							return (a->thumbnailInfo.textureHeight < b->thumbnailInfo.textureHeight);
						return (a->thumbnailInfo.textureWidth < b->thumbnailInfo.textureWidth);
					});
			}

			else
			{
#ifdef USE_CUSTOM_SORTING_ICON
				puHeaderFileThumbnails = tableHeaderAscendingIcon + puHeaderFileThumbnails;
#endif // USE_CUSTOM_SORTING_ICON
				std::sort(prFileList.begin(), prFileList.end(),
					[](const std::shared_ptr<FileInfos>& a, const std::shared_ptr<FileInfos>& b) -> bool
					{
						if (!a.use_count() || !b.use_count())
							return false;

						if (a->fileType != b->fileType) return (a->fileType != 'd'); // directory in last
						if (a->thumbnailInfo.textureWidth == b->thumbnailInfo.textureWidth)
							return (a->thumbnailInfo.textureHeight > b->thumbnailInfo.textureHeight);
						return (a->thumbnailInfo.textureWidth > b->thumbnailInfo.textureWidth);
					});
			}
		}
#endif // USE_THUMBNAILS

		if (vSortingField != SortingFieldEnum::FIELD_NONE)
		{
			puSortingField = vSortingField;
		}

		ApplyFilteringOnFileList(vFileDialogInternal);
	}

	void IGFD::FileManager::ClearFileLists()
	{
		prFilteredFileList.clear();
		prFileList.clear();
	}

	std::string IGFD::FileManager::prOptimizeFilenameForSearchOperations(const std::string& vFileName)
	{
		auto fileName = vFileName;
		// convert to lower case
		for (char& c : fileName)
			c = (char)std::tolower(c);
		return fileName;
	}

	void IGFD::FileManager::AddFile(const FileDialogInternal& vFileDialogInternal, const std::string& vPath, const std::string& vFileName, const char& vFileType)
	{
		auto infos = std::make_shared<FileInfos>();

		infos->filePath = vPath;
		infos->fileName = vFileName;
		infos->fileName_optimized = prOptimizeFilenameForSearchOperations(infos->fileName);

		if (infos->fileName.empty() || (infos->fileName == "." && !vFileDialogInternal.puFilterManager.puDLGFilters.empty())) return; // filename empty or filename is the current dir '.'
		if (infos->fileName != ".." && (vFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_DontShowHiddenFiles) && infos->fileName[0] == '.') // dont show hidden files
			if (!vFileDialogInternal.puFilterManager.puDLGFilters.empty() || (vFileDialogInternal.puFilterManager.puDLGFilters.empty() && infos->fileName != ".")) // except "." if in directory mode
				return;

		infos->fileType = vFileType;

		if (infos->fileType == 'f' ||
			infos->fileType == 'l') // link can have the same extention of a file
		{
			size_t lpt = infos->fileName.find_last_of('.');
			if (lpt != std::string::npos)
			{
				infos->fileExt = infos->fileName.substr(lpt);
			}

			if (!vFileDialogInternal.puFilterManager.IsCoveredByFilters(infos->fileExt))
			{
				return;
			}
		}

		prCompleteFileInfos(infos);
		prFileList.push_back(infos);
	}

	void IGFD::FileManager::ScanDir(const FileDialogInternal& vFileDialogInternal, const std::string& vPath)
	{
		std::string	path = vPath;

		if (prCurrentPathDecomposition.empty())
		{
			SetCurrentDir(path);
		}

		if (!prCurrentPathDecomposition.empty())
		{
#ifdef WIN32
			if (path == puFsRoot)
				path += std::string(1u, PATH_SEP);
#endif // WIN32

			ClearFileLists();

#ifdef USE_STD_FILESYSTEM
			//const auto wpath = IGFD::Utils::WGetString(path.c_str());
			const std::filesystem::path fspath(path);
			const auto dir_iter = std::filesystem::directory_iterator(fspath);
			AddFile(vFileDialogInternal, path, "..", 'd');
			for (const auto& file : dir_iter)
			{
				char fileType = 0;
				if (file.is_symlink())
					fileType = 'l';
				else if (file.is_directory())
					fileType = 'd';
				else
					fileType = 'f';
				auto fileName = file.path().filename().string();
				AddFile(vFileDialogInternal, path, fileName, fileType);
			}
#else // dirent
			struct dirent** files = nullptr;
			int n = scandir(path.c_str(), &files, nullptr, inAlphaSort);
			if (n > 0)
			{
				for (int i = 0; i < n; i++)
				{
					struct dirent* ent = files[i];

					char fileType = 0;
					switch (ent->d_type)
					{
					case DT_REG:
						fileType = 'f'; break;
					case DT_DIR:
						fileType = 'd'; break;
					case DT_LNK:
						fileType = 'l'; break;
					}

					auto fileName = ent->d_name;

					AddFile(vFileDialogInternal, path, fileName, fileType);
				}

				for (int i = 0; i < n; i++)
				{
					free(files[i]);
				}

				free(files);
			}
#endif // USE_STD_FILESYSTEM

			SortFields(vFileDialogInternal, puSortingField, false);
		}
	}

	bool IGFD::FileManager::GetDrives()
	{
		auto drives = IGFD::Utils::GetDrivesList();
		if (!drives.empty())
		{
			prCurrentPath.clear();
			prCurrentPathDecomposition.clear();
			ClearFileLists();
			for (auto& drive : drives)
			{
				auto info = std::make_shared<FileInfos>();
				info->fileName = drive;
				info->fileName_optimized = drive;
				prOptimizeFilenameForSearchOperations(info->fileName_optimized);
				info->fileType = 'd';

				if (!info->fileName.empty())
				{
					prFileList.push_back(info);
				}
			}
			puShowDrives = true;
			return true;
		}
		return false;
	}

	bool IGFD::FileManager::IsComposerEmpty()
	{
		return prCurrentPathDecomposition.empty();
	}
	
	size_t IGFD::FileManager::GetComposerSize()
	{
		return prCurrentPathDecomposition.size();
	}

	bool IGFD::FileManager::IsFileListEmpty()
	{
		return prFileList.empty();
	}

	size_t IGFD::FileManager::GetFullFileListSize()
	{
		return prFileList.size();
	}

	std::shared_ptr<FileInfos> IGFD::FileManager::GetFullFileAt(size_t vIdx)
	{
		if (vIdx < prFileList.size())
			return prFileList[vIdx];
		return nullptr;
	}

	bool IGFD::FileManager::IsFilteredListEmpty()
	{
		return prFilteredFileList.empty();
	}

	size_t IGFD::FileManager::GetFilteredListSize()
	{
		return prFilteredFileList.size();
	}

	std::shared_ptr<FileInfos> IGFD::FileManager::GetFilteredFileAt(size_t vIdx)
	{
		if (vIdx < prFilteredFileList.size())
			return prFilteredFileList[vIdx];
		return nullptr;
	}

	bool IGFD::FileManager::IsFileNameSelected(const std::string& vFileName)
	{
		return prSelectedFileNames.find(vFileName) != prSelectedFileNames.end();
	}

	std::string IGFD::FileManager::GetBack()
	{
		return prCurrentPathDecomposition.back();
	}

	void IGFD::FileManager::ClearComposer()
	{
		prCurrentPathDecomposition.clear();
	}

	void IGFD::FileManager::ClearAll()
	{
		ClearComposer();
		ClearFileLists();
	}

	void IGFD::FileManager::ApplyFilteringOnFileList(const FileDialogInternal& vFileDialogInternal)
	{
		prFilteredFileList.clear();
		for (const auto& file : prFileList)
		{
			if (!file.use_count())
				continue;
			bool show = true;
			if (!file->IsTagFound(vFileDialogInternal.puSearchManager.puSearchTag))  // if search tag
				show = false;
			if (puDLGDirectoryMode && file->fileType != 'd') // directory mode
				show = false;
			if (show)
				prFilteredFileList.push_back(file);
		}
	}

	std::string IGFD::FileManager::prRoundNumber(double vvalue, int n)
	{
		std::stringstream tmp;
		tmp << std::setprecision(n) << std::fixed << vvalue;
		return tmp.str();
	}

	std::string IGFD::FileManager::prFormatFileSize(size_t vByteSize)
	{
		if (vByteSize != 0)
		{
			static double lo = 1024.0;
			static double ko = 1024.0 * 1024.0;
			static double mo = 1024.0 * 1024.0 * 1024.0;

			auto v = (double)vByteSize;

			if (v < lo)
				return prRoundNumber(v, 0) + " o"; // octet
			else if (v < ko)
				return prRoundNumber(v / lo, 2) + " Ko"; // ko
			else  if (v < mo)
				return prRoundNumber(v / ko, 2) + " Mo"; // Mo 
			else
				return prRoundNumber(v / mo, 2) + " Go"; // Go 
		}

		return "";
	}

	void IGFD::FileManager::prCompleteFileInfos(const std::shared_ptr<FileInfos>& vInfos)
	{
		if (!vInfos.use_count())
			return;

		if (vInfos->fileName != "." &&
			vInfos->fileName != "..")
		{
			// _stat struct :
			//dev_t     st_dev;     /* ID of device containing file */
			//ino_t     st_ino;     /* inode number */
			//mode_t    st_mode;    /* protection */
			//nlink_t   st_nlink;   /* number of hard links */
			//uid_t     st_uid;     /* user ID of owner */
			//gid_t     st_gid;     /* group ID of owner */
			//dev_t     st_rdev;    /* device ID (if special file) */
			//off_t     st_size;    /* total size, in bytes */
			//blksize_t st_blksize; /* blocksize for file system I/O */
			//blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
			//time_t    st_atime;   /* time of last access - not sure out of ntfs */
			//time_t    st_mtime;   /* time of last modification - not sure out of ntfs */
			//time_t    st_ctime;   /* time of last status change - not sure out of ntfs */

			std::string fpn;

			if (vInfos->fileType == 'f' || vInfos->fileType == 'l' || vInfos->fileType == 'd') // file
				fpn = vInfos->filePath + std::string(1u, PATH_SEP) + vInfos->fileName;

			struct stat statInfos = {};
			char timebuf[100];
			int result = stat(fpn.c_str(), &statInfos);
			if (!result)
			{
				if (vInfos->fileType != 'd')
				{
					vInfos->fileSize = (size_t)statInfos.st_size;
					vInfos->formatedFileSize = prFormatFileSize(vInfos->fileSize);
				}

				size_t len = 0;
#ifdef MSVC
				struct tm _tm;
				errno_t err = localtime_s(&_tm, &statInfos.st_mtime);
				if (!err) len = strftime(timebuf, 99, DateTimeFormat, &_tm);
#else // MSVC
				struct tm* _tm = localtime(&statInfos.st_mtime);
				if (_tm) len = strftime(timebuf, 99, DateTimeFormat, _tm);
#endif // MSVC
				if (len)
				{
					vInfos->fileModifDate = std::string(timebuf, len);
				}
			}
		}
	}

	void IGFD::FileManager::prRemoveFileNameInSelection(const std::string& vFileName)
	{
		prSelectedFileNames.erase(vFileName);

		if (prSelectedFileNames.size() == 1)
		{
			snprintf(puFileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%s", vFileName.c_str());
		}
		else
		{
			snprintf(puFileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%zu files Selected", prSelectedFileNames.size());
		}
	}
	
	void IGFD::FileManager::prAddFileNameInSelection(const std::string& vFileName, bool vSetLastSelectionFileName)
	{
		prSelectedFileNames.emplace(vFileName);

		if (prSelectedFileNames.size() == 1)
		{
			snprintf(puFileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%s", vFileName.c_str());
		}
		else
		{
			snprintf(puFileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, "%zu files Selected", prSelectedFileNames.size());
		}

		if (vSetLastSelectionFileName)
			prLastSelectedFileName = vFileName;
	}

	void IGFD::FileManager::SetCurrentDir(const std::string& vPath)
	{
		std::string path = vPath;
#ifdef WIN32
		if (puFsRoot == path)
			path += std::string(1u, PATH_SEP);
#endif // WIN32
		
#ifdef USE_STD_FILESYSTEM
		namespace fs = std::filesystem;
		bool dir_opened = fs::is_directory(vPath);
		if (!dir_opened)
		{
			path = ".";
			dir_opened = fs::is_directory(vPath);
		}
		if (dir_opened)
#else
		DIR* dir = opendir(path.c_str());
		if (dir == nullptr)
		{
			path = ".";
			dir = opendir(path.c_str());
		}

		if (dir != nullptr)
#endif // USE_STD_FILESYSTEM
		{
#ifdef WIN32
			DWORD numchar = 0;
			//			numchar = GetFullPathNameA(path.c_str(), PATH_MAX, real_path, nullptr);
			std::wstring wpath = IGFD::Utils::string_to_wstring(path);
			numchar = GetFullPathNameW(wpath.c_str(), 0, nullptr, nullptr);
			std::wstring fpath(numchar, 0);
			GetFullPathNameW(wpath.c_str(), numchar, (wchar_t*)fpath.data(), nullptr);
			std::string real_path = IGFD::Utils::wstring_to_string(fpath);
			if (real_path.back() == '\0') // for fix issue we can have with std::string concatenation.. if there is a \0 at end
				real_path = real_path.substr(0, real_path.size() - 1U);
			if (!real_path.empty())
#elif defined(UNIX) // UNIX is LINUX or APPLE
			char real_path[PATH_MAX]; 
			char* numchar = realpath(path.c_str(), real_path);
			if (numchar != nullptr)
#endif // WIN32
			{
				prCurrentPath = real_path;
				if (prCurrentPath[prCurrentPath.size() - 1] == PATH_SEP)
				{
					prCurrentPath = prCurrentPath.substr(0, prCurrentPath.size() - 1);
				}
				IGFD::Utils::SetBuffer(puInputPathBuffer, MAX_PATH_BUFFER_SIZE, prCurrentPath);
				prCurrentPathDecomposition = IGFD::Utils::SplitStringToVector(prCurrentPath, PATH_SEP, false);
#ifdef UNIX // UNIX is LINUX or APPLE
				prCurrentPathDecomposition.insert(prCurrentPathDecomposition.begin(), std::string(1u, PATH_SEP));
#endif // UNIX
				if (!prCurrentPathDecomposition.empty())
				{
#ifdef WIN32
					puFsRoot = prCurrentPathDecomposition[0];
#endif // WIN32
				}
			}
#ifndef USE_STD_FILESYSTEM
			closedir(dir);
#endif
		}
	}

	bool IGFD::FileManager::CreateDir(const std::string& vPath)
	{
		bool res = false;

		if (!vPath.empty())
		{
			std::string path = prCurrentPath + std::string(1u, PATH_SEP) + vPath;

			res = IGFD::Utils::CreateDirectoryIfNotExist(path);
		}

		return res;
	}

	void IGFD::FileManager::ComposeNewPath(std::vector<std::string>::iterator vIter)
	{
		std::string res;

		while (true)
		{
			if (!res.empty())
			{
#ifdef WIN32
				res = *vIter + std::string(1u, PATH_SEP) + res;
#elif defined(UNIX) // UNIX is LINUX or APPLE
				if (*vIter == puFsRoot)
					res = *vIter + res;
				else
					res = *vIter + PATH_SEP + res;
#endif // WIN32
			}
			else
				res = *vIter;

			if (vIter == prCurrentPathDecomposition.begin())
			{
#if defined(UNIX) // UNIX is LINUX or APPLE
				if (res[0] != PATH_SEP)
					res = PATH_SEP + res;
#endif // defined(UNIX)
				break;
			}

			--vIter;
		}

		prCurrentPath = res;
	}

	bool IGFD::FileManager::SetPathOnParentDirectoryIfAny()
	{
		if (prCurrentPathDecomposition.size() > 1)
		{
			ComposeNewPath(prCurrentPathDecomposition.end() - 2);
			return true;
		}
		return false;
	}

	std::string IGFD::FileManager::GetCurrentPath()
	{
		if (prCurrentPath.empty())
			prCurrentPath = ".";
		return prCurrentPath;
	}

	void IGFD::FileManager::SetCurrentPath(const std::string& vCurrentPath)
	{
		if (vCurrentPath.empty())
			prCurrentPath = ".";
		else
			prCurrentPath = vCurrentPath;
	}

	bool IGFD::FileManager::IsFileExist(const std::string& vFile)
	{
		std::ifstream docFile(vFile, std::ios::in);
		if (docFile.is_open())
		{
			docFile.close();
			return true;
		}
		return false;
	}

	void IGFD::FileManager::SetDefaultFileName(const std::string& vFileName)
	{
		puDLGDefaultFileName = vFileName;
		IGFD::Utils::SetBuffer(puFileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, vFileName);
	}

	bool IGFD::FileManager::SelectDirectory(const std::shared_ptr<FileInfos>& vInfos)
	{
		if (!vInfos.use_count())
			return false;

		bool pathClick = false;

		if (vInfos->fileName == "..")
		{
			pathClick = SetPathOnParentDirectoryIfAny();
		}
		else
		{
			std::string newPath;

			if (puShowDrives)
			{
				newPath = vInfos->fileName + std::string(1u, PATH_SEP);
			}
			else
			{
#ifdef __linux__
				if (puFsRoot == prCurrentPath)
					newPath = prCurrentPath + vInfos->fileName;
				else
#endif // __linux__
					newPath = prCurrentPath + std::string(1u, PATH_SEP) + vInfos->fileName;
			}

			if (IGFD::Utils::IsDirectoryExist(newPath))
			{
				if (puShowDrives)
				{
					prCurrentPath = vInfos->fileName;
					puFsRoot = prCurrentPath;
				}
				else
				{
					prCurrentPath = newPath; //-V820
				}
				pathClick = true;
			}
		}

		return pathClick;
	}

	void IGFD::FileManager::SelectFileName(const FileDialogInternal& vFileDialogInternal, const std::shared_ptr<FileInfos>& vInfos)
	{
		if (!vInfos.use_count())
			return;

		if (ImGui::GetIO().KeyCtrl)
		{
			if (puDLGcountSelectionMax == 0) // infinite selection
			{
				if (prSelectedFileNames.find(vInfos->fileName) == prSelectedFileNames.end()) // not found +> add
				{
					prAddFileNameInSelection(vInfos->fileName, true);
				}
				else // found +> remove
				{
					prRemoveFileNameInSelection(vInfos->fileName);
				}
			}
			else // selection limited by size
			{
				if (prSelectedFileNames.size() < puDLGcountSelectionMax)
				{
					if (prSelectedFileNames.find(vInfos->fileName) == prSelectedFileNames.end()) // not found +> add
					{
						prAddFileNameInSelection(vInfos->fileName, true);
					}
					else // found +> remove
					{
						prRemoveFileNameInSelection(vInfos->fileName);
					}
				}
			}
		}
		else if (ImGui::GetIO().KeyShift)
		{
			if (puDLGcountSelectionMax != 1)
			{
				prSelectedFileNames.clear();
				// we will iterate filelist and get the last selection after the start selection
				bool startMultiSelection = false;
				std::string fileNameToSelect = vInfos->fileName;
				std::string savedLastSelectedFileName; // for invert selection mode
				for (const auto& file : prFileList)
				{
					if (!file.use_count())
						continue;

					bool canTake = true;
					if (!file->IsTagFound(vFileDialogInternal.puSearchManager.puSearchTag)) canTake = false;
					if (canTake) // if not filtered, we will take files who are filtered by the dialog
					{
						if (file->fileName == prLastSelectedFileName)
						{
							startMultiSelection = true;
							prAddFileNameInSelection(prLastSelectedFileName, false);
						}
						else if (startMultiSelection)
						{
							if (puDLGcountSelectionMax == 0) // infinite selection
							{
								prAddFileNameInSelection(file->fileName, false);
							}
							else // selection limited by size
							{
								if (prSelectedFileNames.size() < puDLGcountSelectionMax)
								{
									prAddFileNameInSelection(file->fileName, false);
								}
								else
								{
									startMultiSelection = false;
									if (!savedLastSelectedFileName.empty())
										prLastSelectedFileName = savedLastSelectedFileName;
									break;
								}
							}
						}

						if (file->fileName == fileNameToSelect)
						{
							if (!startMultiSelection) // we are before the last Selected FileName, so we must inverse
							{
								savedLastSelectedFileName = prLastSelectedFileName;
								prLastSelectedFileName = fileNameToSelect;
								fileNameToSelect = savedLastSelectedFileName;
								startMultiSelection = true;
								prAddFileNameInSelection(prLastSelectedFileName, false);
							}
							else
							{
								startMultiSelection = false;
								if (!savedLastSelectedFileName.empty())
									prLastSelectedFileName = savedLastSelectedFileName;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			prSelectedFileNames.clear();
			IGFD::Utils::ResetBuffer(puFileNameBuffer);
			prAddFileNameInSelection(vInfos->fileName, true);
		}
	}

	void IGFD::FileManager::DrawDirectoryCreation(const FileDialogInternal& vFileDialogInternal)
	{
		if (vFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_DisableCreateDirectoryButton)
			return;

		if (IMGUI_BUTTON(createDirButtonString))
		{
			if (!prCreateDirectoryMode)
			{
				prCreateDirectoryMode = true;
				IGFD::Utils::ResetBuffer(puDirectoryNameBuffer);
			}
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(buttonCreateDirString);

		if (prCreateDirectoryMode)
		{
			ImGui::SameLine();

			ImGui::PushItemWidth(100.0f);
			ImGui::InputText("##DirectoryFileName", puDirectoryNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
			ImGui::PopItemWidth();

			ImGui::SameLine();

			if (IMGUI_BUTTON(okButtonString))
			{
				std::string newDir = std::string(puDirectoryNameBuffer);
				if (CreateDir(newDir))
				{
					SetCurrentPath(prCurrentPath + std::string(1u, PATH_SEP) + newDir);
					OpenCurrentPath(vFileDialogInternal);
				}

				prCreateDirectoryMode = false;
			}

			ImGui::SameLine();

			if (IMGUI_BUTTON(cancelButtonString))
			{
				prCreateDirectoryMode = false;
			}
		}
	}

	void IGFD::FileManager::DrawPathComposer(const FileDialogInternal& vFileDialogInternal)
	{
		if (IMGUI_BUTTON(resetButtonString))
		{
			SetCurrentPath(".");
			OpenCurrentPath(vFileDialogInternal);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(buttonResetPathString);

#ifdef WIN32
		ImGui::SameLine();

		if (IMGUI_BUTTON(drivesButtonString))
		{
			puDrivesClicked = true;
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(buttonDriveString);
#endif // WIN32

		ImGui::SameLine();
		
		if (IMGUI_BUTTON(editPathButtonString))
		{
			puInputPathActivated = true;
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(buttonEditPathString);

		ImGui::SameLine();

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

		// show current path
		if (!prCurrentPathDecomposition.empty())
		{
			ImGui::SameLine();

			if (puInputPathActivated)
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::InputText("##pathedition", puInputPathBuffer, MAX_PATH_BUFFER_SIZE);
				ImGui::PopItemWidth();
			}
			else
			{
				int _id = 0;
				for (auto itPathDecomp = prCurrentPathDecomposition.begin();
					itPathDecomp != prCurrentPathDecomposition.end(); ++itPathDecomp)
				{
					if (itPathDecomp != prCurrentPathDecomposition.begin())
						ImGui::SameLine();
					ImGui::PushID(_id++);
					bool click = IMGUI_PATH_BUTTON((*itPathDecomp).c_str());
					ImGui::PopID();
					if (click)
					{
						ComposeNewPath(itPathDecomp);
						puPathClicked = true;
						break;
					}
					// activate input for path
					if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
					{
						ComposeNewPath(itPathDecomp);
						IGFD::Utils::SetBuffer(puInputPathBuffer, MAX_PATH_BUFFER_SIZE, prCurrentPath);
						puInputPathActivated = true;
						break;
					}
				}
			}
		}
	}

	std::string IGFD::FileManager::GetResultingPath()
	{
		std::string path = prCurrentPath;

		if (puDLGDirectoryMode) // if directory mode
		{
			std::string selectedDirectory = puFileNameBuffer;
			if (!selectedDirectory.empty() &&
				selectedDirectory != ".")
				path += std::string(1u, PATH_SEP) + selectedDirectory;
		}

		return path;
	}

	std::string IGFD::FileManager::GetResultingFileName(FileDialogInternal& vFileDialogInternal)
	{
		if (!puDLGDirectoryMode) // if not directory mode
		{
			return vFileDialogInternal.puFilterManager.ReplaceExtentionWithCurrentFilter(std::string(puFileNameBuffer));
		}

		return ""; // directory mode
	}

	std::string IGFD::FileManager::GetResultingFilePathName(FileDialogInternal& vFileDialogInternal)
	{
		std::string result = GetResultingPath();

		std::string filename = GetResultingFileName(vFileDialogInternal);
		if (!filename.empty())
		{
#ifdef UNIX
			if (puFsRoot != result)
#endif // UNIX
				result += std::string(1u, PATH_SEP);

			result += filename;
		}

		return result;
	}

	std::map<std::string, std::string> IGFD::FileManager::GetResultingSelection()
	{
		std::map<std::string, std::string> res;

		for (auto& selectedFileName : prSelectedFileNames)
		{
			std::string result = GetResultingPath();

#ifdef UNIX
			if (puFsRoot != result)
#endif // UNIX
				result += std::string(1u, PATH_SEP);

			result += selectedFileName;

			res[selectedFileName] = result;
		}

		return res;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILE DIALOG INTERNAL ///////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	void IGFD::FileDialogInternal::NewFrame()
	{
		puCanWeContinue = true;	// reset flag for possibily validate the dialog
		puIsOk = false;				// reset dialog result
		puFileManager.puDrivesClicked = false;
		puFileManager.puPathClicked = false;
		
		puNeedToExitDialog = false;

#ifdef USE_DIALOG_EXIT_WITH_KEY
		if (ImGui::IsKeyPressed(IGFD_EXIT_KEY))
		{
			// we do that here with the data's defined at the last frame
			// because escape key can quit input activation and at the end of the frame all flag will be false
			// so we will detect nothing
			if (!(puFileManager.puInputPathActivated ||
				puSearchManager.puSearchInputIsActive ||
				puFileInputIsActive ||
				puFileListViewIsActive))
			{
				puNeedToExitDialog = true; // need to quit dialog
			}
		}
		else
#endif
		{
			puSearchManager.puSearchInputIsActive = false;
			puFileInputIsActive = false;
			puFileListViewIsActive = false;
		}
	}

	void IGFD::FileDialogInternal::EndFrame()
	{
		// directory change
		if (puFileManager.puPathClicked)
		{
			puFileManager.OpenCurrentPath(*this);
		}

		if (puFileManager.puDrivesClicked)
		{
			if (puFileManager.GetDrives())
			{
				puFileManager.ApplyFilteringOnFileList(*this);
			}
		}

		if (puFileManager.puInputPathActivated)
		{
			auto gio = ImGui::GetIO();
			if (ImGui::IsKeyReleased(gio.KeyMap[ImGuiKey_Enter]))
			{
				puFileManager.SetCurrentPath(std::string(puFileManager.puInputPathBuffer));
				puFileManager.OpenCurrentPath(*this);
				puFileManager.puInputPathActivated = false;
			}
			if (ImGui::IsKeyReleased(gio.KeyMap[ImGuiKey_Escape]))
			{
				puFileManager.puInputPathActivated = false;
			}
		}
	}

	void IGFD::FileDialogInternal::ResetForNewDialog()
	{
	
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//// THUMBNAIL FEATURE //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	IGFD::ThumbnailFeature::ThumbnailFeature()
	{
#ifdef USE_THUMBNAILS
		prDisplayMode = DisplayModeEnum::FILE_LIST;
#endif
	}

	IGFD::ThumbnailFeature::~ThumbnailFeature()	= default;

	void IGFD::ThumbnailFeature::NewThumbnailFrame(FileDialogInternal& vFileDialogInternal)
	{
		(void)vFileDialogInternal;

#ifdef USE_THUMBNAILS
		prStartThumbnailFileDatasExtraction();
#endif
	}

	void IGFD::ThumbnailFeature::EndThumbnailFrame(FileDialogInternal& vFileDialogInternal)
	{
#ifdef USE_THUMBNAILS
		prClearThumbnails(vFileDialogInternal);
#endif
	}

	void IGFD::ThumbnailFeature::QuitThumbnailFrame(FileDialogInternal& vFileDialogInternal)
	{
#ifdef USE_THUMBNAILS
		prStopThumbnailFileDatasExtraction();
		prClearThumbnails(vFileDialogInternal);
#endif
	}

#ifdef USE_THUMBNAILS
	void IGFD::ThumbnailFeature::prStartThumbnailFileDatasExtraction()
	{
		const bool res = prThumbnailGenerationThread.use_count() && prThumbnailGenerationThread->joinable();
		if (!res)
		{
			prIsWorking = true;
			prCountFiles = 0U;
			prThumbnailGenerationThread = std::shared_ptr<std::thread>(
				new std::thread(&IGFD::ThumbnailFeature::prThreadThumbnailFileDatasExtractionFunc, this),
				[this](std::thread* obj)
				{
					prIsWorking = false;
					if (obj)
						obj->join();
				});
		}
	}

	bool IGFD::ThumbnailFeature::prStopThumbnailFileDatasExtraction()
	{
		const bool res = prThumbnailGenerationThread.use_count() && prThumbnailGenerationThread->joinable();
		if (res)
		{
			prThumbnailGenerationThread.reset();
		}

		return res;
	}
	
	void IGFD::ThumbnailFeature::prThreadThumbnailFileDatasExtractionFunc()
	{
		prCountFiles = 0U;
		prIsWorking = true;

		// infinite loop while is thread working
		while(prIsWorking)
		{
			if (!prThumbnailFileDatasToGet.empty())
			{
				std::shared_ptr<FileInfos> file = nullptr;
				prThumbnailFileDatasToGetMutex.lock();
				//get the first file in the list
				if (!prThumbnailFileDatasToGet.empty())
					file = (*prThumbnailFileDatasToGet.begin());
				prThumbnailFileDatasToGetMutex.unlock();

				// retrieve datas of the texture file if its an image file
				if (file.use_count())
				{
					if (file->fileType == 'f')
					{
						if (file->fileExt == ".png"
							|| file->fileExt == ".bmp"
							|| file->fileExt == ".tga"
							|| file->fileExt == ".jpg" || file->fileExt == ".jpeg"
							|| file->fileExt == ".gif"
							|| file->fileExt == ".psd"
							|| file->fileExt == ".pic"
							|| file->fileExt == ".ppm" || file->fileExt == ".pgm"
							//|| file->fileExt == ".hdr" => format float so in few times
							)
						{
							auto fpn = file->filePath + std::string(1u, PATH_SEP) + file->fileName;

							int w = 0;
							int h = 0;
							int chans = 0;
							uint8_t *datas = stbi_load(fpn.c_str(), &w, &h, &chans, STBI_rgb_alpha);
							if (datas)
							{
								if (w && h)
								{
									// resize with respect to glyph ratio
									float ratioX = (float)w / (float)h;
									float newX = DisplayMode_ThumbailsList_ImageHeight * ratioX;
									float newY = w / ratioX;
									if (newX < w) 
										newY = DisplayMode_ThumbailsList_ImageHeight;

									const auto newWidth = (int)newX;
									const auto newHeight = (int)newY;
									const auto newBufSize = (size_t)(newWidth * newHeight * 4);
									auto resizedData = new uint8_t[newBufSize];
									
									const int resizeSucceeded = stbir_resize_uint8(
										datas, w, h, 0,
										resizedData, newWidth, newHeight, 0,
										4);

									if (resizeSucceeded)
									{
										file->thumbnailInfo.textureFileDatas = resizedData;
										file->thumbnailInfo.textureWidth = newWidth;
										file->thumbnailInfo.textureHeight = newHeight;
										file->thumbnailInfo.textureChannels = 4;

										// we set that at least, because will launch the gpu creation of the texture in the main thread
										file->thumbnailInfo.isReadyToUpload = true;

										// need gpu loading
										prAddThumbnailToCreate(file);
									}
								}
								else
								{
									printf("image loading fail : w:%i h:%i c:%i\n", w, h, 4);
								}

								stbi_image_free(datas);
							}
						}
					}

					// peu importe le resultat on vire le fichicer
					// remove form this list
					// write => thread concurency issues
					prThumbnailFileDatasToGetMutex.lock();
					prThumbnailFileDatasToGet.pop_front();
					prThumbnailFileDatasToGetMutex.unlock();
				}
			}
		}

		prIsWorking = false;
	}

	inline void inVariadicProgressBar(float fraction, const ImVec2& size_arg, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char TempBuffer[512];
		const int w = vsnprintf(TempBuffer, 511, fmt, args);
		va_end(args);
		if (w)
		{
			ImGui::ProgressBar(fraction, size_arg, TempBuffer);
		}
	}

	void IGFD::ThumbnailFeature::prDrawThumbnailGenerationProgress()
	{
		if (prThumbnailGenerationThread.use_count() && prThumbnailGenerationThread->joinable())
		{
			if (!prThumbnailFileDatasToGet.empty())
			{
				const auto p = (float)((double)prCountFiles / (double)prThumbnailFileDatasToGet.size()); // read => no thread concurency issues
				inVariadicProgressBar(p, ImVec2(50, 0), "%u/%u", prCountFiles, (uint32_t)prThumbnailFileDatasToGet.size()); // read => no thread concurency issues
				ImGui::SameLine();
			}
		}
	}

	void IGFD::ThumbnailFeature::prAddThumbnailToLoad(const std::shared_ptr<FileInfos>& vFileInfos)
	{
		if (vFileInfos.use_count())
		{
			if (vFileInfos->fileType == 'f')
			{
				if (vFileInfos->fileExt == ".png"
					|| vFileInfos->fileExt == ".bmp"
					|| vFileInfos->fileExt == ".tga"
					|| vFileInfos->fileExt == ".jpg" || vFileInfos->fileExt == ".jpeg"
					|| vFileInfos->fileExt == ".gif"
					|| vFileInfos->fileExt == ".psd"
					|| vFileInfos->fileExt == ".pic"
					|| vFileInfos->fileExt == ".ppm" || vFileInfos->fileExt == ".pgm"
					//|| file->fileExt == ".hdr" => format float so in few times
					)
				{
					// write => thread concurency issues
					prThumbnailFileDatasToGetMutex.lock();
					prThumbnailFileDatasToGet.push_back(vFileInfos);
					vFileInfos->thumbnailInfo.isLoadingOrLoaded = true;
					prThumbnailFileDatasToGetMutex.unlock();
				}
			}
		}
	}
	
	void IGFD::ThumbnailFeature::prAddThumbnailToCreate(const std::shared_ptr<FileInfos>& vFileInfos)
	{
		if (vFileInfos.use_count())
		{
			// write => thread concurency issues
			prThumbnailToCreateMutex.lock();
			prThumbnailToCreate.push_back(vFileInfos);
			prThumbnailToCreateMutex.unlock();
		}
	}

	void IGFD::ThumbnailFeature::prAddThumbnailToDestroy(IGFD_Thumbnail_Info vIGFD_Thumbnail_Info)
	{
		// write => thread concurency issues
		prThumbnailToDestroyMutex.lock();
		prThumbnailToDestroy.push_back(vIGFD_Thumbnail_Info);
		prThumbnailToDestroyMutex.unlock();
	}

	void IGFD::ThumbnailFeature::prDrawDisplayModeToolBar()
	{
		if (IMGUI_RADIO_BUTTON(DisplayMode_FilesList_ButtonString,
			prDisplayMode == DisplayModeEnum::FILE_LIST))
			prDisplayMode = DisplayModeEnum::FILE_LIST;
		if (ImGui::IsItemHovered())	ImGui::SetTooltip(DisplayMode_FilesList_ButtonHelp);
		ImGui::SameLine();
		if (IMGUI_RADIO_BUTTON(DisplayMode_ThumbailsList_ButtonString,
			prDisplayMode == DisplayModeEnum::THUMBNAILS_LIST))
			prDisplayMode = DisplayModeEnum::THUMBNAILS_LIST;
		if (ImGui::IsItemHovered())	ImGui::SetTooltip(DisplayMode_ThumbailsList_ButtonHelp);
		ImGui::SameLine();
		/* todo
		if (IMGUI_RADIO_BUTTON(DisplayMode_ThumbailsGrid_ButtonString,
			prDisplayMode == DisplayModeEnum::THUMBNAILS_GRID))
			prDisplayMode = DisplayModeEnum::THUMBNAILS_GRID;
		if (ImGui::IsItemHovered())	ImGui::SetTooltip(DisplayMode_ThumbailsGrid_ButtonHelp);
		ImGui::SameLine();
		*/
		prDrawThumbnailGenerationProgress();
	}

	void IGFD::ThumbnailFeature::prClearThumbnails(FileDialogInternal& vFileDialogInternal)
	{
		// directory wil be changed so the file list will be erased
		if (vFileDialogInternal.puFileManager.puPathClicked)
		{
			size_t count = vFileDialogInternal.puFileManager.GetFullFileListSize();
			for (size_t idx = 0U; idx < count; idx++)
			{
				auto file = vFileDialogInternal.puFileManager.GetFullFileAt(idx);
				if (file.use_count())
				{
					if (file->thumbnailInfo.isReadyToDisplay)
					{
						prAddThumbnailToDestroy(file->thumbnailInfo);
					}
				}
			}
		}
	}

	void IGFD::ThumbnailFeature::SetCreateThumbnailCallback(const CreateThumbnailFun& vCreateThumbnailFun)
	{
		prCreateThumbnailFun = vCreateThumbnailFun;
	}

	void IGFD::ThumbnailFeature::SetDestroyThumbnailCallback(const DestroyThumbnailFun& vCreateThumbnailFun)
	{
		prDestroyThumbnailFun = vCreateThumbnailFun;
	}

	void IGFD::ThumbnailFeature::ManageGPUThumbnails()
	{
		if (prCreateThumbnailFun)
		{
			if (!prThumbnailToCreate.empty())
			{
				for (const auto& file : prThumbnailToCreate)
				{
					if (file.use_count())
					{
						prCreateThumbnailFun(&file->thumbnailInfo);
					}
				}
				prThumbnailToCreateMutex.lock();
				prThumbnailToCreate.clear();
				prThumbnailToCreateMutex.unlock();
			}
		}
		else
		{
			printf("No Callback found for create texture\nYou need to define the callback with a call to SetCreateThumbnailCallback\n");
		}

		if (prDestroyThumbnailFun)
		{
			if (!prThumbnailToDestroy.empty())
			{
				for (auto thumbnail : prThumbnailToDestroy)
				{
					prDestroyThumbnailFun(&thumbnail);
				}
				prThumbnailToDestroyMutex.lock();
				prThumbnailToDestroy.clear();
				prThumbnailToDestroyMutex.unlock();
			}
		}
		else
		{
		printf("No Callback found for destroy texture\nYou need to define the callback with a call to SetCreateThumbnailCallback\n");
		}
	}

#endif // USE_THUMBNAILS

	/////////////////////////////////////////////////////////////////////////////////////
	//// BOOKMARK FEATURE ///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	IGFD::BookMarkFeature::BookMarkFeature()
	{
#ifdef USE_BOOKMARK
		prBookmarkWidth = defaultBookmarkPaneWith;
#endif // USE_BOOKMARK
	}

#ifdef USE_BOOKMARK
	void IGFD::BookMarkFeature::prDrawBookmarkButton()
	{
		IMGUI_TOGGLE_BUTTON(bookmarksButtonString, &prBookmarkPaneShown);

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(bookmarksButtonHelpString);
	}
	bool IGFD::BookMarkFeature::prDrawBookmarkPane(FileDialogInternal& vFileDialogInternal, const ImVec2& vSize)
	{
		bool res = false;

		ImGui::BeginChild("##bookmarkpane", vSize);

		static int selectedBookmarkForEdition = -1;

		if (IMGUI_BUTTON(addBookmarkButtonString "##ImGuiFileDialogAddBookmark"))
		{
			if (!vFileDialogInternal.puFileManager.IsComposerEmpty())
			{
				BookmarkStruct bookmark;
				bookmark.name = vFileDialogInternal.puFileManager.GetBack();
				bookmark.path = vFileDialogInternal.puFileManager.GetCurrentPath();
				prBookmarks.push_back(bookmark);
			}
		}
		if (selectedBookmarkForEdition >= 0 &&
			selectedBookmarkForEdition < (int)prBookmarks.size())
		{
			ImGui::SameLine();
			if (IMGUI_BUTTON(removeBookmarkButtonString "##ImGuiFileDialogAddBookmark"))
			{
				prBookmarks.erase(prBookmarks.begin() + selectedBookmarkForEdition);
				if (selectedBookmarkForEdition == (int)prBookmarks.size())
					selectedBookmarkForEdition--;
			}

			if (selectedBookmarkForEdition >= 0 &&
				selectedBookmarkForEdition < (int)prBookmarks.size())
			{
				ImGui::SameLine();

				ImGui::PushItemWidth(vSize.x - ImGui::GetCursorPosX());
				if (ImGui::InputText("##ImGuiFileDialogBookmarkEdit", prBookmarkEditBuffer, MAX_FILE_DIALOG_NAME_BUFFER))
				{
					prBookmarks[selectedBookmarkForEdition].name = std::string(prBookmarkEditBuffer);
				}
				ImGui::PopItemWidth();
			}
		}

		ImGui::Separator();

		if (!prBookmarks.empty())
		{
			prBookmarkClipper.Begin((int)prBookmarks.size(), ImGui::GetTextLineHeightWithSpacing());
			while (prBookmarkClipper.Step())
			{
				for (int i = prBookmarkClipper.DisplayStart; i < prBookmarkClipper.DisplayEnd; i++)
				{
					if (i < 0) continue;
					const BookmarkStruct& bookmark = prBookmarks[i];
					ImGui::PushID(i);
					if (ImGui::Selectable(bookmark.name.c_str(), selectedBookmarkForEdition == i,
						ImGuiSelectableFlags_AllowDoubleClick) |
						(selectedBookmarkForEdition == -1 &&
							bookmark.path == vFileDialogInternal.puFileManager.GetCurrentPath())) // select if path is current
					{
						selectedBookmarkForEdition = i;
						IGFD::Utils::ResetBuffer(prBookmarkEditBuffer);
						IGFD::Utils::AppendToBuffer(prBookmarkEditBuffer, MAX_FILE_DIALOG_NAME_BUFFER, bookmark.name);

						if (ImGui::IsMouseDoubleClicked(0)) // apply path
						{
							vFileDialogInternal.puFileManager.SetCurrentPath(bookmark.path);
							vFileDialogInternal.puFileManager.OpenCurrentPath(vFileDialogInternal);
							res = true;
						}
					}
					ImGui::PopID();
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", bookmark.path.c_str());
				}
			}
			prBookmarkClipper.End();
		}

		ImGui::EndChild();

		return res;
	}

	std::string IGFD::BookMarkFeature::SerializeBookmarks()
	{
		std::string res;

		size_t idx = 0;
		for (auto& it : prBookmarks)
		{
			if (idx++ != 0)
				res += "##"; // ## because reserved by imgui, so an input text cant have ##
			res += it.name + "##" + it.path;
		}

		return res;
	}

	void IGFD::BookMarkFeature::DeserializeBookmarks(const std::string& vBookmarks)
	{
		if (!vBookmarks.empty())
		{
			prBookmarks.clear();
			auto arr = IGFD::Utils::SplitStringToVector(vBookmarks, '#', false);
			for (size_t i = 0; i < arr.size(); i += 2)
			{
				BookmarkStruct bookmark;
				bookmark.name = arr[i];
				if (i + 1 < arr.size()) // for avoid crash if arr size is impair due to user mistake after edition
				{
					// if bad format we jump this bookmark
					bookmark.path = arr[i + 1];
					prBookmarks.push_back(bookmark);
				}
			}
		}
	}
#endif // USE_BOOKMARK

	/////////////////////////////////////////////////////////////////////////////////////
	//// KEY EXPLORER FEATURE ///////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	KeyExplorerFeature::KeyExplorerFeature() = default;

#ifdef USE_EXPLORATION_BY_KEYS
	bool IGFD::KeyExplorerFeature::prLocateItem_Loop(FileDialogInternal& vFileDialogInternal, ImWchar vC)
	{
		bool found = false;

		auto& fdi = vFileDialogInternal.puFileManager;
		if (!fdi.IsFilteredListEmpty())
		{
			auto countFiles = fdi.GetFilteredListSize();
			for (size_t i = prLocateFileByInputChar_lastFileIdx; i < countFiles; i++)
			{
				auto nfo = fdi.GetFilteredFileAt(i);
				if (nfo.use_count())
				{
					if (nfo->fileName_optimized[0] == vC || // lower case search
						nfo->fileName[0] == vC) // maybe upper case search
					{
						//float p = ((float)i) * ImGui::GetTextLineHeightWithSpacing();
						float p = (float)((double)i / (double)countFiles) * ImGui::GetScrollMaxY();
						ImGui::SetScrollY(p);
						prLocateFileByInputChar_lastFound = true;
						prLocateFileByInputChar_lastFileIdx = i;
						prStartFlashItem(prLocateFileByInputChar_lastFileIdx);

						auto infos = fdi.GetFilteredFileAt(prLocateFileByInputChar_lastFileIdx);
						if (infos.use_count())
						{
							if (infos->fileType == 'd')
							{
								if (fdi.puDLGDirectoryMode) // directory chooser
								{
									fdi.SelectFileName(vFileDialogInternal, infos);
								}
							}
							else
							{
								fdi.SelectFileName(vFileDialogInternal, infos);
							}

							found = true;
							break;
						}
					}
				}
			}
		}

		return found;
	}

	void IGFD::KeyExplorerFeature::prLocateByInputKey(FileDialogInternal& vFileDialogInternal)
	{
		ImGuiContext& g = *GImGui;
		auto& fdi = vFileDialogInternal.puFileManager;
		if (!g.ActiveId && !fdi.IsFilteredListEmpty())
		{
			auto& queueChar = ImGui::GetIO().InputQueueCharacters;
			auto countFiles = fdi.GetFilteredListSize();

			// point by char
			if (!queueChar.empty())
			{
				ImWchar c = queueChar.back();
				if (prLocateFileByInputChar_InputQueueCharactersSize != queueChar.size())
				{
					if (c == prLocateFileByInputChar_lastChar) // next file starting with same char until
					{
						if (prLocateFileByInputChar_lastFileIdx < countFiles - 1U)
							prLocateFileByInputChar_lastFileIdx++;
						else
							prLocateFileByInputChar_lastFileIdx = 0;
					}

					if (!prLocateItem_Loop(vFileDialogInternal, c))
					{
						// not found, loop again from 0 this time
						prLocateFileByInputChar_lastFileIdx = 0;
						prLocateItem_Loop(vFileDialogInternal, c);
					}

					prLocateFileByInputChar_lastChar = c;
				}
			}

			prLocateFileByInputChar_InputQueueCharactersSize = queueChar.size();
		}
	}

	void IGFD::KeyExplorerFeature::prExploreWithkeys(FileDialogInternal& vFileDialogInternal, ImGuiID vListViewID)
	{
		auto& fdi = vFileDialogInternal.puFileManager;
		if (!fdi.IsFilteredListEmpty())
		{
			bool canWeExplore = false;
			bool hasNav = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard);
			
			ImGuiContext& g = *GImGui;
			if (!hasNav && !g.ActiveId) // no nav and no activated inputs
				canWeExplore = true;

			if (g.NavId && g.NavId == vListViewID)
			{
				if (ImGui::IsKeyPressedMap(IGFD_KEY_ENTER) ||
					ImGui::IsKeyPressedMap(ImGuiKey_KeyPadEnter) ||
					ImGui::IsKeyPressedMap(ImGuiKey_Space))
				{
					ImGui::ActivateItem(vListViewID);
					ImGui::SetActiveID(vListViewID, g.CurrentWindow);
				}
			}
			
			if (vListViewID == g.LastActiveId-1) // if listview id is the last acticated nav id (ImGui::ActivateItem(vListViewID);)
				canWeExplore = true;

			if (canWeExplore)
			{
				if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
				{
					ImGui::ClearActiveID();
					g.LastActiveId = 0;
				}

				auto countFiles = fdi.GetFilteredListSize();

				// explore
				bool exploreByKey = false;
				bool enterInDirectory = false;
				bool exitDirectory = false;

				if ((hasNav && ImGui::IsKeyPressedMap(ImGuiKey_UpArrow)) || (!hasNav && ImGui::IsKeyPressed(IGFD_KEY_UP)))
				{
					exploreByKey = true;
					if (prLocateFileByInputChar_lastFileIdx > 0)
						prLocateFileByInputChar_lastFileIdx--;
					else
						prLocateFileByInputChar_lastFileIdx = countFiles - 1U;
				}
				else if ((hasNav && ImGui::IsKeyPressedMap(ImGuiKey_DownArrow)) || (!hasNav && ImGui::IsKeyPressed(IGFD_KEY_DOWN)))
				{
					exploreByKey = true;
					if (prLocateFileByInputChar_lastFileIdx < countFiles - 1U)
						prLocateFileByInputChar_lastFileIdx++;
					else
						prLocateFileByInputChar_lastFileIdx = 0U;
				}
				else if (ImGui::IsKeyReleased(IGFD_KEY_ENTER))
				{
					exploreByKey = true;
					enterInDirectory = true;
				}
				else if (ImGui::IsKeyReleased(IGFD_KEY_BACKSPACE))
				{
					exploreByKey = true;
					exitDirectory = true;
				}

				if (exploreByKey)
				{
					//float totalHeight = prFilteredFileList.size() * ImGui::GetTextLineHeightWithSpacing();
					float p = (float)((double)prLocateFileByInputChar_lastFileIdx / (double)(countFiles - 1U)) * ImGui::GetScrollMaxY();// seems not udpated in tables version outside tables
					//float p = ((float)locateFileByInputChar_lastFileIdx) * ImGui::GetTextLineHeightWithSpacing();
					ImGui::SetScrollY(p);
					prStartFlashItem(prLocateFileByInputChar_lastFileIdx);

					auto infos = fdi.GetFilteredFileAt(prLocateFileByInputChar_lastFileIdx);
					if (infos.use_count())
					{
						if (infos->fileType == 'd')
						{
							if (!fdi.puDLGDirectoryMode || enterInDirectory)
							{
								if (enterInDirectory)
								{
									if (fdi.SelectDirectory(infos))
									{
										// changement de repertoire
										vFileDialogInternal.puFileManager.OpenCurrentPath(vFileDialogInternal);
										if (prLocateFileByInputChar_lastFileIdx > countFiles - 1U)
										{
											prLocateFileByInputChar_lastFileIdx = 0;
										}
									}
								}
							}
							else // directory chooser
							{
								fdi.SelectFileName(vFileDialogInternal, infos);
							}
						}
						else
						{
							fdi.SelectFileName(vFileDialogInternal, infos);
						}

						if (exitDirectory)
						{
							auto nfo = std::make_shared<FileInfos>();
							nfo->fileName = "..";

							if (fdi.SelectDirectory(nfo))
							{
								// changement de repertoire
								vFileDialogInternal.puFileManager.OpenCurrentPath(vFileDialogInternal);
								if (prLocateFileByInputChar_lastFileIdx > countFiles - 1U)
								{
									prLocateFileByInputChar_lastFileIdx = 0;
								}
							}
#ifdef WIN32
							else
							{
								if (fdi.GetComposerSize() == 1U)
								{
									if (fdi.GetDrives())
									{
										fdi.ApplyFilteringOnFileList(vFileDialogInternal);
									}
								}
							}
#endif // WIN32
						}
					}
				}
			}
		}
	}

	bool IGFD::KeyExplorerFeature::prFlashableSelectable(const char* label, bool selected,
		ImGuiSelectableFlags flags, bool vFlashing, const ImVec2& size_arg)
	{
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
		ImGuiID id = window->GetID(label);
		ImVec2 label_size = CalcTextSize(label, nullptr, true);
		ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		ImVec2 pos = window->DC.CursorPos;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ItemSize(size, 0.0f);

		// Fill horizontal space
		// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
			size.x = ImMax(label_size.x, max_x - min_x);

		// Text stays at the submission position, but bounding box may be extended on both sides
		const ImVec2 text_min = pos;
		const ImVec2 text_max(min_x + size.x, pos.y + size.y);

		// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		{
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
			const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}
		//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

		// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns)
		{
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		bool item_add;
		const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
		if (disabled_item)
		{
			ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
			g.CurrentItemFlags |= ImGuiItemFlags_Disabled;
			item_add = ItemAdd(bb, id);
			g.CurrentItemFlags = backup_item_flags;
		}
		else
		{
			item_add = ItemAdd(bb, id);
		}

		if (span_all_columns)
		{
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add)
			return false;

		const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
		if (disabled_item && !disabled_global) // Only testing this as an optimization
			BeginDisabled(true);

		// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
		// which would be advantageous since most selectable are not selected.
		if (span_all_columns && window->DC.CurrentColumns)
			PushColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			TablePushBackgroundChannel();

		// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

		const bool was_selected = selected;
		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

		// Auto-select when moved into
		// - This will be more fully fleshed in the range-select branch
		// - This is not exposed as it won't nicely work with some user side handling of shift/control
		// - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
		//   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
		//   - (2) usage will fail with clipped items
		//   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
		if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == window->DC.NavFocusScopeIdCurrent)
			if (g.NavJustMovedToId == id)
				selected = pressed = true;

		// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		{
			if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
			{
				SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect(bb.Min - window->Pos, bb.Max - window->Pos));
				g.NavDisableHighlight = true;
			}
		}
		if (pressed)
			MarkItemEdited(id);

		if (flags & ImGuiSelectableFlags_AllowItemOverlap)
			SetItemAllowOverlap();

		// In this branch, Selectable() cannot toggle the selection so this will never trigger.
		if (selected != was_selected) //-V547
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		// Render
		if ((held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld)) || vFlashing)
			hovered = true;
		if (hovered || selected)
		{
			const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
			RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
		}
		RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

		if (span_all_columns && window->DC.CurrentColumns)
			PopColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			TablePopBackgroundChannel();

		RenderTextClipped(text_min, text_max, label, nullptr, &label_size, style.SelectableTextAlign, &bb);

		// Automatically close popups
		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
			CloseCurrentPopup();

		if (disabled_item && !disabled_global)
			EndDisabled();

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
		return pressed; //-V1020
	}

	void IGFD::KeyExplorerFeature::prStartFlashItem(size_t vIdx)
	{
		prFlashAlpha = 1.0f;
		prFlashedItem = vIdx;
	}

	bool IGFD::KeyExplorerFeature::prBeginFlashItem(size_t vIdx)
	{
		bool res = false;

		if (prFlashedItem == vIdx &&
			std::abs(prFlashAlpha - 0.0f) > 0.00001f)
		{
			prFlashAlpha -= prFlashAlphaAttenInSecs * ImGui::GetIO().DeltaTime;
			if (prFlashAlpha < 0.0f) prFlashAlpha = 0.0f;

			ImVec4 hov = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
			hov.w = prFlashAlpha;
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, hov);
			res = true;
		}

		return res;
	}

	void IGFD::KeyExplorerFeature::prEndFlashItem()
	{
		ImGui::PopStyleColor();
	}

	void IGFD::KeyExplorerFeature::SetFlashingAttenuationInSeconds(float vAttenValue)
	{
		prFlashAlphaAttenInSecs = 1.0f / ImMax(vAttenValue, 0.01f);
	}
#endif // USE_EXPLORATION_BY_KEYS

	/////////////////////////////////////////////////////////////////////////////////////
	//// FILE DIALOG CONSTRUCTOR / DESTRUCTOR ///////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	IGFD::FileDialog::FileDialog() : BookMarkFeature(), KeyExplorerFeature(), ThumbnailFeature() {}
	IGFD::FileDialog::~FileDialog() = default;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	///// FILE DIALOG STANDARD DIALOG ////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// path and fileName can be specified
	void IGFD::FileDialog::OpenDialog(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vPath,
		const std::string& vFileName,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		prFileDialogInternal.ResetForNewDialog();

		prFileDialogInternal.puDLGkey = vKey;
		prFileDialogInternal.puDLGtitle = vTitle;
		prFileDialogInternal.puDLGuserDatas = vUserDatas;
		prFileDialogInternal.puDLGflags = vFlags;
		prFileDialogInternal.puDLGoptionsPane = nullptr;
		prFileDialogInternal.puDLGoptionsPaneWidth = 0.0f;
		prFileDialogInternal.puDLGmodal = false;

		prFileDialogInternal.puFilterManager.puDLGdefaultExt.clear();
		prFileDialogInternal.puFilterManager.ParseFilters(vFilters);

		prFileDialogInternal.puFileManager.puDLGDirectoryMode = (vFilters == nullptr);
		if (vPath.empty())
			prFileDialogInternal.puFileManager.puDLGpath = prFileDialogInternal.puFileManager.GetCurrentPath();
		else
			prFileDialogInternal.puFileManager.puDLGpath = vPath;
		prFileDialogInternal.puFileManager.SetCurrentPath(vPath);
		prFileDialogInternal.puFileManager.puDLGcountSelectionMax = vCountSelectionMax;
		prFileDialogInternal.puFileManager.SetDefaultFileName(vFileName);

		prFileDialogInternal.puFileManager.ClearAll();
		
		prFileDialogInternal.puShowDialog = true;					// open dialog
	}

	// path and filename are obtained from filePathName
	void IGFD::FileDialog::OpenDialog(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vFilePathName,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		prFileDialogInternal.ResetForNewDialog();

		prFileDialogInternal.puDLGkey = vKey;
		prFileDialogInternal.puDLGtitle = vTitle;
		prFileDialogInternal.puDLGoptionsPane = nullptr;
		prFileDialogInternal.puDLGoptionsPaneWidth = 0.0f;
		prFileDialogInternal.puDLGuserDatas = vUserDatas;
		prFileDialogInternal.puDLGflags = vFlags;
		prFileDialogInternal.puDLGmodal = false;

		auto ps = IGFD::Utils::ParsePathFileName(vFilePathName);
		if (ps.isOk)
		{
			prFileDialogInternal.puFileManager.puDLGpath = ps.path;
			prFileDialogInternal.puFileManager.SetDefaultFileName(vFilePathName);
			prFileDialogInternal.puFilterManager.puDLGdefaultExt = "." + ps.ext;
		}
		else
		{
			prFileDialogInternal.puFileManager.puDLGpath = prFileDialogInternal.puFileManager.GetCurrentPath();
			prFileDialogInternal.puFileManager.SetDefaultFileName("");
			prFileDialogInternal.puFilterManager.puDLGdefaultExt.clear();
		}

		prFileDialogInternal.puFilterManager.ParseFilters(vFilters);
		prFileDialogInternal.puFilterManager.SetSelectedFilterWithExt(
			prFileDialogInternal.puFilterManager.puDLGdefaultExt);
		
		prFileDialogInternal.puFileManager.SetCurrentPath(prFileDialogInternal.puFileManager.puDLGpath);

		prFileDialogInternal.puFileManager.puDLGDirectoryMode = (vFilters == nullptr);
		prFileDialogInternal.puFileManager.puDLGcountSelectionMax = vCountSelectionMax; //-V101

		prFileDialogInternal.puFileManager.ClearAll();
		
		prFileDialogInternal.puShowDialog = true;
	}

	// with pane
	// path and fileName can be specified
	void IGFD::FileDialog::OpenDialog(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vPath,
		const std::string& vFileName,
		const PaneFun& vSidePane,
		const float& vSidePaneWidth,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		prFileDialogInternal.ResetForNewDialog();

		prFileDialogInternal.puDLGkey = vKey;
		prFileDialogInternal.puDLGtitle = vTitle;
		prFileDialogInternal.puDLGuserDatas = vUserDatas;
		prFileDialogInternal.puDLGflags = vFlags;
		prFileDialogInternal.puDLGoptionsPane = vSidePane;
		prFileDialogInternal.puDLGoptionsPaneWidth = vSidePaneWidth;
		prFileDialogInternal.puDLGmodal = false;

		prFileDialogInternal.puFilterManager.puDLGdefaultExt.clear();
		prFileDialogInternal.puFilterManager.ParseFilters(vFilters);

		prFileDialogInternal.puFileManager.puDLGcountSelectionMax = vCountSelectionMax;
		prFileDialogInternal.puFileManager.puDLGDirectoryMode = (vFilters == nullptr);
		if (vPath.empty())
			prFileDialogInternal.puFileManager.puDLGpath = prFileDialogInternal.puFileManager.GetCurrentPath();
		else
			prFileDialogInternal.puFileManager.puDLGpath = vPath;

		prFileDialogInternal.puFileManager.SetCurrentPath(prFileDialogInternal.puFileManager.puDLGpath);

		prFileDialogInternal.puFileManager.SetDefaultFileName(vFileName);

		prFileDialogInternal.puFileManager.ClearAll();
		
		prFileDialogInternal.puShowDialog = true;					// open dialog
	}

	// with pane
	// path and filename are obtained from filePathName
	void IGFD::FileDialog::OpenDialog(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vFilePathName,
		const PaneFun& vSidePane,
		const float& vSidePaneWidth,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		prFileDialogInternal.ResetForNewDialog();

		prFileDialogInternal.puDLGkey = vKey;
		prFileDialogInternal.puDLGtitle = vTitle;
		prFileDialogInternal.puDLGoptionsPane = vSidePane;
		prFileDialogInternal.puDLGoptionsPaneWidth = vSidePaneWidth;
		prFileDialogInternal.puDLGuserDatas = vUserDatas;
		prFileDialogInternal.puDLGflags = vFlags;
		prFileDialogInternal.puDLGmodal = false;

		auto ps = IGFD::Utils::ParsePathFileName(vFilePathName);
		if (ps.isOk)
		{
			prFileDialogInternal.puFileManager.puDLGpath = ps.path;
			prFileDialogInternal.puFileManager.SetDefaultFileName(vFilePathName);
			prFileDialogInternal.puFilterManager.puDLGdefaultExt = "." + ps.ext;
		}
		else
		{
			prFileDialogInternal.puFileManager.puDLGpath = prFileDialogInternal.puFileManager.GetCurrentPath();
			prFileDialogInternal.puFileManager.SetDefaultFileName("");
			prFileDialogInternal.puFilterManager.puDLGdefaultExt.clear();
		}

		prFileDialogInternal.puFileManager.SetCurrentPath(prFileDialogInternal.puFileManager.puDLGpath);

		prFileDialogInternal.puFileManager.puDLGcountSelectionMax = vCountSelectionMax; //-V101
		prFileDialogInternal.puFileManager.puDLGDirectoryMode = (vFilters == nullptr);
		prFileDialogInternal.puFilterManager.ParseFilters(vFilters);
		prFileDialogInternal.puFilterManager.SetSelectedFilterWithExt(
			prFileDialogInternal.puFilterManager.puDLGdefaultExt);

		prFileDialogInternal.puFileManager.ClearAll();

		prFileDialogInternal.puShowDialog = true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	///// FILE DIALOG MODAL DIALOG ///////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	void IGFD::FileDialog::OpenModal(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vPath,
		const std::string& vFileName,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		OpenDialog(
			vKey, vTitle, vFilters,
			vPath, vFileName,
			vCountSelectionMax, vUserDatas, vFlags);

		prFileDialogInternal.puDLGmodal = true;
	}

	void IGFD::FileDialog::OpenModal(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vFilePathName,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		OpenDialog(
			vKey, vTitle, vFilters,
			vFilePathName,
			vCountSelectionMax, vUserDatas, vFlags);

		prFileDialogInternal.puDLGmodal = true;
	}

	// with pane
	// path and fileName can be specified
	void IGFD::FileDialog::OpenModal(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vPath,
		const std::string& vFileName,
		const PaneFun& vSidePane,
		const float& vSidePaneWidth,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		OpenDialog(
			vKey, vTitle, vFilters,
			vPath, vFileName,
			vSidePane, vSidePaneWidth,
			vCountSelectionMax, vUserDatas, vFlags);

		prFileDialogInternal.puDLGmodal = true;
	}

	// with pane
	// path and filename are obtained from filePathName
	void IGFD::FileDialog::OpenModal(
		const std::string& vKey,
		const std::string& vTitle,
		const char* vFilters,
		const std::string& vFilePathName,
		const PaneFun& vSidePane,
		const float& vSidePaneWidth,
		const int& vCountSelectionMax,
		UserDatas vUserDatas,
		ImGuiFileDialogFlags vFlags)
	{
		if (prFileDialogInternal.puShowDialog) // if already opened, quit
			return;

		OpenDialog(
			vKey, vTitle, vFilters,
			vFilePathName,
			vSidePane, vSidePaneWidth,
			vCountSelectionMax, vUserDatas, vFlags);

		prFileDialogInternal.puDLGmodal = true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	///// FILE DIALOG DISPLAY FUNCTION ///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////

	bool IGFD::FileDialog::Display(const std::string& vKey, ImGuiWindowFlags vFlags, ImVec2 vMinSize, ImVec2 vMaxSize)
	{
		bool res = false;

		if (prFileDialogInternal.puShowDialog && prFileDialogInternal.puDLGkey == vKey)
		{
			if (prFileDialogInternal.puUseCustomLocale)
				setlocale(prFileDialogInternal.puLocaleCategory, prFileDialogInternal.puLocaleBegin.c_str());

			auto& fdFile = prFileDialogInternal.puFileManager;
			auto& fdFilter = prFileDialogInternal.puFilterManager;

			static ImGuiWindowFlags flags;

			// to be sure than only one dialog is displayed per frame
			ImGuiContext& g = *GImGui;
			if (g.FrameCount == prFileDialogInternal.puLastImGuiFrameCount) // one instance was displayed this frame before for this key +> quit
				return res;
			prFileDialogInternal.puLastImGuiFrameCount = g.FrameCount; // mark this instance as used this frame

			std::string name = prFileDialogInternal.puDLGtitle + "##" + prFileDialogInternal.puDLGkey;
			if (prFileDialogInternal.puName != name)
			{
				fdFile.ClearComposer();
				fdFile.ClearFileLists();
				flags = vFlags;
			}

			NewFrame();

#ifdef IMGUI_HAS_VIEWPORT
			if (!ImGui::GetIO().ConfigViewportsNoDecoration)
			{
				// https://github.com/ocornut/imgui/issues/4534
				ImGuiWindowClass window_class;
				window_class.ViewportFlagsOverrideClear = ImGuiViewportFlags_NoDecoration;
				ImGui::SetNextWindowClass(&window_class);
			}
#endif // IMGUI_HAS_VIEWPORT

			ImGui::SetNextWindowSizeConstraints(vMinSize, vMaxSize);

			bool beg = false;
			if (prFileDialogInternal.puDLGmodal &&
				!prFileDialogInternal.puOkResultToConfirm) // disable modal because the confirm dialog for overwrite is a new modal
			{
				ImGui::OpenPopup(name.c_str());
				beg = ImGui::BeginPopupModal(name.c_str(), (bool*)nullptr,
					flags | ImGuiWindowFlags_NoScrollbar);
			}
			else
			{
				beg = ImGui::Begin(name.c_str(), (bool*)nullptr, flags | ImGuiWindowFlags_NoScrollbar);
			}
			if (beg)
			{
#ifdef IMGUI_HAS_VIEWPORT
				// if decoration is enabled we disable the resizing feature of imgui for avoid crash with SDL2 and GLFW3
				if (ImGui::GetIO().ConfigViewportsNoDecoration)
				{
					flags = vFlags;
				}
				else
				{
					auto win = ImGui::GetCurrentWindowRead();
					if (win->Viewport->Idx != 0)
						flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
					else
						flags = vFlags;
				}
#endif // IMGUI_HAS_VIEWPORT

				prFileDialogInternal.puName = name; //-V820
				puAnyWindowsHovered |= ImGui::IsWindowHovered();

				if (fdFile.puDLGpath.empty())
					fdFile.puDLGpath = "."; // defaut path is '.'

				fdFilter.SetDefaultFilterIfNotDefined();

				// init list of files
				if (fdFile.IsFileListEmpty() && !fdFile.puShowDrives)
				{
					IGFD::Utils::ReplaceString(fdFile.puDLGDefaultFileName, fdFile.puDLGpath, ""); // local path
					if (!fdFile.puDLGDefaultFileName.empty())
					{
						fdFile.SetDefaultFileName(fdFile.puDLGDefaultFileName);
						fdFilter.SetSelectedFilterWithExt(fdFilter.puDLGdefaultExt);
					}
					else if (fdFile.puDLGDirectoryMode) // directory mode
						fdFile.SetDefaultFileName(".");
					fdFile.ScanDir(prFileDialogInternal, fdFile.puDLGpath);
				}

				// draw dialog parts
				prDrawHeader(); // bookmark, directory, path
				prDrawContent(); // bookmark, files view, side pane 
				res = prDrawFooter(); // file field, filter combobox, ok/cancel buttons

				EndFrame();

				// for display in dialog center, the confirm to overwrite dlg
				prFileDialogInternal.puDialogCenterPos = ImGui::GetCurrentWindowRead()->ContentRegionRect.GetCenter();

				// when the confirm to overwrite dialog will appear we need to 
				// disable the modal mode of the main file dialog
				// see prOkResultToConfirm under
				if (prFileDialogInternal.puDLGmodal &&
					!prFileDialogInternal.puOkResultToConfirm)
					ImGui::EndPopup();
			}

			// same things here regarding prOkResultToConfirm
			if (!prFileDialogInternal.puDLGmodal || prFileDialogInternal.puOkResultToConfirm)
				ImGui::End();

			// confirm the result and show the confirm to overwrite dialog if needed
			res =  prConfirm_Or_OpenOverWriteFileDialog_IfNeeded(res, vFlags);
			
			if (prFileDialogInternal.puUseCustomLocale)
				setlocale(prFileDialogInternal.puLocaleCategory, prFileDialogInternal.puLocaleEnd.c_str());
		}

		return res;
	}

	void IGFD::FileDialog::NewFrame()
	{
		prFileDialogInternal.NewFrame();
		NewThumbnailFrame(prFileDialogInternal);
	}
	
	void IGFD::FileDialog::EndFrame()
	{
		EndThumbnailFrame(prFileDialogInternal);
		prFileDialogInternal.EndFrame();
		
	}
	void IGFD::FileDialog::QuitFrame()
	{
		QuitThumbnailFrame(prFileDialogInternal);
	}

	void IGFD::FileDialog::prDrawHeader()
	{
#ifdef USE_BOOKMARK
		prDrawBookmarkButton();
		ImGui::SameLine();
#endif // USE_BOOKMARK

		prFileDialogInternal.puFileManager.DrawDirectoryCreation(prFileDialogInternal);
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		prFileDialogInternal.puFileManager.DrawPathComposer(prFileDialogInternal);

#ifdef USE_THUMBNAILS
		if (!(prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_DisableThumbnailMode))
		{
			prDrawDisplayModeToolBar();
			ImGui::SameLine();
			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			ImGui::SameLine();
		}
#endif // USE_THUMBNAILS

		prFileDialogInternal.puSearchManager.DrawSearchBar(prFileDialogInternal);
	}

	void IGFD::FileDialog::prDrawContent()
	{
		ImVec2 size = ImGui::GetContentRegionAvail() - ImVec2(0.0f, prFileDialogInternal.puFooterHeight);

#ifdef USE_BOOKMARK
		if (prBookmarkPaneShown)
		{
			//size.x -= prBookmarkWidth;
			float otherWidth = size.x - prBookmarkWidth;
			ImGui::PushID("##splitterbookmark");
			IGFD::Utils::Splitter(true, 4.0f,
				&prBookmarkWidth, &otherWidth, 10.0f,
				10.0f + prFileDialogInternal.puDLGoptionsPaneWidth, size.y);
			ImGui::PopID();
			size.x -= otherWidth;
			prDrawBookmarkPane(prFileDialogInternal, size);
			ImGui::SameLine();
		}
#endif // USE_BOOKMARK

		size.x = ImGui::GetContentRegionAvail().x - prFileDialogInternal.puDLGoptionsPaneWidth;

		if (prFileDialogInternal.puDLGoptionsPane)
		{
			ImGui::PushID("##splittersidepane");
			IGFD::Utils::Splitter(true, 4.0f, &size.x, &prFileDialogInternal.puDLGoptionsPaneWidth, 10.0f, 10.0f, size.y);
			ImGui::PopID();
		}

#ifdef USE_THUMBNAILS
		if (prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_DisableThumbnailMode)
		{
			prDrawFileListView(size);
		}
		else
		{
			switch (prDisplayMode)
			{
			case DisplayModeEnum::FILE_LIST:
				prDrawFileListView(size);
				break;
			case DisplayModeEnum::THUMBNAILS_LIST:
				prDrawThumbnailsListView(size);
				break;
			case DisplayModeEnum::THUMBNAILS_GRID:
				prDrawThumbnailsGridView(size);
			}
		}
#else
		prDrawFileListView(size);
#endif // USE_THUMBNAILS

		if (prFileDialogInternal.puDLGoptionsPane)
		{
			prDrawSidePane(size.y);
		}
	}

	bool IGFD::FileDialog::prDrawFooter()
	{
		auto& fdFile = prFileDialogInternal.puFileManager;
		
		float posY = ImGui::GetCursorPos().y; // height of last bar calc

		if (!fdFile.puDLGDirectoryMode)
			ImGui::Text(fileNameString);
		else // directory chooser
			ImGui::Text(dirNameString);

		ImGui::SameLine();

		// Input file fields
		float width = ImGui::GetContentRegionAvail().x;
		if (!fdFile.puDLGDirectoryMode)
			width -= FILTER_COMBO_WIDTH;
		ImGui::PushItemWidth(width);
		ImGui::InputText("##FileName", fdFile.puFileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
		if (ImGui::GetItemID() == ImGui::GetActiveID())
			prFileDialogInternal.puFileInputIsActive = true;
		ImGui::PopItemWidth();

		// combobox of filters
		prFileDialogInternal.puFilterManager.DrawFilterComboBox(prFileDialogInternal);

		bool res = false;

		// OK Button
		if (prFileDialogInternal.puCanWeContinue && strlen(fdFile.puFileNameBuffer))
		{
			if (IMGUI_BUTTON(okButtonString "##validationdialog"))
			{
				prFileDialogInternal.puIsOk = true;
				res = true;
			}

			ImGui::SameLine();
		}

		// Cancel Button
		if (IMGUI_BUTTON(cancelButtonString "##validationdialog") || 
			prFileDialogInternal.puNeedToExitDialog) // dialog exit asked
		{
			prFileDialogInternal.puIsOk = false;
			res = true;
		}

		prFileDialogInternal.puFooterHeight = ImGui::GetCursorPosY() - posY;

		return res;
	}

	bool IGFD::FileDialog::prSelectableItem(int vidx, std::shared_ptr<FileInfos> vInfos, bool vSelected, const char* vFmt, ...)
	{
		if (!vInfos.use_count())
			return false;

		auto& fdi = prFileDialogInternal.puFileManager;

		static ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_AllowDoubleClick |
			ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SpanAvailWidth;

		va_list args;
		va_start(args, vFmt);
		vsnprintf(fdi.puVariadicBuffer, MAX_FILE_DIALOG_NAME_BUFFER, vFmt, args);
		va_end(args);

		float h = 0.0f;
#ifdef USE_THUMBNAILS
		if (prDisplayMode == DisplayModeEnum::THUMBNAILS_LIST)
			h = DisplayMode_ThumbailsList_ImageHeight;
#endif // USE_THUMBNAILS
#ifdef USE_EXPLORATION_BY_KEYS
		bool flashed = prBeginFlashItem(vidx);
		bool res = prFlashableSelectable(fdi.puVariadicBuffer, vSelected, selectableFlags,
			flashed, ImVec2(-1.0f, h));
		if (flashed)
			prEndFlashItem();
#else // USE_EXPLORATION_BY_KEYS
		(void)vidx; // remove a warnings ofr unused var

		bool res = ImGui::Selectable(fdi.puVariadicBuffer, vSelected, selectableFlags, ImVec2(-1.0f, h));
#endif // USE_EXPLORATION_BY_KEYS
		if (res)
		{
			if (vInfos->fileType == 'd')
			{
				// nav system, selectebale cause open directory or select directory
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)
				{
					if (fdi.puDLGDirectoryMode) // directory chooser
					{
						fdi.SelectFileName(prFileDialogInternal, vInfos);
					}
					else
					{
						fdi.puPathClicked = fdi.SelectDirectory(vInfos);
					}
				}
				else // no nav system => classic behavior
				{
					if (ImGui::IsMouseDoubleClicked(0)) // 0 -> left mouse button double click
					{
						fdi.puPathClicked = fdi.SelectDirectory(vInfos);
					}
					else if (fdi.puDLGDirectoryMode) // directory chooser
					{
						fdi.SelectFileName(prFileDialogInternal, vInfos);
					}
				}

				return true; // needToBreakTheloop
			}
			else
			{
				fdi.SelectFileName(prFileDialogInternal, vInfos);
			}
		}

		return false;
	}

	void IGFD::FileDialog::prDrawFileListView(ImVec2 vSize)
	{
		auto& fdi = prFileDialogInternal.puFileManager;

		ImGui::PushID(this);

		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg |
			ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY |
			ImGuiTableFlags_NoHostExtendY
#ifndef USE_CUSTOM_SORTING_ICON
			| ImGuiTableFlags_Sortable
#endif // USE_CUSTOM_SORTING_ICON
			;
		auto listViewID = ImGui::GetID("##FileDialog_fileTable");
		if (ImGui::BeginTableEx("##FileDialog_fileTable", listViewID, 4, flags, vSize, 0.0f))
		{
			ImGui::TableSetupScrollFreeze(0, 1); // Make header always visible
			ImGui::TableSetupColumn(fdi.puHeaderFileName.c_str(), ImGuiTableColumnFlags_WidthStretch, -1, 0);
			ImGui::TableSetupColumn(fdi.puHeaderFileType.c_str(), ImGuiTableColumnFlags_WidthFixed | 
				((prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_HideColumnType) ? ImGuiTableColumnFlags_DefaultHide : 0), -1, 1);
			ImGui::TableSetupColumn(fdi.puHeaderFileSize.c_str(), ImGuiTableColumnFlags_WidthFixed | 
				((prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_HideColumnSize) ? ImGuiTableColumnFlags_DefaultHide : 0), -1, 2);
			ImGui::TableSetupColumn(fdi.puHeaderFileDate.c_str(), ImGuiTableColumnFlags_WidthFixed | 
				((prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_HideColumnDate) ? ImGuiTableColumnFlags_DefaultHide : 0), -1, 3);

#ifndef USE_CUSTOM_SORTING_ICON
			// Sort our data if sort specs have been changed!
			if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			{
				if (sorts_specs->SpecsDirty && !fdi.IsFileListEmpty())
				{
					if (sorts_specs->Specs->ColumnUserID == 0)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME, true);
					else if (sorts_specs->Specs->ColumnUserID == 1)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_TYPE, true);
					else if (sorts_specs->Specs->ColumnUserID == 2)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_SIZE, true);
					else //if (sorts_specs->Specs->ColumnUserID == 3) => alwayd true for the moment, to uncomment if we add a fourth column
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_DATE, true);

					sorts_specs->SpecsDirty = false;
				}
			}

			ImGui::TableHeadersRow();
#else // USE_CUSTOM_SORTING_ICON
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			for (int column = 0; column < 4; column++)
			{
				ImGui::TableSetColumnIndex(column);
				const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
				ImGui::PushID(column);
				ImGui::TableHeader(column_name);
				ImGui::PopID();
				if (ImGui::IsItemClicked())
				{
					if (column == 0)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME, true);
					else if (column == 1)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_TYPE, true);
					else if (column == 2)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_SIZE, true);
					else //if (column == 3) => alwayd true for the moment, to uncomment if we add a fourth column
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_DATE, true);
				}
			}
#endif // USE_CUSTOM_SORTING_ICON
			if (!fdi.IsFilteredListEmpty())
			{
				prFileListClipper.Begin((int)fdi.GetFilteredListSize(), ImGui::GetTextLineHeightWithSpacing());
				while (prFileListClipper.Step())
				{
					for (int i = prFileListClipper.DisplayStart; i < prFileListClipper.DisplayEnd; i++)
					{
						if (i < 0) continue;

						auto infos = fdi.GetFilteredFileAt((size_t)i);
						if (!infos.use_count())
							continue;

						ImVec4 c;
						std::string icon;
						bool showColor = prFileDialogInternal.puFilterManager.GetExtentionInfos(infos->fileExt, &c, &icon);
						if (showColor)
							ImGui::PushStyleColor(ImGuiCol_Text, c);

						std::string str;// = " " + infos->fileName;
						if (infos->fileType == 'd') str = dirEntryString;
						else if (infos->fileType == 'l') str = linkEntryString;
						else if (infos->fileType == 'f')
						{
							if (showColor && !icon.empty())
								str = icon;
							else
								str = fileEntryString;
						}
						str += " " + infos->fileName;
						
						bool selected = fdi.IsFileNameSelected(infos->fileName); // found

						ImGui::TableNextRow();

						bool needToBreakTheloop = false;

						if (ImGui::TableNextColumn()) // file name
						{
							needToBreakTheloop = prSelectableItem(i, infos, selected, str.c_str());
						}
						if (ImGui::TableNextColumn()) // file type
						{
							ImGui::Text("%s", infos->fileExt.c_str());
						}
						if (ImGui::TableNextColumn()) // file size
						{
							if (infos->fileType != 'd')
							{
								ImGui::Text("%s ", infos->formatedFileSize.c_str());
							}
							else
							{
								ImGui::Text("");
							}
						}
						if (ImGui::TableNextColumn()) // file date + time
						{
							ImGui::Text("%s", infos->fileModifDate.c_str());
						}

						if (showColor)
							ImGui::PopStyleColor();

						if (needToBreakTheloop)
							break;
					}
				}
				prFileListClipper.End();
			}

#ifdef USE_EXPLORATION_BY_KEYS
			if (!fdi.puInputPathActivated)
			{
				prLocateByInputKey(prFileDialogInternal);
				prExploreWithkeys(prFileDialogInternal, listViewID);
			}
#endif // USE_EXPLORATION_BY_KEYS

			ImGuiContext& g = *GImGui;
			if (g.LastActiveId - 1 == listViewID || g.LastActiveId == listViewID)
			{
				prFileDialogInternal.puFileListViewIsActive = true;
			}

			ImGui::EndTable();
		}

		ImGui::PopID();
	}

#ifdef USE_THUMBNAILS
	void IGFD::FileDialog::prDrawThumbnailsListView(ImVec2 vSize)
	{
		auto& fdi = prFileDialogInternal.puFileManager;

		ImGui::PushID(this);

		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg |
			ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY |
			ImGuiTableFlags_NoHostExtendY
#ifndef USE_CUSTOM_SORTING_ICON
			| ImGuiTableFlags_Sortable
#endif // USE_CUSTOM_SORTING_ICON
			;
		auto listViewID = ImGui::GetID("##FileDialog_fileTable");
		if (ImGui::BeginTableEx("##FileDialog_fileTable", listViewID, 5, flags, vSize, 0.0f))
		{
			ImGui::TableSetupScrollFreeze(0, 1); // Make header always visible
			ImGui::TableSetupColumn(fdi.puHeaderFileName.c_str(), ImGuiTableColumnFlags_WidthStretch, -1, 0);
			ImGui::TableSetupColumn(fdi.puHeaderFileType.c_str(), ImGuiTableColumnFlags_WidthFixed |
				((prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_HideColumnType) ? ImGuiTableColumnFlags_DefaultHide : 0), -1, 1);
			ImGui::TableSetupColumn(fdi.puHeaderFileSize.c_str(), ImGuiTableColumnFlags_WidthFixed |
				((prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_HideColumnSize) ? ImGuiTableColumnFlags_DefaultHide : 0), -1, 2);
			ImGui::TableSetupColumn(fdi.puHeaderFileDate.c_str(), ImGuiTableColumnFlags_WidthFixed |
				((prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_HideColumnDate) ? ImGuiTableColumnFlags_DefaultHide : 0), -1, 3);
			// not needed to have an option for hide the thumbnails since this is why this view is used
			ImGui::TableSetupColumn(fdi.puHeaderFileThumbnails.c_str(), ImGuiTableColumnFlags_WidthFixed, -1, 4);

#ifndef USE_CUSTOM_SORTING_ICON
			// Sort our data if sort specs have been changed!
			if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			{
				if (sorts_specs->SpecsDirty && !fdi.IsFileListEmpty())
				{
					if (sorts_specs->Specs->ColumnUserID == 0)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME, true);
					else if (sorts_specs->Specs->ColumnUserID == 1)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_TYPE, true);
					else if (sorts_specs->Specs->ColumnUserID == 2)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_SIZE, true);
					else if (sorts_specs->Specs->ColumnUserID == 3)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_DATE, true);
					else // if (sorts_specs->Specs->ColumnUserID == 4) = > always true for the moment, to uncomment if we add another column
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_THUMBNAILS, true);
					sorts_specs->SpecsDirty = false;
				}
			}

			ImGui::TableHeadersRow();
#else // USE_CUSTOM_SORTING_ICON
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			for (int column = 0; column < 5; column++)
			{
				ImGui::TableSetColumnIndex(column);
				const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
				ImGui::PushID(column);
				ImGui::TableHeader(column_name);
				ImGui::PopID();
				if (ImGui::IsItemClicked())
				{
					if (column == 0)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_FILENAME, true);
					else if (column == 1)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_TYPE, true);
					else if (column == 2)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_SIZE, true);
					else if (column == 3)
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_DATE, true);
					else // if (column == 4) = > always true for the moment, to uncomment if we add another column
						fdi.SortFields(prFileDialogInternal, IGFD::FileManager::SortingFieldEnum::FIELD_THUMBNAILS, true);
				}
			}
#endif // USE_CUSTOM_SORTING_ICON
			if (!fdi.IsFilteredListEmpty())
			{
				ImGuiContext& g = *GImGui;
				const float itemHeight = ImMax(g.FontSize, DisplayMode_ThumbailsList_ImageHeight) + g.Style.ItemSpacing.y;

				prFileListClipper.Begin((int)fdi.GetFilteredListSize(), itemHeight);
				while (prFileListClipper.Step())
				{
					for (int i = prFileListClipper.DisplayStart; i < prFileListClipper.DisplayEnd; i++)
					{
						if (i < 0) continue;

						auto infos = fdi.GetFilteredFileAt((size_t)i);
						if (!infos.use_count())
							continue;

						ImVec4 c;
						std::string icon;
						bool showColor = prFileDialogInternal.puFilterManager.GetExtentionInfos(infos->fileExt, &c, &icon);
						if (showColor)
							ImGui::PushStyleColor(ImGuiCol_Text, c);

						std::string str;
						if (infos->fileType == 'd') str = dirEntryString;
						else if (infos->fileType == 'l') str = linkEntryString;
						else if (infos->fileType == 'f')
						{
							if (showColor && !icon.empty())
								str = icon;
							else
								str = fileEntryString;
						}
						str += " " + infos->fileName;
						bool selected = fdi.IsFileNameSelected(infos->fileName); // found

						ImGui::TableNextRow();

						bool needToBreakTheloop = false;

						if (ImGui::TableNextColumn()) // file name
						{
							needToBreakTheloop = prSelectableItem(i, infos, selected, str.c_str());
						}
						if (ImGui::TableNextColumn()) // file type
						{
							ImGui::Text("%s", infos->fileExt.c_str());
						}
						if (ImGui::TableNextColumn()) // file size
						{
							if (infos->fileType != 'd')
							{
								ImGui::Text("%s ", infos->formatedFileSize.c_str());
							}
							else
							{
								ImGui::Text("");
							}
						}
						if (ImGui::TableNextColumn()) // file date + time
						{
							ImGui::Text("%s", infos->fileModifDate.c_str());
						}
						if (ImGui::TableNextColumn()) // file thumbnails
						{
							if (!infos->thumbnailInfo.isLoadingOrLoaded)
							{
								prAddThumbnailToLoad(infos);
							}
							if (infos->thumbnailInfo.isReadyToDisplay && 
								infos->thumbnailInfo.textureID)
							{
								ImGui::Image((ImTextureID)infos->thumbnailInfo.textureID, 
									ImVec2((float)infos->thumbnailInfo.textureWidth, 
										(float)infos->thumbnailInfo.textureHeight));
							}
						}

						if (showColor)
							ImGui::PopStyleColor();

						if (needToBreakTheloop)
							break;
					}
				}
				prFileListClipper.End();
			}

#ifdef USE_EXPLORATION_BY_KEYS
			if (!fdi.puInputPathActivated)
			{
				prLocateByInputKey(prFileDialogInternal);
				prExploreWithkeys(prFileDialogInternal, listViewID);
			}
#endif // USE_EXPLORATION_BY_KEYS

			ImGuiContext& g = *GImGui;
			if (g.LastActiveId - 1 == listViewID || g.LastActiveId == listViewID)
			{
				prFileDialogInternal.puFileListViewIsActive = true;
			}

			ImGui::EndTable();
		}

		ImGui::PopID();
	}

	void IGFD::FileDialog::prDrawThumbnailsGridView(ImVec2 vSize)
	{
		if (ImGui::BeginChild("##thumbnailsGridsFiles", vSize))
		{
			// todo
		}

		ImGui::EndChild();
	}

#endif

	void IGFD::FileDialog::prDrawSidePane(float vHeight)
	{
		ImGui::SameLine();

		ImGui::BeginChild("##FileTypes", ImVec2(0, vHeight));

		prFileDialogInternal.puDLGoptionsPane(
			prFileDialogInternal.puFilterManager.GetSelectedFilter().filter.c_str(), 
			prFileDialogInternal.puDLGuserDatas, &prFileDialogInternal.puCanWeContinue);

		ImGui::EndChild();
	}

	void IGFD::FileDialog::Close()
	{
		prFileDialogInternal.puDLGkey.clear();
		prFileDialogInternal.puShowDialog = false;
	}

	bool IGFD::FileDialog::WasOpenedThisFrame(const std::string& vKey) const
	{
		bool res = prFileDialogInternal.puShowDialog && prFileDialogInternal.puDLGkey == vKey;
		if (res)
		{
			ImGuiContext& g = *GImGui;
			res &= prFileDialogInternal.puLastImGuiFrameCount == g.FrameCount; // return true if a dialog was displayed in this frame
		}
		return res;
	}

	bool IGFD::FileDialog::WasOpenedThisFrame() const
	{
		bool res = prFileDialogInternal.puShowDialog;
		if (res)
		{
			ImGuiContext& g = *GImGui;
			res &= prFileDialogInternal.puLastImGuiFrameCount == g.FrameCount; // return true if a dialog was displayed in this frame
		}
		return res;
	}

	bool IGFD::FileDialog::IsOpened(const std::string& vKey) const
	{
		return (prFileDialogInternal.puShowDialog && prFileDialogInternal.puDLGkey == vKey);
	}

	bool IGFD::FileDialog::IsOpened() const
	{
		return prFileDialogInternal.puShowDialog;
	}

	std::string IGFD::FileDialog::GetOpenedKey() const
	{
		if (prFileDialogInternal.puShowDialog)
			return prFileDialogInternal.puDLGkey;
		return "";
	}

	std::string IGFD::FileDialog::GetFilePathName()
	{
		return prFileDialogInternal.puFileManager.GetResultingFilePathName(prFileDialogInternal);
	}

	std::string IGFD::FileDialog::GetCurrentPath()
	{
		return prFileDialogInternal.puFileManager.GetResultingPath();
	}

	std::string IGFD::FileDialog::GetCurrentFileName()
	{
		return prFileDialogInternal.puFileManager.GetResultingFileName(prFileDialogInternal);
	}

	std::string IGFD::FileDialog::GetCurrentFilter()
	{
		return prFileDialogInternal.puFilterManager.GetSelectedFilter().filter;
	}

	std::map<std::string, std::string> IGFD::FileDialog::GetSelection()
	{
		return prFileDialogInternal.puFileManager.GetResultingSelection();
	}

	UserDatas IGFD::FileDialog::GetUserDatas() const
	{
		return prFileDialogInternal.puDLGuserDatas;
	}

	bool IGFD::FileDialog::IsOk() const
	{
		return prFileDialogInternal.puIsOk;
	}

	void IGFD::FileDialog::SetExtentionInfos(const std::string& vFilter, const FileExtentionInfos& vInfos)
	{
		prFileDialogInternal.puFilterManager.SetExtentionInfos(vFilter, vInfos);
	}

	void IGFD::FileDialog::SetExtentionInfos(const std::string& vFilter, const ImVec4& vColor, const std::string& vIcon)
	{
		prFileDialogInternal.puFilterManager.SetExtentionInfos(vFilter, vColor, vIcon);
	}

	bool IGFD::FileDialog::GetExtentionInfos(const std::string& vFilter, ImVec4* vOutColor, std::string* vOutIcon)
	{
		return prFileDialogInternal.puFilterManager.GetExtentionInfos(vFilter, vOutColor, vOutIcon);
	}

	void IGFD::FileDialog::ClearExtentionInfos()
	{
		prFileDialogInternal.puFilterManager.ClearExtentionInfos();
	}

	void IGFD::FileDialog::SetLocales(const int& vLocaleCategory, const std::string& vLocaleBegin, const std::string& vLocaleEnd)
	{
		prFileDialogInternal.puUseCustomLocale = true;
		prFileDialogInternal.puLocaleBegin = vLocaleBegin;
		prFileDialogInternal.puLocaleEnd = vLocaleEnd;
	}

	//////////////////////////////////////////////////////////////////////////////
	//// OVERWRITE DIALOG ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	bool IGFD::FileDialog::prConfirm_Or_OpenOverWriteFileDialog_IfNeeded(bool vLastAction, ImGuiWindowFlags vFlags)
	{
		// if confirmation => return true for confirm the overwrite et quit the dialog
		// if cancel => return false && set IsOk to false for keep inside the dialog

		// if IsOk == false => return false for quit the dialog
		if (!prFileDialogInternal.puIsOk && vLastAction)
		{
			QuitFrame();
			return true;
		}

		// if IsOk == true && no check of overwrite => return true for confirm the dialog
		if (prFileDialogInternal.puIsOk && vLastAction && !(prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_ConfirmOverwrite))
		{
			QuitFrame();
			return true;
		}

		// if IsOk == true && check of overwrite => return false and show confirm to overwrite dialog
		if ((prFileDialogInternal.puOkResultToConfirm || (prFileDialogInternal.puIsOk && vLastAction)) && 
			(prFileDialogInternal.puDLGflags & ImGuiFileDialogFlags_ConfirmOverwrite))
		{
			if (prFileDialogInternal.puIsOk) // catched only one time
			{
				if (!prFileDialogInternal.puFileManager.IsFileExist(GetFilePathName())) // not existing => quit dialog
				{
					QuitFrame();
					return true;
				}
				else // existing => confirm dialog to open
				{
					prFileDialogInternal.puIsOk = false;
					prFileDialogInternal.puOkResultToConfirm = true;
				}
			}

			std::string name = OverWriteDialogTitleString "##" + prFileDialogInternal.puDLGtitle + prFileDialogInternal.puDLGkey + "OverWriteDialog";

			bool res = false;

			ImGui::OpenPopup(name.c_str());
			if (ImGui::BeginPopupModal(name.c_str(), (bool*)0,
				vFlags | ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				ImGui::SetWindowPos(prFileDialogInternal.puDialogCenterPos - ImGui::GetWindowSize() * 0.5f); // next frame needed for GetWindowSize to work

				ImGui::Text("%s", OverWriteDialogMessageString);

				if (IMGUI_BUTTON(OverWriteDialogConfirmButtonString))
				{
					prFileDialogInternal.puOkResultToConfirm = false;
					prFileDialogInternal.puIsOk = true;
					res = true;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (IMGUI_BUTTON(OverWriteDialogCancelButtonString))
				{
					prFileDialogInternal.puOkResultToConfirm = false;
					prFileDialogInternal.puIsOk = false;
					res = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (res)
			{
				QuitFrame();
			}
			return res;
		}

		return false;
	}
}

#endif // __cplusplus

/////////////////////////////////////////////////////////////////
///// C Interface ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// Return an initialized IGFD_Selection_Pair
IMGUIFILEDIALOG_API IGFD_Selection_Pair IGFD_Selection_Pair_Get(void)
{
	IGFD_Selection_Pair res = {};
	res.fileName = nullptr;
	res.filePathName = nullptr;
	return res;
}

// destroy only the content of vSelection_Pair
IMGUIFILEDIALOG_API void IGFD_Selection_Pair_DestroyContent(IGFD_Selection_Pair* vSelection_Pair)
{
	if (vSelection_Pair)
	{
		delete[] vSelection_Pair->fileName;
		delete[] vSelection_Pair->filePathName;
	}
}

// Return an initialized IGFD_Selection
IMGUIFILEDIALOG_API IGFD_Selection IGFD_Selection_Get(void)
{
	return { nullptr, 0U };
}

// destroy only the content of vSelection
IMGUIFILEDIALOG_API void IGFD_Selection_DestroyContent(IGFD_Selection* vSelection)
{
	if (vSelection)
	{
		if (vSelection->table)
		{
			for (size_t i = 0U; i < vSelection->count; i++)
			{
				IGFD_Selection_Pair_DestroyContent(&vSelection->table[i]);
			}
			delete[] vSelection->table;
		}
		vSelection->count = 0U;
	}
}

// create an instance of ImGuiFileDialog
IMGUIFILEDIALOG_API ImGuiFileDialog* IGFD_Create(void)
{
	return new ImGuiFileDialog();
}

// destroy the instance of ImGuiFileDialog
IMGUIFILEDIALOG_API void IGFD_Destroy(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		delete vContext;
		vContext = nullptr;
	}
}

// standard dialog
IMGUIFILEDIALOG_API void IGFD_OpenDialog(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vPath,
	const char* vFileName,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenDialog(
			vKey, vTitle, vFilters, vPath, vFileName,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API void IGFD_OpenDialog2(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vFilePathName,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenDialog(
			vKey, vTitle, vFilters, vFilePathName,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API void IGFD_OpenPaneDialog(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vPath,
	const char* vFileName,
	IGFD_PaneFun vSidePane,
	const float vSidePaneWidth,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenDialog(
			vKey, vTitle, vFilters,
			vPath, vFileName,
			vSidePane, vSidePaneWidth,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API void IGFD_OpenPaneDialog2(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vFilePathName,
	IGFD_PaneFun vSidePane,
	const float vSidePaneWidth,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenDialog(
			vKey, vTitle, vFilters,
			vFilePathName,
			vSidePane, vSidePaneWidth,
			vCountSelectionMax, vUserDatas, flags);
	}
}

// modal dialog
IMGUIFILEDIALOG_API void IGFD_OpenModal(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vPath,
	const char* vFileName,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenModal(
			vKey, vTitle, vFilters, vPath, vFileName,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API void IGFD_OpenModal2(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vFilePathName,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenModal(
			vKey, vTitle, vFilters, vFilePathName,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API void IGFD_OpenPaneModal(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vPath,
	const char* vFileName,
	IGFD_PaneFun vSidePane,
	const float vSidePaneWidth,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenModal(
			vKey, vTitle, vFilters,
			vPath, vFileName,
			vSidePane, vSidePaneWidth,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API void IGFD_OpenPaneModal2(
	ImGuiFileDialog* vContext,
	const char* vKey,
	const char* vTitle,
	const char* vFilters,
	const char* vFilePathName,
	IGFD_PaneFun vSidePane,
	const float vSidePaneWidth,
	const int vCountSelectionMax,
	void* vUserDatas,
	ImGuiFileDialogFlags flags)
{
	if (vContext)
	{
		vContext->OpenDialog(
			vKey, vTitle, vFilters,
			vFilePathName,
			vSidePane, vSidePaneWidth,
			vCountSelectionMax, vUserDatas, flags);
	}
}

IMGUIFILEDIALOG_API bool IGFD_DisplayDialog(ImGuiFileDialog* vContext,
	const char* vKey, ImGuiWindowFlags vFlags, ImVec2 vMinSize, ImVec2 vMaxSize)
{
	if (vContext)
	{
		return vContext->Display(vKey, vFlags, vMinSize, vMaxSize);
	}

	return false;
}

IMGUIFILEDIALOG_API void IGFD_CloseDialog(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		vContext->Close();
	}
}

IMGUIFILEDIALOG_API bool IGFD_IsOk(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		return vContext->IsOk();
	}

	return false;
}

IMGUIFILEDIALOG_API bool IGFD_WasKeyOpenedThisFrame(ImGuiFileDialog* vContext,
	const char* vKey)
{
	if (vContext)
	{
		vContext->WasOpenedThisFrame(vKey);
	}

	return false;
}

IMGUIFILEDIALOG_API bool IGFD_WasOpenedThisFrame(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		vContext->WasOpenedThisFrame();
	}

	return false;
}

IMGUIFILEDIALOG_API bool IGFD_IsKeyOpened(ImGuiFileDialog* vContext,
	const char* vCurrentOpenedKey)
{
	if (vContext)
	{
		vContext->IsOpened(vCurrentOpenedKey);
	}

	return false;
}

IMGUIFILEDIALOG_API bool IGFD_IsOpened(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		vContext->IsOpened();
	}

	return false;
}

IMGUIFILEDIALOG_API IGFD_Selection IGFD_GetSelection(ImGuiFileDialog* vContext)
{
	IGFD_Selection res = IGFD_Selection_Get();

	if (vContext)
	{
		auto sel = vContext->GetSelection();
		if (!sel.empty())
		{
			res.count = sel.size();
			res.table = new IGFD_Selection_Pair[res.count];

			size_t idx = 0U;
			for (const auto& s : sel)
			{
				IGFD_Selection_Pair* pair = res.table + idx++;

				// fileName
				if (!s.first.empty())
				{
					size_t siz = s.first.size() + 1U;
					pair->fileName = new char[siz];
#ifndef MSVC
					strncpy(pair->fileName, s.first.c_str(), siz);
#else
					strncpy_s(pair->fileName, siz, s.first.c_str(), siz);
#endif
					pair->fileName[siz - 1U] = '\0';
				}

				// filePathName
				if (!s.second.empty())
				{
					size_t siz = s.first.size() + 1U;
					pair->filePathName = new char[siz];
#ifndef MSVC
					strncpy(pair->filePathName, s.first.c_str(), siz);
#else
					strncpy_s(pair->filePathName, siz, s.first.c_str(), siz);
#endif
					pair->filePathName[siz - 1U] = '\0';
				}
			}

			return res;
		}
	}

	return res;
}

IMGUIFILEDIALOG_API char* IGFD_GetFilePathName(ImGuiFileDialog* vContext)
{
	char* res = nullptr;

	if (vContext)
	{
		auto s = vContext->GetFilePathName();
		if (!s.empty())
		{
			size_t siz = s.size() + 1U;
			res = new char[siz];
#ifndef MSVC
			strncpy(res, s.c_str(), siz);
#else
			strncpy_s(res, siz, s.c_str(), siz);
#endif
			res[siz - 1U] = '\0';
		}
	}

	return res;
}

IMGUIFILEDIALOG_API char* IGFD_GetCurrentFileName(ImGuiFileDialog* vContext)
{
	char* res = nullptr;

	if (vContext)
	{
		auto s = vContext->GetCurrentFileName();
		if (!s.empty())
		{
			size_t siz = s.size() + 1U;
			res = new char[siz];
#ifndef MSVC
			strncpy(res, s.c_str(), siz);
#else
			strncpy_s(res, siz, s.c_str(), siz);
#endif
			res[siz - 1U] = '\0';
		}
	}

	return res;
}

IMGUIFILEDIALOG_API char* IGFD_GetCurrentPath(ImGuiFileDialog* vContext)
{
	char* res = nullptr;

	if (vContext)
	{
		auto s = vContext->GetCurrentPath();
		if (!s.empty())
		{
			size_t siz = s.size() + 1U;
			res = new char[siz];
#ifndef MSVC
			strncpy(res, s.c_str(), siz);
#else
			strncpy_s(res, siz, s.c_str(), siz);
#endif
			res[siz - 1U] = '\0';
		}
	}

	return res;
}

IMGUIFILEDIALOG_API char* IGFD_GetCurrentFilter(ImGuiFileDialog* vContext)
{
	char* res = nullptr;

	if (vContext)
	{
		auto s = vContext->GetCurrentFilter();
		if (!s.empty())
		{
			size_t siz = s.size() + 1U;
			res = new char[siz];
#ifndef MSVC
			strncpy(res, s.c_str(), siz);
#else
			strncpy_s(res, siz, s.c_str(), siz);
#endif
			res[siz - 1U] = '\0';
		}
	}

	return res;
}

IMGUIFILEDIALOG_API void* IGFD_GetUserDatas(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		return vContext->GetUserDatas();
	}

	return nullptr;
}

IMGUIFILEDIALOG_API void IGFD_SetExtentionInfos(ImGuiFileDialog* vContext,
	const char* vFilter, ImVec4 vColor, const char* vIcon)
{
	if (vContext)
	{
		vContext->SetExtentionInfos(vFilter, vColor, vIcon);
	}
}

IMGUIFILEDIALOG_API void IGFD_SetExtentionInfos2(ImGuiFileDialog* vContext,
	const char* vFilter, float vR, float vG, float vB, float vA, const char* vIcon)
{
	if (vContext)
	{
		vContext->SetExtentionInfos(vFilter, ImVec4(vR, vG, vB, vA), vIcon);
	}
}

IMGUIFILEDIALOG_API bool IGFD_GetExtentionInfos(ImGuiFileDialog* vContext,
	const char* vFilter, ImVec4* vOutColor, char** vOutIcon)
{
	if (vContext)
	{
		std::string icon;
		bool res = vContext->GetExtentionInfos(vFilter, vOutColor, &icon);
		if (!icon.empty() && vOutIcon)
		{
			size_t siz = icon.size() + 1U;
			*vOutIcon = new char[siz];
#ifndef MSVC
			strncpy(*vOutIcon, icon.c_str(), siz);
#else
			strncpy_s(*vOutIcon, siz, icon.c_str(), siz);
#endif
			(*vOutIcon)[siz - 1U] = '\0';
		}
		return res;
	}

	return false;
}

IMGUIFILEDIALOG_API void IGFD_ClearExtentionInfos(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		vContext->ClearExtentionInfos();
	}
}
IMGUIFILEDIALOG_API void SetLocales(ImGuiFileDialog* vContext, const int vCategory, const char* vBeginLocale, const char* vEndLocale)
{
	if (vContext)
	{
		vContext->SetLocales(vCategory, (vBeginLocale ? vBeginLocale : ""), (vEndLocale ? vEndLocale : ""));
	}
}

#ifdef USE_EXPLORATION_BY_KEYS
IMGUIFILEDIALOG_API void IGFD_SetFlashingAttenuationInSeconds(ImGuiFileDialog* vContext, float vAttenValue)
{
	if (vContext)
	{
		vContext->SetFlashingAttenuationInSeconds(vAttenValue);
	}
}
#endif

#ifdef USE_BOOKMARK
IMGUIFILEDIALOG_API char* IGFD_SerializeBookmarks(ImGuiFileDialog* vContext)
{
	char* res = nullptr;

	if (vContext)
	{
		auto s = vContext->SerializeBookmarks();
		if (!s.empty())
		{
			size_t siz = s.size() + 1U;
			res = new char[siz];
#ifndef MSVC
			strncpy(res, s.c_str(), siz);
#else
			strncpy_s(res, siz, s.c_str(), siz);
#endif
			res[siz - 1U] = '\0';
		}
	}

	return res;
}

IMGUIFILEDIALOG_API void IGFD_DeserializeBookmarks(ImGuiFileDialog* vContext, const char* vBookmarks)
{
	if (vContext)
	{
		vContext->DeserializeBookmarks(vBookmarks);
	}
}
#endif

#ifdef USE_THUMBNAILS
IMGUIFILEDIALOG_API void SetCreateThumbnailCallback(ImGuiFileDialog* vContext, const IGFD_CreateThumbnailFun vCreateThumbnailFun)
{
	if (vContext)
	{
		vContext->SetCreateThumbnailCallback(vCreateThumbnailFun);
	}
}

IMGUIFILEDIALOG_API void SetDestroyThumbnailCallback(ImGuiFileDialog* vContext, const IGFD_DestroyThumbnailFun vDestroyThumbnailFun)
{
	if (vContext)
	{
		vContext->SetDestroyThumbnailCallback(vDestroyThumbnailFun);
	}
}

IMGUIFILEDIALOG_API void ManageGPUThumbnails(ImGuiFileDialog* vContext)
{
	if (vContext)
	{
		vContext->ManageGPUThumbnails();
	}
}
#endif // USE_THUMBNAILS
