#pragma once

//-----------------------------------------------------------------------------
// mvTextureStorage
//
//     - This class acts as a manager for texture storage. The 
//       texture storage system serves 2 purposes:
//
//         * Allows some image based widgets to share the same textures.
//         * Allows some textures to be automatically cleaned up using
//           a reference counting system.
//
//     - AddTexture will increment an existing texture if it already exists
//     
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <unordered_map>
#include "mvEvents.h"
#include "mvPythonParser.h"
#include "cpp.hint"
#include "mvApp.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTexture
	//-----------------------------------------------------------------------------
	struct mvTexture
	{
		int   width = 0;
		int   height = 0;
		void* texture = nullptr;
		int   count = 0;
	};

	enum class mvTextureFormat
	{
		RGBA_INT = 0u,
		RGBA_FLOAT,
		RGB_FLOAT,
		RGB_INT,
		BGRA_INT,
		BGRA_FLOAT,
		BGR_FLOAT,
		BGR_INT
	};

	//-----------------------------------------------------------------------------
	// mvTextureStorage
	//-----------------------------------------------------------------------------
	class mvTextureStorage : public mvEventHandler
	{

		struct CompileTimeTexture
		{
			std::string name;
			std::vector<float> data;
			unsigned width;
			unsigned height;
			mvTextureFormat format;
		};

	public:

		static void InsertConstants(std::vector<std::pair<std::string, long>>& constants);

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_CREATE_EXTRA_COMMAND(add_texture);
		MV_CREATE_EXTRA_COMMAND(decrement_texture);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(add_texture);
			MV_ADD_EXTRA_COMMAND(decrement_texture);
		MV_END_EXTRA_COMMANDS

	public:

		mvTextureStorage();
		~mvTextureStorage();

		bool onEvent     (mvEvent& event) override;
		bool onFirstFrame(mvEvent& event);
		bool onDecrement (mvEvent& event);

		void       addTexture       (const std::string& name);
		void       addTexture       (const std::string& name, float* data, unsigned width, unsigned height, mvTextureFormat format);
		void       addDelayedTexture(const std::string& name);
		void       addDelayedTexture(const std::string& name, const std::vector<float>& data, unsigned width, unsigned height, mvTextureFormat format);
		void       incrementTexture (const std::string& name);
		void       decrementTexture (const std::string& name);
		mvTexture* getTexture       (const std::string& name);
		unsigned   getTextureCount  ();
		void       deleteAllTextures();

		// new debugger window
		void show_debugger();
		void refreshAtlas();
		void scheduleRefresh() { m_dirty = true; }
		bool isValid() const { return !m_dirty; }

	private:
		
		std::unordered_map<std::string, mvTexture> m_textures;
		std::vector<CompileTimeTexture>            m_delayedTextures;
		bool                                       m_dirty = false;

	};

}