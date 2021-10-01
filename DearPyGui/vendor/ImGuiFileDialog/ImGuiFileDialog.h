/*
MIT License

Copyright (c) 2019-2021 Stephane Cuillerdier (aka aiekick)

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

/*
-----------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------

github repo : https://github.com/aiekick/ImGuiFileDialog

-----------------------------------------------------------------------------------------------------------------
## Description :
-----------------------------------------------------------------------------------------------------------------

this File Dialog is build on top of DearImGui
(On windows, need te lib Dirent : https://github.com/tronkko/dirent, use the branch 1.23 for avoid any issues)
Complete readme here : https://github.com/aiekick/ImGuiFileDialog/blob/master/README.md)

this filedialog was created principally for have custom pane with widgets accrdoing to file extention.
it was not possible with native filedialog

An example of the File Dialog integrated within the ImGui Demo App

-----------------------------------------------------------------------------------------------------------------
## Features :
-----------------------------------------------------------------------------------------------------------------

- Separate system for call and display
  - can be many func calls with different params for one display func by ex
- Can use custom pane via function binding
  - this pane can block the validation of the dialog
  - can also display different things according to current filter and User Datas
- Support of Filter Custom Coloring / Icons / text
- Multi Selection (ctrl/shift + click) :
  - 0 => infinite
  - 1 => one file (default)
  - n => n files
- Compatible with MacOs, Linux, Win
  - On Win version you can list Drives
- Support of Modal/Standard dialog type
- Support both Mode : File Chooser or Directory Chooser
- Support filter collection / Custom filter name
- Support files Exploring with keys : Up / Down / Enter (open dir) / Backspace (come back)
- Support files Exploring by input char (case insensitive)
- Support bookmark creation/edition/call for directory (can have custom name corresponding to a path)
- Support input path edition by right click on a path button
- Support of a 'Confirm to Overwrite" dialog if File Exist


-----------------------------------------------------------------------------------------------------------------
## NameSpace / SingleTon
-----------------------------------------------------------------------------------------------------------------

Use the Namespace IGFD (for avoid conflict with variables, struct and class names)

you can display only one dialog at a time, this class is a simgleton and must be called like that :
ImGuiFileDialog::Instance()->method_of_your_choice()

-----------------------------------------------------------------------------------------------------------------
## Simple Dialog :
-----------------------------------------------------------------------------------------------------------------

Example code :
void drawGui()
{
  // open Dialog Simple
  if (ImGui::Button("Open File Dialog"))
	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");

  // display
  if (ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
  {
	// action if OK
	if (ImGuiFileDialog::Instance()->IsOk == true)
	{
	  std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
	  std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  // action
	}
	// close
	ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
  }
}

-----------------------------------------------------------------------------------------------------------------
## Directory Chooser :
-----------------------------------------------------------------------------------------------------------------

For have only a directory chooser, you just need to specify a filter null :

Example code :
ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", 0, ".");

In this mode you can select any directory with one click, and open directory with double click

-----------------------------------------------------------------------------------------------------------------
## Dialog with Custom Pane :
-----------------------------------------------------------------------------------------------------------------

Example code :
static bool canValidateDialog = false;
inline void InfosPane(std::string& vFilter, IGFD::UserDatas vUserDatas, bool *vCantContinue) // if vCantContinue is false, the user cant validate the dialog
{
	ImGui::TextColored(ImVec4(0, 1, 1, 1), "Infos Pane");
	ImGui::Text("Selected Filter : %s", vFilter.c_str());
	if (vUserDatas)
		ImGui::Text("UserDatas : %s", vUserDatas);
	ImGui::Checkbox("if not checked you cant validate the dialog", &canValidateDialog);
	if (vCantContinue)
		*vCantContinue = canValidateDialog;
}

void drawGui()
{
  // open Dialog with Pane
  if (ImGui::Button("Open File Dialog with a custom pane"))
	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp",
			".", "", std::bind(&InfosPane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 350, 1, IGFD::UserDatas("InfosPane"));

  // display and action if ok
  if (ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
  {
	if (ImGuiFileDialog::Instance()->IsOk == true)
	{
		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
		std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
		// here convert from string because a string was passed as a userDatas, but it can be what you want
		std::string userDatas;
		if (ImGuiFileDialog::Instance()->GetUserDatas())
			userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
		auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

		// action
	}
	// close
	ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
  }
}

-----------------------------------------------------------------------------------------------------------------
## Filter Infos
-----------------------------------------------------------------------------------------------------------------

You can define color for a filter type
Example code :
ImGuiFileDialog::Instance()->SetExtentionInfos(".cpp", ImVec4(1,1,0, 0.9));
ImGuiFileDialog::Instance()->SetExtentionInfos(".h", ImVec4(0,1,0, 0.9));
ImGuiFileDialog::Instance()->SetExtentionInfos(".hpp", ImVec4(0,0,1, 0.9));
ImGuiFileDialog::Instance()->SetExtentionInfos(".md", ImVec4(1,0,1, 0.9));


![alt text](doc/color_filter.png)

and also specific icons (with icon font files) or file type names :

Example code :
// add an icon for png files
ImGuiFileDialog::Instance()->SetExtentionInfos(".png", ImVec4(0,1,1,0.9), ICON_IMFDLG_FILE_TYPE_PIC);
// add a text for gif files (the default value is [File]
ImGuiFileDialog::Instance()->SetExtentionInfos(".gif", ImVec4(0, 1, 0.5, 0.9), "[GIF]");


![alt text](doc/filter_Icon.png)

-----------------------------------------------------------------------------------------------------------------
## Filter Collections
-----------------------------------------------------------------------------------------------------------------

you can define a custom filter name who correspond to a group of filter

you must use this syntax : custom_name1{filter1,filter2,filter3},custom_name2{filter1,filter2},filter1
when you will select custom_name1, the gorup of filter 1 to 3 will be applied
the reserved char are {}, you cant use them for define filter name.

Example code :
const char *filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", ICON_IMFDLG_FOLDER_OPEN " Choose a File", filters, ".");

## Multi Selection

You can define in OpenDialog/OpenModal call the count file you wan to select :
- 0 => infinite
- 1 => one file only (default)
- n => n files only

See the define at the end of these funcs after path.

Example code :
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*,.cpp,.h,.hpp", ".");
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose 1 File", ".*,.cpp,.h,.hpp", ".", 1);
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose 5 File", ".*,.cpp,.h,.hpp", ".", 5);
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose many File", ".*,.cpp,.h,.hpp", ".", 0);
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png,.jpg",
   ".", "", std::bind(&InfosPane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 350, 1, "SaveFile"); // 1 file

-----------------------------------------------------------------------------------------------------------------
## File Dialog Constraints
-----------------------------------------------------------------------------------------------------------------

you can define min/max size of the dialog when you display It

by ex :

* MaxSize is the full display size
* MinSize in the half display size.

Example code :
ImVec2 maxSize = ImVec2((float)display_w, (float)display_h);
ImVec2 minSize = maxSize * 0.5f;
ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize);

-----------------------------------------------------------------------------------------------------------------
## Detail View Mode
-----------------------------------------------------------------------------------------------------------------

You can have tables display like that.

- uncomment "#define USE_IMGUI_TABLES" in you custom config file (CustomImGuiFileDialogConfig.h in this example)

-----------------------------------------------------------------------------------------------------------------
## Exploring by keys
-----------------------------------------------------------------------------------------------------------------

you can activate this feature by uncomment : "#define USE_EXPLORATION_BY_KEYS"
in you custom config file (CustomImGuiFileDialogConfig.h in this example)

you can also uncomment the next lines for define your keys :

* IGFD_KEY_UP => Up key for explore to the top
* IGFD_KEY_DOWN => Down key for explore to the bottom
* IGFD_KEY_ENTER => Enter key for open directory
* IGFD_KEY_BACKSPACE => BackSpace for comming back to the last directory

you can also explore a file list by use the current key char.

as you see the current item is flashed (by default for 1 sec)
you can define the flashing life time by yourself with the function

Example code :
ImGuiFileDialog::Instance()->SetFlashingAttenuationInSeconds(1.0f);

-----------------------------------------------------------------------------------------------------------------
## Bookmarks
-----------------------------------------------------------------------------------------------------------------

you can create/edit/call path bookmarks and load/save them in file

you can activate it by uncomment : "#define USE_BOOKMARK"

in you custom config file (CustomImGuiFileDialogConfig.h in this example)

you can also uncomment the next lines for customize it :
Example code :
#define bookmarkPaneWith 150.0f => width of the bookmark pane
#define IMGUI_TOGGLE_BUTTON ToggleButton => customize the Toggled button (button stamp must be : (const char* label, bool *toggle)
#define bookmarksButtonString "Bookmark" => the text in the toggle button
#define bookmarksButtonHelpString "Bookmark" => the helper text when mouse over the button
#define addBookmarkButtonString "+" => the button for add a bookmark
#define removeBookmarkButtonString "-" => the button for remove the selected bookmark


* you can select each bookmark for edit the displayed name corresponding to a path
* you must double click on the label for apply the bookmark

you can also serialize/deserialize bookmarks by ex for load/save from/to file : (check the app sample by ex)
Example code :
Load => ImGuiFileDialog::Instance()->DeserializeBookmarks(bookmarString);
Save => std::string bookmarkString = ImGuiFileDialog::Instance()->SerializeBookmarks();

-----------------------------------------------------------------------------------------------------------------
## Path Edition :
-----------------------------------------------------------------------------------------------------------------

if you click right on one of any path button, you can input or modify the path pointed by this button.
then press the validate key (Enter by default with GLFW) for validate the new path
or press the escape key (Escape by default with GLFW) for quit the input path edition

see in this gif doc/inputPathEdition.gif :
1) button edition with mouse button right and escape key for quit the edition
2) focus the input and press validation for set path

-----------------------------------------------------------------------------------------------------------------
## Confirm to OverWrite Dialog :
-----------------------------------------------------------------------------------------------------------------

If you want avoid OverWrite your files after confirmation,
you can show a Dialog for confirm or cancel the OverWrite operation.

You just need to define the flag ImGuiFileDialogFlags_ConfirmOverwrite
in your call to OpenDialog/OpenModal

By default this flag is not set, since there is no pre-defined way to
define if a dialog will be for Open or Save behavior. (and its wanted :) )

Example code For Standard Dialog :
Example code :
ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey",
	ICON_IGFD_SAVE " Choose a File", filters,
	".", "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);

Example code For Modal Dialog :
Example code :
ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey",
	ICON_IGFD_SAVE " Choose a File", filters,
	".", "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);

This dialog will only verify the file in the file field.
So Not to be used with GetSelection()

The Confirm dialog will be a forced Modal Dialog, not moveable, displayed
in the center of the current FileDialog.

As usual you can customize the dialog,
in you custom config file (CustomImGuiFileDialogConfig.h in this example)

you can  uncomment the next lines for customize it :

Example code :
#define OverWriteDialogTitleString "The file Already Exist !"
#define OverWriteDialogMessageString "Would you like to OverWrite it ?"
#define OverWriteDialogConfirmButtonString "Confirm"
#define OverWriteDialogCancelButtonString "Cancel"

-----------------------------------------------------------------------------------------------------------------
## Flags :
-----------------------------------------------------------------------------------------------------------------

flag must be specified in OpenDialog or OpenModal
* ImGuiFileDialogFlags_ConfirmOverwrite 	=> show confirm to overwrite dialog
* ImGuiFileDialogFlags_DontShowHiddenFiles 	=> dont show hidden file (file starting with a .)

-----------------------------------------------------------------------------------------------------------------
## Open / Save dialog Behavior :
-----------------------------------------------------------------------------------------------------------------

There is no way to distinguish the "open dialog" behavior than "save dialog" behavior.
So you msut adapt the return according to your need :

if you want open file(s) or directory(s), you must use : GetSelection() method. you will obtain a std::map<FileName, FilePathName> of the selection
if you want create a file, you must use : GetFilePathName()/GetCurrentFileName()

the return method's and comments :

Example code :
std::map<std::string, std::string> GetSelection(); // Open File behavior : will return selection via a map<FileName, FilePathName>
std::string GetFilePathName();                     // Create File behavior : will always return the content of the field with current filter extention and current path
std::string GetCurrentFileName();                  // Create File behavior : will always return the content of the field with current filter extention
std::string GetCurrentPath();                      // will return current path
std::string GetCurrentFilter();                    // get selected filter
UserDatas GetUserDatas();                          // get user datas send with Open Dialog

-----------------------------------------------------------------------------------------------------------------
## Thumbnails Display
-----------------------------------------------------------------------------------------------------------------

You can now, display thumbnails of pictures.

The file resize use stb/image so the following files extentions are supported :
(.png, .bmp, .tga, .jpg, .jpeg, .gif, .psd, .pic, .ppm, .pgm)
only tested with .png, .bmp, .tga, .jpg, .jpeg and .gif by the way

Corresponding to your backend (ex : OpenGl) you need to define two callbacks :
* the first is a callback who will be called by ImGuiFileDialog for create the backend texture
* the second is a callback who will be called by ImGuiFileDialog for destroy the backend texture

After that you need to call the function who is responsible to create / destroy the textures.
this function must be called in your GPU Rendering zone for avoid destroying of used texture.
if you do that at the same place of your imgui code, some backend can crash your app, by ex with vulkan.

ex, for opengl :

Example code :
// Create thumbnails texture
ImGuiFileDialog::Instance()->SetCreateThumbnailCallback([](IGFD_Thumbnail_Info *vThumbnail_Info) -> void
{
	if (vThumbnail_Info &&
		vThumbnail_Info->isReadyToUpload &&
		vThumbnail_Info->textureFileDatas)
	{
		GLuint textureId = 0;
		glGenTextures(1, &textureId);
		vThumbnail_Info->textureID = (void*)textureId;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			(GLsizei)vThumbnail_Info->textureWidth, (GLsizei)vThumbnail_Info->textureHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, vThumbnail_Info->textureFileDatas);
		glFinish();
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] vThumbnail_Info->textureFileDatas;
		vThumbnail_Info->textureFileDatas = nullptr;

		vThumbnail_Info->isReadyToUpload = false;
		vThumbnail_Info->isReadyToDisplay = true;
	}
});

Example code :
// Destroy thumbnails texture
ImGuiFileDialog::Instance()->SetDestroyThumbnailCallback([](IGFD_Thumbnail_Info* vThumbnail_Info)
{
	if (vThumbnail_Info)
	{
		GLuint texID = (GLuint)vThumbnail_Info->textureID;
		glDeleteTextures(1, &texID);
		glFinish();
	}
});

Example code :
// GPU Rendering Zone // To call for Create/ Destroy Textures
ImGuiFileDialog::Instance()->ManageGPUThumbnails();

-----------------------------------------------------------------------------------------------------------------
## C API
-----------------------------------------------------------------------------------------------------------------

A C API is available let you include ImGuiFileDialog in your C project.
btw, ImGuiFileDialog depend of ImGui and dirent (for windows)

Sample code with cimgui :

// create ImGuiFileDialog
ImGuiFileDialog *cfileDialog = IGFD_Create();

// open dialog
if (igButton("Open File", buttonSize))
{
	IGFD_OpenDialog(cfiledialog,
		"filedlg",                              // dialog key (make it possible to have different treatment reagrding the dialog key
		"Open a File",                          // dialog title
		"c files(*.c *.h){.c,.h}",              // dialog filter syntax : simple => .h,.c,.pp, etc and collections : text1{filter0,filter1,filter2}, text2{filter0,filter1,filter2}, etc..
		".",                                    // base directory for files scan
		"",                                     // base filename
		0,                                      // a fucntion for display a right pane if you want
		0.0f,                                   // base width of the pane
		0,                                      // count selection : 0 infinite, 1 one file (default), n (n files)
		"User data !",                          // some user datas
		ImGuiFileDialogFlags_ConfirmOverwrite); // ImGuiFileDialogFlags
}

ImGuiIO* ioptr = igGetIO();
ImVec2 maxSize;
maxSize.x = ioptr->DisplaySize.x * 0.8f;
maxSize.y = ioptr->DisplaySize.y * 0.8f;
ImVec2 minSize;
minSize.x = maxSize.x * 0.25f;
minSize.y = maxSize.y * 0.25f;

// display dialog
if (IGFD_DisplayDialog(cfiledialog, "filedlg", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
{
	if (IGFD_IsOk(cfiledialog)) // result ok
	{
		char* cfilePathName = IGFD_GetFilePathName(cfiledialog);
		printf("GetFilePathName : %s\n", cfilePathName);
		char* cfilePath = IGFD_GetCurrentPath(cfiledialog);
		printf("GetCurrentPath : %s\n", cfilePath);
		char* cfilter = IGFD_GetCurrentFilter(cfiledialog);
		printf("GetCurrentFilter : %s\n", cfilter);
		// here convert from string because a string was passed as a userDatas, but it can be what you want
		void* cdatas = IGFD_GetUserDatas(cfiledialog);
		if (cdatas)
			printf("GetUserDatas : %s\n", (const char*)cdatas);
		struct IGFD_Selection csel = IGFD_GetSelection(cfiledialog); // multi selection
		printf("Selection :\n");
		for (int i = 0; i < (int)csel.count; i++)
		{
			printf("(%i) FileName %s => path %s\n", i, csel.table[i].fileName, csel.table[i].filePathName);
		}
		// action

		// destroy
		if (cfilePathName) free(cfilePathName);
		if (cfilePath) free(cfilePath);
		if (cfilter) free(cfilter);

		IGFD_Selection_DestroyContent(&csel);
	}
	IGFD_CloseDialog(cfiledialog);
}

// destroy ImGuiFileDialog
IGFD_Destroy(cfiledialog);

-----------------------------------------------------------------------------------------------------------------
## Std::filesystem (c++17) can be used instead of dirent.h
-----------------------------------------------------------------------------------------------------------------

you just need to uncomment that in the config file

#define USE_STD_FILESYSTEM

in this mode dirent is not more required

-----------------------------------------------------------------------------------------------------------------
## How to Integrate ImGuiFileDialog in your project
-----------------------------------------------------------------------------------------------------------------

### ImGuiFileDialog require :

* dirent v1.23 (only when USE_STD_FILESYSTEM is not defined) (https://github.com/tronkko/dirent/tree/v1.23) lib, only for windows. Successfully tested with version v1.23 only
* Dear ImGui (https://github.com/ocornut/imgui/tree/master) (with/without tables widgets)

### Customize ImGuiFileDialog :

You just need to write your own config file by override the file : ImGuiFileDialog/ImGuiFileDialogConfig.h
like i do here with CustomImGuiFileDialogConfig.h

After that, for let ImGuiFileDialog your own custom file,
you must define the preprocessor directive CUSTOM_IMGUIFILEDIALOG_CONFIG with the path of you custom config file.
This path must be relative to the directory where you put ImGuiFileDialog module.

-----------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------

Thats all.

You can check by example in this repo with the file CustomImGuiFileDialogConfig.h :
- this trick was used for have custom icon font instead of labels for buttons or messages titles
- you can also use your custom imgui button, the button call stamp must be same by the way :)

The Custom Icon Font (in CustomFont.cpp and CustomFont.h) was made with ImGuiFontStudio (https://github.com/aiekick/ImGuiFontStudio) i wrote for that :)
ImGuiFontStudio is using also ImGuiFileDialog.

-----------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------
*/

#ifndef IMGUIFILEDIALOG_H
#define IMGUIFILEDIALOG_H

#define IMGUIFILEDIALOG_VERSION "v0.6.2"

#ifndef CUSTOM_IMGUIFILEDIALOG_CONFIG
#include "ImGuiFileDialogConfig.h"
#else // CUSTOM_IMGUIFILEDIALOG_CONFIG
#include CUSTOM_IMGUIFILEDIALOG_CONFIG
#endif // CUSTOM_IMGUIFILEDIALOG_CONFIG

typedef int ImGuiFileDialogFlags; // -> enum ImGuiFileDialogFlags_
enum ImGuiFileDialogFlags_
{
	ImGuiFileDialogFlags_None = 0,
	ImGuiFileDialogFlags_ConfirmOverwrite = (1 << 0),							// show confirm to overwrite dialog
	ImGuiFileDialogFlags_DontShowHiddenFiles = (1 << 1),						// dont show hidden file (file starting with a .)
	ImGuiFileDialogFlags_DisableCreateDirectoryButton = (1 << 2),				// disable the create directory button
	ImGuiFileDialogFlags_HideColumnType = (1 << 3),								// hide column file type
	ImGuiFileDialogFlags_HideColumnSize = (1 << 4),								// hide column file size
	ImGuiFileDialogFlags_HideColumnDate = (1 << 5),								// hide column file date
#ifdef USE_THUMBNAILS
	ImGuiFileDialogFlags_DisableThumbnailMode = (1 << 6),						// disable the thumbnail mode
#endif
	ImGuiFileDialogFlags_Default = ImGuiFileDialogFlags_ConfirmOverwrite
};

#ifdef USE_THUMBNAILS
struct IGFD_Thumbnail_Info
{
	int isReadyToDisplay = 0;				// ready to be rendered, so texture created
	int isReadyToUpload = 0;				// ready to upload to gpu
	int isLoadingOrLoaded = 0;				// was sent to laoding or loaded
	void* textureID = 0;					// 2d texture id (void* is like ImtextureID type) (GL, DX, VK, Etc..)
 	unsigned char* textureFileDatas = 0;	// file texture datas, will be rested to null after gpu upload
	int textureWidth = 0;					// width of the texture to upload
	int textureHeight = 0;					// height of the texture to upload
	int textureChannels = 0;				// count channels of the texture to upload
	void* userDatas = 0;					// user datas
};
#endif // USE_THUMBNAILS

#ifdef __cplusplus

#include <imgui.h>

#include <cfloat>
#include <utility>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <mutex>

namespace IGFD
{
#ifndef MAX_FILE_DIALOG_NAME_BUFFER 
#define MAX_FILE_DIALOG_NAME_BUFFER 1024
#endif // MAX_FILE_DIALOG_NAME_BUFFER

#ifndef MAX_PATH_BUFFER_SIZE
#define MAX_PATH_BUFFER_SIZE 1024
#endif // MAX_PATH_BUFFER_SIZE

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FileDialogInternal;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class SearchManager
	{
	public:
		std::string puSearchTag;
		char puSearchBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
		bool puSearchInputIsActive = false;

	public:
		void Clear();																							// clear datas
		void DrawSearchBar(FileDialogInternal& vFileDialogInternal);											// draw the search bar
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FileExtentionInfos
	{
	public:
		ImVec4 color = ImVec4(0, 0, 0, 0);
		std::string icon;

	public:
		FileExtentionInfos();
		FileExtentionInfos(const ImVec4& vColor, const std::string& vIcon = "");
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Utils
	{
	public:
		struct PathStruct
		{
			std::string path;
			std::string name;
			std::string ext;
			bool isOk = false;
		};

	public:
		static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
		static bool ReplaceString(std::string& str, const std::string& oldStr, const std::string& newStr);
		static bool IsDirectoryExist(const std::string& name);
		static bool CreateDirectoryIfNotExist(const std::string& name);
		static PathStruct ParsePathFileName(const std::string& vPathFileName);
		static void AppendToBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr);
		static void ResetBuffer(char* vBuffer);
		static void SetBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr);
#ifdef WIN32
		static bool WReplaceString(std::wstring& str, const std::wstring& oldStr, const std::wstring& newStr);
		static std::vector<std::wstring> WSplitStringToVector(const std::wstring& text, char delimiter, bool pushEmpty);
		static std::string wstring_to_string(const std::wstring& wstr);
		static std::wstring string_to_wstring(const std::string& mbstr);
#endif
		static std::vector<std::string> SplitStringToVector(const std::string& text, char delimiter, bool pushEmpty);
		static std::vector<std::string> GetDrivesList();
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FilterManager
	{
	public:
		class FilterInfosStruct
		{
		public:
			std::string filter;
			std::set<std::string> collectionfilters;

		public:
			void clear();																						// clear the datas
			bool empty() const;																					// is filter empty
			bool exist(const std::string& vFilter) const;														// is filter exist
		};

	private:
		std::vector<FilterInfosStruct> prParsedFilters;
		std::unordered_map<std::string, FileExtentionInfos> prFileExtentionInfos;
		FilterInfosStruct prSelectedFilter;

	public:
		std::string puDLGFilters;
		std::string puDLGdefaultExt;

	public:
		void ParseFilters(const char* vFilters);																// Parse filter syntax, detect and parse filter collection
		void SetSelectedFilterWithExt(const std::string& vFilter);												// Select filter
		void SetExtentionInfos(const std::string& vFilter, const FileExtentionInfos& vInfos);					// link filter to ExtentionInfos
		void SetExtentionInfos(const std::string& vFilter, const ImVec4& vColor, const std::string& vIcon);		// link filter to Color and Icon
		bool GetExtentionInfos(const std::string& vFilter, ImVec4* vOutColor, std::string* vOutIcon);			// get Color and Icon for Filter
		void ClearExtentionInfos();																				// clear prFileExtentionInfos
		bool IsCoveredByFilters(const std::string& vTag) const;													// check if current file extention (vTag) is covered by current filter
		bool DrawFilterComboBox(FileDialogInternal& vFileDialogInternal);										// draw the filter combobox
		FilterInfosStruct GetSelectedFilter();																	// get the current selected filter
		std::string ReplaceExtentionWithCurrentFilter(const std::string vFile) const;									// replace the extention of the current file by the selected filter
		void SetDefaultFilterIfNotDefined();																	// define the first filter if no filter is selected
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FileInfos
	{
	public:
		char fileType = ' ';					// dirent fileType (f:file, d:directory, l:link)				
		std::string filePath;					// path of the file
		std::string fileName;					// filename of the file
		std::string fileName_optimized;			// optimized for search => insensitivecase
		std::string fileExt;					// extention of the file
		size_t fileSize = 0;					// for sorting operations
		std::string formatedFileSize;			// file size formated (10 o, 10 ko, 10 mo, 10 go)
		std::string fileModifDate;				// file user defined format of the date (data + time by default)
#ifdef USE_THUMBNAILS
		IGFD_Thumbnail_Info thumbnailInfo;		// structre for the display for image file tetxure
#endif // USE_THUMBNAILS

	public:
		bool IsTagFound(const std::string& vTag) const;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FileManager
	{
	public: // types
		enum class SortingFieldEnum		// sorting for filetering of the file lsit
		{
			FIELD_NONE = 0,				// no sorting preference, result indetermined haha..
			FIELD_FILENAME,				// sorted by filename
			FIELD_TYPE,					// sorted by filetype
			FIELD_SIZE,					// sorted by filesize (formated file size)
			FIELD_DATE,					// sorted by filedate
#ifdef USE_THUMBNAILS
			FIELD_THUMBNAILS,			// sorted by thumbnails (comparaison by width then by height)
#endif // USE_THUMBNAILS
		};

	private:
		std::string prCurrentPath;											// current path (to be decomposed in prCurrentPathDecomposition
		std::vector<std::string> prCurrentPathDecomposition;				// part words
		std::vector<std::shared_ptr<FileInfos>> prFileList;					// base container
		std::vector<std::shared_ptr<FileInfos>> prFilteredFileList;			// filtered container (search, sorting, etc..)
		std::string prLastSelectedFileName;									// for shift multi selection
		std::set<std::string> prSelectedFileNames;							// the user selection of FilePathNames
		bool prCreateDirectoryMode = false;									// for create directory widget

	public:
		char puVariadicBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";			// called by prSelectableItem
		bool puInputPathActivated = false;									// show input for path edition
		bool puDrivesClicked = false;										// event when a drive button is clicked
		bool puPathClicked = false;											// event when a path button was clicked
		char puInputPathBuffer[MAX_PATH_BUFFER_SIZE] = "";					// input path buffer for imgui widget input text (displayed in palce of composer)
		char puFileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";			// file name buffer in footer for imgui widget input text
		char puDirectoryNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";		// directory name buffer in footer for imgui widget input text (when is directory mode)
		std::string puHeaderFileName;										// detail view name of column file
		std::string puHeaderFileType;										// detail view name of column type
		std::string puHeaderFileSize;										// detail view name of column size
		std::string puHeaderFileDate;										// detail view name of column date + time
#ifdef USE_THUMBNAILS
		std::string puHeaderFileThumbnails;									// detail view name of column thumbnails
		bool puSortingDirection[5] = { true, true, true, true, true };		// detail view // true => Descending, false => Ascending
#else
		bool puSortingDirection[4] = { true, true, true, true };			// detail view // true => Descending, false => Ascending
#endif
		SortingFieldEnum puSortingField = SortingFieldEnum::FIELD_FILENAME;	// detail view sorting column
		bool puShowDrives = false;											// drives are shown (only on os windows)

		std::string puDLGpath;												// base path set by user when OpenDialog/OpenModal was called
		std::string puDLGDefaultFileName;									// base default file path name set by user when OpenDialog/OpenModal was called
		size_t puDLGcountSelectionMax = 1U; // 0 for infinite				// base max selection count set by user when OpenDialog/OpenModal was called
		bool puDLGDirectoryMode = false;									// is directory mode (defiend like : puDLGDirectoryMode = (filters.empty()))

		std::string puFsRoot;

	private:
		static std::string prRoundNumber(double vvalue, int n);											// custom rounding number
		static std::string prFormatFileSize(size_t vByteSize);											// format file size field
		static std::string prOptimizeFilenameForSearchOperations(const std::string& vFileName);			// turn all text in lower case for search facilitie
		static void prCompleteFileInfos(const std::shared_ptr<FileInfos>& FileInfos);					// set time and date infos of a file (detail view mode)
		void prRemoveFileNameInSelection(const std::string& vFileName);									// selection : remove a file name
		void prAddFileNameInSelection(const std::string& vFileName, bool vSetLastSelectionFileName);	// selection : add a file name
		void AddFile(const FileDialogInternal& vFileDialogInternal, 
			const std::string& vPath, const std::string& vFileName, const char& vFileType);				// add file called by scandir

	public:
		FileManager();
		bool IsComposerEmpty();
		size_t GetComposerSize();
		bool IsFileListEmpty();
		bool IsFilteredListEmpty();
		size_t GetFullFileListSize();
		std::shared_ptr<FileInfos> GetFullFileAt(size_t vIdx);
		size_t GetFilteredListSize();
		std::shared_ptr<FileInfos> GetFilteredFileAt(size_t vIdx);
		bool IsFileNameSelected(const std::string& vFileName);
		std::string GetBack();
		void ClearComposer();
		void ClearFileLists();																			// clear file list, will destroy thumbnail textures
		void ClearAll();
		void ApplyFilteringOnFileList(const FileDialogInternal& vFileDialogInternal);
		void OpenCurrentPath(const FileDialogInternal& vFileDialogInternal);							// set the path of the dialog, will launch the directory scan for populate the file listview
		void SortFields(const FileDialogInternal& vFileDialogInternal, 
			const SortingFieldEnum& vSortingField, const bool& vCanChangeOrder);						// will sort a column
		bool GetDrives();																				// list drives on windows platform
		bool CreateDir(const std::string& vPath);														// create a directory on the file system
		void ComposeNewPath(std::vector<std::string>::iterator vIter);									// compose a path from the compose path widget
		bool SetPathOnParentDirectoryIfAny();															// compose paht on parent directory
		std::string GetCurrentPath();																	// get the current path
		void SetCurrentPath(const std::string& vCurrentPath);											// set the current path
		static bool IsFileExist(const std::string& vFile);
		void SetDefaultFileName(const std::string& vFileName);
		bool SelectDirectory(const std::shared_ptr<FileInfos>& vInfos);										// enter directory
		void SelectFileName(const FileDialogInternal& vFileDialogInternal, 
			const std::shared_ptr<FileInfos>& vInfos);															// select filename
		
		//depend of dirent.h
		void SetCurrentDir(const std::string& vPath);													// define current directory for scan
		void ScanDir(const FileDialogInternal& vFileDialogInternal, const std::string& vPath);			// scan the directory for retrieve the file list

	public:
		std::string GetResultingPath();
		std::string GetResultingFileName(FileDialogInternal& vFileDialogInternal);
		std::string GetResultingFilePathName(FileDialogInternal& vFileDialogInternal);
		std::map<std::string, std::string> GetResultingSelection();

	public:
		void DrawDirectoryCreation(const FileDialogInternal& vFileDialogInternal);						// draw directory creation widget
		void DrawPathComposer(const FileDialogInternal& vFileDialogInternal);							// draw path composer widget
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef USE_THUMBNAILS
	typedef std::function<void(IGFD_Thumbnail_Info*)> CreateThumbnailFun;	// texture 2d creation function binding
	typedef std::function<void(IGFD_Thumbnail_Info*)> DestroyThumbnailFun;	// texture 2d destroy function binding
#endif
	class ThumbnailFeature
	{
	protected:
		ThumbnailFeature();
		~ThumbnailFeature();

		void NewThumbnailFrame(FileDialogInternal& vFileDialogInternal);
		void EndThumbnailFrame(FileDialogInternal& vFileDialogInternal);
		void QuitThumbnailFrame(FileDialogInternal& vFileDialogInternal);

#ifdef USE_THUMBNAILS
	protected:
		enum class DisplayModeEnum
		{
			FILE_LIST = 0,
			THUMBNAILS_LIST,
			THUMBNAILS_GRID
		};

	private:
		uint32_t prCountFiles = 0U;
		bool prIsWorking = false;
		std::shared_ptr<std::thread> prThumbnailGenerationThread = nullptr;
		std::list<std::shared_ptr<FileInfos>> prThumbnailFileDatasToGet;	// base container
		std::mutex prThumbnailFileDatasToGetMutex;
		std::list<std::shared_ptr<FileInfos>> prThumbnailToCreate;			// base container
		std::mutex prThumbnailToCreateMutex;
		std::list<IGFD_Thumbnail_Info> prThumbnailToDestroy;				// base container
		std::mutex prThumbnailToDestroyMutex;

		CreateThumbnailFun prCreateThumbnailFun = nullptr;
		DestroyThumbnailFun prDestroyThumbnailFun = nullptr;

	protected:
		DisplayModeEnum prDisplayMode = DisplayModeEnum::FILE_LIST;

	protected:
		// will be call in cpu zone (imgui computations, will call a texture file retrieval thread)
		void prStartThumbnailFileDatasExtraction();								// start the thread who will get byte buffer from image files
		bool prStopThumbnailFileDatasExtraction();								// stop the thread who will get byte buffer from image files
		void prThreadThumbnailFileDatasExtractionFunc();						// the thread who will get byte buffer from image files
		void prDrawThumbnailGenerationProgress();								// a little progressbar who will display the texture gen status
		void prAddThumbnailToLoad(const std::shared_ptr<FileInfos>& vFileInfos);		// add texture to load in the thread
		void prAddThumbnailToCreate(const std::shared_ptr<FileInfos>& vFileInfos);
		void prAddThumbnailToDestroy(IGFD_Thumbnail_Info vIGFD_Thumbnail_Info);
		void prDrawDisplayModeToolBar();										// draw display mode toolbar (file list, thumbnails list, small thumbnails grid, big thumbnails grid)
		void prClearThumbnails(FileDialogInternal& vFileDialogInternal);

	public:
		void SetCreateThumbnailCallback(const CreateThumbnailFun& vCreateThumbnailFun);
		void SetDestroyThumbnailCallback(const DestroyThumbnailFun& vCreateThumbnailFun);
		
		// must be call in gpu zone (rendering, possibly one rendering thread)
		void ManageGPUThumbnails();	// in gpu rendering zone, whill create or destroy texture
#endif
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class BookMarkFeature
	{
	protected:
		BookMarkFeature();

#ifdef USE_BOOKMARK
	private:
		struct BookmarkStruct
		{
			std::string name;			// name of the bookmark
			
			// todo: the path could be relative, better if the app is movedn but bookmarked path can be outside of the app
			std::string path;			// absolute path of the bookmarked directory 
		};

	private:
		ImGuiListClipper prBookmarkClipper;
		std::vector<BookmarkStruct> prBookmarks;
		char prBookmarkEditBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";

	protected:
		float prBookmarkWidth = 200.0f;
		bool prBookmarkPaneShown = false;
		
	protected:
		void prDrawBookmarkButton();															// draw bookmark button
		bool prDrawBookmarkPane(FileDialogInternal& vFileDialogInternal, const ImVec2& vSize);	// draw bookmark Pane

	public:
		std::string SerializeBookmarks();							// serialize bookmarks : return bookmark buffer to save in a file
		void DeserializeBookmarks(									// deserialize bookmarks : load bookmark buffer to load in the dialog (saved from previous use with SerializeBookmarks())
			const std::string& vBookmarks);							// bookmark buffer to load
#endif // USE_BOOKMARK
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// file localization by input chat // widget flashing
	class KeyExplorerFeature
	{
	protected:
		KeyExplorerFeature();

#ifdef USE_EXPLORATION_BY_KEYS
	private:
		size_t prFlashedItem = 0;																// flash when select by char
		float prFlashAlpha = 0.0f;																// flash when select by char
		float prFlashAlphaAttenInSecs = 1.0f;													// fps display dependant
		size_t prLocateFileByInputChar_lastFileIdx = 0;
		ImWchar prLocateFileByInputChar_lastChar = 0;
		int prLocateFileByInputChar_InputQueueCharactersSize = 0;
		bool prLocateFileByInputChar_lastFound = false;

	protected:
		void prLocateByInputKey(FileDialogInternal& vFileDialogInternal);						// select a file line in listview according to char key
		bool prLocateItem_Loop(FileDialogInternal& vFileDialogInternal, ImWchar vC);			// restrat for start of list view if not found a corresponding file
		void prExploreWithkeys(FileDialogInternal& vFileDialogInternal, ImGuiID vListViewID);	// select file/directory line in listview accroding to up/down enter/backspace keys
		static bool prFlashableSelectable(																// custom flashing selectable widgets, for flash the selected line in a short time
			const char* label, bool selected = false, ImGuiSelectableFlags flags = 0,
			bool vFlashing = false, const ImVec2& size = ImVec2(0, 0));
		void prStartFlashItem(size_t vIdx);														// define than an item must be flashed
		bool prBeginFlashItem(size_t vIdx);														// start the flashing of a line in lsit view
		static void prEndFlashItem();																	// end the fleshing accrdoin to var prFlashAlphaAttenInSecs

	public:
		void SetFlashingAttenuationInSeconds(													// set the flashing time of the line in file list when use exploration keys
			float vAttenValue);																	// set the attenuation (from flashed to not flashed) in seconds
#endif // USE_EXPLORATION_BY_KEYS
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef void* UserDatas;
	typedef std::function<void(const char*, UserDatas, bool*)> PaneFun;							// side pane function binding
	class FileDialogInternal
	{
	public:
		FileManager puFileManager;
		FilterManager puFilterManager;
		SearchManager puSearchManager;

	public:
		std::string puName;
		bool puShowDialog = false;
		ImVec2 puDialogCenterPos = ImVec2(0, 0);												// center pos for display the confirm overwrite dialog
		int puLastImGuiFrameCount = 0;															// to be sure than only one dialog displayed per frame
		float puFooterHeight = 0.0f;
		bool puCanWeContinue = true;															// events
		bool puOkResultToConfirm = false;														// to confim if ok for OverWrite
		bool puIsOk = false;
		bool puFileInputIsActive = false;														// when input text for file or directory is active
		bool puFileListViewIsActive = false;													// when list view is active
		std::string puDLGkey;
		std::string puDLGtitle;
		ImGuiFileDialogFlags puDLGflags = ImGuiFileDialogFlags_None;
		UserDatas puDLGuserDatas = nullptr;
		PaneFun puDLGoptionsPane = nullptr;
		float puDLGoptionsPaneWidth = 0.0f;
		bool puDLGmodal = false;
		bool puNeedToExitDialog = false;

		bool puUseCustomLocale = false;
		int puLocaleCategory = LC_ALL;	// locale category to use
		std::string puLocaleBegin; // the locale who will be applied at start of the display dialog
		std::string puLocaleEnd; // the locale who will be applaied at end of the display dialog

	public:
		void NewFrame();			// new frame, so maybe neded to do somethings, like reset events
		void EndFrame();			// end frame, so maybe neded to do somethings fater all
		void ResetForNewDialog();	// reset what is needed to reset for the openging of a new dialog
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FileDialog : 
		public BookMarkFeature, 
		public KeyExplorerFeature, 
		public ThumbnailFeature
	{
	private:
		FileDialogInternal prFileDialogInternal;
		ImGuiListClipper prFileListClipper;

	public:
		bool puAnyWindowsHovered = false;							// not remember why haha :) todo : to check if we can remove

	public:
		static FileDialog* Instance()								// Singleton for easier accces form anywhere but only one dialog at a time
		{
			static FileDialog _instance;
			return &_instance;
		}

	public:
		FileDialog();												// ImGuiFileDialog Constructor. can be used for have many dialog at same tiem (not possible with singleton)
		virtual ~FileDialog();										// ImGuiFileDialog Destructor

		// standard dialog
		void OpenDialog(											// open simple dialog (path and fileName can be specified)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vPath,								// path
			const std::string& vFileName,							// defaut file name
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		void OpenDialog(											// open simple dialog (path and filename are obtained from filePathName)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vFilePathName,						// file path name (will be decompsoed in path and fileName)
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		// with pane
		void OpenDialog(											// open dialog with custom right pane (path and fileName can be specified)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vPath,								// path
			const std::string& vFileName,							// defaut file name
			const PaneFun& vSidePane,								// side pane
			const float& vSidePaneWidth = 250.0f,					// side pane width
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		void OpenDialog(											// open dialog with custom right pane (path and filename are obtained from filePathName)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vFilePathName,						// file path name (will be decompsoed in path and fileName)
			const PaneFun& vSidePane,								// side pane
			const float& vSidePaneWidth = 250.0f,					// side pane width
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		// modal dialog
		void OpenModal(												// open simple modal (path and fileName can be specified)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vPath,								// path
			const std::string& vFileName,							// defaut file name
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		void OpenModal(												// open simple modal (path and fielname are obtained from filePathName)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vFilePathName,						// file path name (will be decompsoed in path and fileName)
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		// with pane
		void OpenModal(												// open modal with custom right pane (path and filename are obtained from filePathName)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vPath,								// path
			const std::string& vFileName,							// defaut file name
			const PaneFun& vSidePane,								// side pane
			const float& vSidePaneWidth = 250.0f,					// side pane width
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		void OpenModal(												// open modal with custom right pane (path and fielname are obtained from filePathName)
			const std::string& vKey,								// key dialog
			const std::string& vTitle,								// title
			const char* vFilters,									// filters
			const std::string& vFilePathName,						// file path name (will be decompsoed in path and fileName)
			const PaneFun& vSidePane,								// side pane
			const float& vSidePaneWidth = 250.0f,					// side pane width
			const int& vCountSelectionMax = 1,						// count selection max
			UserDatas vUserDatas = nullptr,							// user datas (can be retrieved in pane)
			ImGuiFileDialogFlags vFlags = 0);						// ImGuiFileDialogFlags 

		// Display / Close dialog form
		bool Display(												// Display the dialog. return true if a result was obtained (Ok or not)
			const std::string& vKey,								// key dialog to display (if not the same key as defined by OpenDialog/Modal => no opening)
			ImGuiWindowFlags vFlags = ImGuiWindowFlags_NoCollapse,	// ImGuiWindowFlags
			ImVec2 vMinSize = ImVec2(0, 0),							// mininmal size contraint for the ImGuiWindow
			ImVec2 vMaxSize = ImVec2(FLT_MAX, FLT_MAX));			// maximal size contraint for the ImGuiWindow
		void Close();												// close dialog

		// queries
		bool WasOpenedThisFrame(const std::string& vKey) const;		// say if the dialog key was already opened this frame
		bool WasOpenedThisFrame() const;							// say if the dialog was already opened this frame
		bool IsOpened(const std::string& vKey) const;				// say if the key is opened
		bool IsOpened() const;										// say if the dialog is opened somewhere
		std::string GetOpenedKey() const;							// return the dialog key who is opened, return nothing if not opened

		// get result
		bool IsOk() const;											// true => Dialog Closed with Ok result / false : Dialog closed with cancel result
		std::map<std::string, std::string> GetSelection();			// Open File behavior : will return selection via a map<FileName, FilePathName>
		std::string GetFilePathName();								// Save File behavior : will always return the content of the field with current filter extention and current path
		std::string GetCurrentFileName();							// Save File behavior : will always return the content of the field with current filter extention
		std::string GetCurrentPath();								// will return current path
		std::string GetCurrentFilter();								// will return selected filter
		UserDatas GetUserDatas() const;								// will return user datas send with Open Dialog/Modal

		// extentions displaying
		void SetExtentionInfos(										// SetExtention datas for have custom display of particular file type
			const std::string& vFilter,								// extention filter to tune
			const FileExtentionInfos& vInfos);						// Filter Extention Struct who contain Color and Icon/Text for the display of the file with extention filter
		void SetExtentionInfos(										// SetExtention datas for have custom display of particular file type
			const std::string& vFilter,								// extention filter to tune
			const ImVec4& vColor,									// wanted color for the display of the file with extention filter
			const std::string& vIcon = "");							// wanted text or icon of the file with extention filter
		bool GetExtentionInfos(										// GetExtention datas. return true is extention exist
			const std::string& vFilter,								// extention filter (same as used in SetExtentionInfos)
			ImVec4* vOutColor,										// color to retrieve
			std::string* vOutIcon = 0);								// icon or text to retrieve
		void ClearExtentionInfos();									// clear extentions setttings

		void SetLocales(											// set locales to use before and after the dialog display
			const int& vLocaleCategory,								// set local category
			const std::string& vLocaleBegin,						// locale to use at begining of the dialog display
			const std::string& vLocaleEnd);							// locale to use at the end of the dialog display

	protected:
		void NewFrame();											// new frame just at begining of display
		void EndFrame();											// end frame just at end of display
		void QuitFrame();											// quit frame when qui quit the dialog

		// others
		bool prConfirm_Or_OpenOverWriteFileDialog_IfNeeded(
			bool vLastAction, ImGuiWindowFlags vFlags);				// treatment of the result, start the confirm to overwrite dialog if needed (if defined with flag)
	
	public:
		// dialog parts
		virtual void prDrawHeader();								// draw header part of the dialog (bookmark btn, dir creation, path composer, search bar)
		virtual void prDrawContent();								// draw content part of the dialog (bookmark pane, file list, side pane)
		virtual bool prDrawFooter();								// draw footer part of the dialog (file field, fitler combobox, ok/cancel btn's)

		// widgets components
		virtual void prDrawSidePane(float vHeight);					// draw side pane
		virtual bool prSelectableItem(int vidx, 
			std::shared_ptr<FileInfos> vInfos, 
			bool vSelected, const char* vFmt, ...);					// draw a custom selectable behavior item
		virtual void prDrawFileListView(ImVec2 vSize);				// draw file list view (default mode)

#ifdef USE_THUMBNAILS
		virtual void prDrawThumbnailsListView(ImVec2 vSize);		// draw file list view with small thumbnails on the same line
		virtual void prDrawThumbnailsGridView(ImVec2 vSize);		// draw a grid of small thumbnails
#endif
	};
}

typedef IGFD::UserDatas IGFDUserDatas;
typedef IGFD::PaneFun IGFDPaneFun;
typedef IGFD::FileDialog ImGuiFileDialog;
#else // __cplusplus
typedef struct ImGuiFileDialog ImGuiFileDialog;
typedef struct IGFD_Selection_Pair IGFD_Selection_Pair;
typedef struct IGFD_Selection IGFD_Selection;
#endif // __cplusplus

// C Interface

#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
#ifdef IMGUIFILEDIALOG_NO_EXPORT
#define API
#else // IMGUIFILEDIALOG_NO_EXPORT
#define API __declspec(dllexport)
#endif // IMGUIFILEDIALOG_NO_EXPORT
#else // defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define API  __attribute__((__visibility__("default")))
#else // __GNUC__
#define API
#endif // __GNUC__
#endif // defined _WIN32 || defined __CYGWIN__

#ifdef __cplusplus
#define IMGUIFILEDIALOG_API extern "C" API 
#else // __cplusplus
#define IMGUIFILEDIALOG_API
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// C API ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IGFD_Selection_Pair
{
	char* fileName;
	char* filePathName;
};

IMGUIFILEDIALOG_API IGFD_Selection_Pair IGFD_Selection_Pair_Get();										// return an initialized IGFD_Selection_Pair			
IMGUIFILEDIALOG_API void IGFD_Selection_Pair_DestroyContent(IGFD_Selection_Pair* vSelection_Pair);		// destroy the content of a IGFD_Selection_Pair

struct IGFD_Selection
{
	IGFD_Selection_Pair* table;	// 0
	size_t count;						// 0U
};

IMGUIFILEDIALOG_API IGFD_Selection IGFD_Selection_Get();											// return an initialized IGFD_Selection
IMGUIFILEDIALOG_API void IGFD_Selection_DestroyContent(IGFD_Selection* vSelection);					// destroy the content of a IGFD_Selection

// constructor / destructor
IMGUIFILEDIALOG_API ImGuiFileDialog* IGFD_Create(void);												// create the filedialog context
IMGUIFILEDIALOG_API void IGFD_Destroy(ImGuiFileDialog* vContext);									// destroy the filedialog context

typedef void (*IGFD_PaneFun)(const char*, void*, bool*);											// callback fucntion for display the pane

#ifdef USE_THUMBNAILS
typedef void (*IGFD_CreateThumbnailFun)(IGFD_Thumbnail_Info*);										// callback function for create thumbnail texture
typedef void (*IGFD_DestroyThumbnailFun)(IGFD_Thumbnail_Info*);										// callback fucntion for destroy thumbnail texture
#endif // USE_THUMBNAILS

IMGUIFILEDIALOG_API void IGFD_OpenDialog(					// open a standard dialog
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vPath,										// path
	const char* vFileName,									// defaut file name
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenDialog2(					// open a standard dialog
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vFilePathName,								// defaut file path name (path and filename witl be extracted from it)
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenPaneDialog(				// open a standard dialog with pane
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vPath,										// path
	const char* vFileName,									// defaut file name
	const IGFD_PaneFun vSidePane,							// side pane
	const float vSidePaneWidth,								// side pane base width
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenPaneDialog2(				// open a standard dialog with pane
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vFilePathName,								// defaut file name (path and filename witl be extracted from it)
	const IGFD_PaneFun vSidePane,							// side pane
	const float vSidePaneWidth,								// side pane base width
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenModal(					// open a modal dialog
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vPath,										// path
	const char* vFileName,									// defaut file name
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenModal2(					// open a modal dialog
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vFilePathName,								// defaut file name (path and filename witl be extracted from it)
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenPaneModal(				// open a modal dialog with pane
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vPath,										// path
	const char* vFileName,									// defaut file name
	const IGFD_PaneFun vSidePane,							// side pane
	const float vSidePaneWidth,								// side pane base width
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API void IGFD_OpenPaneModal2(				// open a modal dialog with pane
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog
	const char* vTitle,										// title
	const char* vFilters,									// filters/filter collections. set it to null for directory mode 
	const char* vFilePathName,								// defaut file name (path and filename witl be extracted from it)
	const IGFD_PaneFun vSidePane,							// side pane
	const float vSidePaneWidth,								// side pane base width
	const int vCountSelectionMax,							// count selection max
	void* vUserDatas,										// user datas (can be retrieved in pane)
	ImGuiFileDialogFlags vFlags);							// ImGuiFileDialogFlags 

IMGUIFILEDIALOG_API bool IGFD_DisplayDialog(				// Display the dialog
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context
	const char* vKey,										// key dialog to display (if not the same key as defined by OpenDialog/Modal => no opening)
	ImGuiWindowFlags vFlags,								// ImGuiWindowFlags
	ImVec2 vMinSize,										// mininmal size contraint for the ImGuiWindow
	ImVec2 vMaxSize);										// maximal size contraint for the ImGuiWindow

IMGUIFILEDIALOG_API void IGFD_CloseDialog(					// Close the dialog
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context			

IMGUIFILEDIALOG_API bool IGFD_IsOk(							// true => Dialog Closed with Ok result / false : Dialog closed with cancel result
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context		

IMGUIFILEDIALOG_API bool IGFD_WasKeyOpenedThisFrame(		// say if the dialog key was already opened this frame
	ImGuiFileDialog* vContext, 								// ImGuiFileDialog context		
	const char* vKey);

IMGUIFILEDIALOG_API bool IGFD_WasOpenedThisFrame(			// say if the dialog was already opened this frame
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context	

IMGUIFILEDIALOG_API bool IGFD_IsKeyOpened(					// say if the dialog key is opened
	ImGuiFileDialog* vContext, 								// ImGuiFileDialog context		
	const char* vCurrentOpenedKey);							// the dialog key

IMGUIFILEDIALOG_API bool IGFD_IsOpened(						// say if the dialog is opened somewhere	
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context		

IMGUIFILEDIALOG_API IGFD_Selection IGFD_GetSelection(		// Open File behavior : will return selection via a map<FileName, FilePathName>
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context		

IMGUIFILEDIALOG_API char* IGFD_GetFilePathName(				// Save File behavior : will always return the content of the field with current filter extention and current path
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context				

IMGUIFILEDIALOG_API char* IGFD_GetCurrentFileName(			// Save File behavior : will always return the content of the field with current filter extention
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context				

IMGUIFILEDIALOG_API char* IGFD_GetCurrentPath(				// will return current path
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context					

IMGUIFILEDIALOG_API char* IGFD_GetCurrentFilter(			// will return selected filter
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context						

IMGUIFILEDIALOG_API void* IGFD_GetUserDatas(				// will return user datas send with Open Dialog/Modal
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context											

IMGUIFILEDIALOG_API void IGFD_SetExtentionInfos(			// SetExtention datas for have custom display of particular file type
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	const char* vFilter,									// extention filter to tune
	ImVec4 vColor,											// wanted color for the display of the file with extention filter
	const char* vIconText);									// wanted text or icon of the file with extention filter (can be sued with font icon)

IMGUIFILEDIALOG_API void IGFD_SetExtentionInfos2(			// SetExtention datas for have custom display of particular file type
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	const char* vFilter,									// extention filter to tune
	float vR, float vG, float vB, float vA,					// wanted color channels RGBA for the display of the file with extention filter
	const char* vIconText);									// wanted text or icon of the file with extention filter (can be sued with font icon)

IMGUIFILEDIALOG_API bool IGFD_GetExtentionInfos(
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	const char* vFilter,									// extention filter (same as used in SetExtentionInfos)
	ImVec4* vOutColor,										// color to retrieve
	char** vOutIconText);									// icon or text to retrieve

IMGUIFILEDIALOG_API void IGFD_ClearExtentionInfos(			// clear extentions setttings
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context

IMGUIFILEDIALOG_API void SetLocales(						// set locales to use before and after display
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	const int vCategory,									// set local category
	const char* vBeginLocale,								// locale to use at begining of the dialog display
	const char* vEndLocale);								// locale to set at end of the dialog display

#ifdef USE_EXPLORATION_BY_KEYS
IMGUIFILEDIALOG_API void IGFD_SetFlashingAttenuationInSeconds(	// set the flashing time of the line in file list when use exploration keys
	ImGuiFileDialog* vContext,									// ImGuiFileDialog context 
	float vAttenValue);											// set the attenuation (from flashed to not flashed) in seconds
#endif

#ifdef USE_BOOKMARK
IMGUIFILEDIALOG_API char* IGFD_SerializeBookmarks(			// serialize bookmarks : return bookmark buffer to save in a file
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context

IMGUIFILEDIALOG_API void IGFD_DeserializeBookmarks(			// deserialize bookmarks : load bookmar buffer to load in the dialog (saved from previous use with SerializeBookmarks())
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	const char* vBookmarks);								// bookmark buffer to load 
#endif

#ifdef USE_THUMBNAILS
IMGUIFILEDIALOG_API void SetCreateThumbnailCallback(		// define the callback for create the thumbnails texture
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	IGFD_CreateThumbnailFun vCreateThumbnailFun);			// the callback for create the thumbnails texture

IMGUIFILEDIALOG_API void SetDestroyThumbnailCallback(		// define the callback for destroy the thumbnails texture
	ImGuiFileDialog* vContext,								// ImGuiFileDialog context 
	IGFD_DestroyThumbnailFun vDestroyThumbnailFun);			// the callback for destroy the thumbnails texture

IMGUIFILEDIALOG_API void ManageGPUThumbnails(				// must be call in gpu zone, possibly a thread, will call the callback for create / destroy the textures
	ImGuiFileDialog* vContext);								// ImGuiFileDialog context 
#endif // USE_THUMBNAILS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // IMGUIFILEDIALOG_H
