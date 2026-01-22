#include "mvFontManager.h"
#include "mvFontItems.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <assert.h>
#include <array>
#include "mvContext.h"
#include "mvAppItem.h"
#include "mvCore.h"
#include "mvItemRegistry.h"
#include "mvViewport.h"
#include "mvPyUtils.h"
#include <frameobject.h>
#include "mvTextureItems.h"
#include <CustomFont.cpp>
#include <CustomFont.h>

#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void
HelpMarker(const char* desc)
{
ImGui::TextDisabled("(?)");
if (ImGui::IsItemHovered())
{
	if(ImGui::BeginTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
}

// [Internal] Display details for a single font, called by ShowStyleEditor().
void
mvFontManager::drawFontNode(ImFont* font)
{
	ImGuiContext& g = *GImGui;
	ImGuiMetricsConfig* cfg = &g.DebugMetricsConfig;
	ImFontAtlas* atlas = font->OwnerAtlas;
	bool opened = ImGui::TreeNode(font, "Font: \"%s\": %d sources(s)", font->GetDebugName(), font->Sources.Size);

	// Display preview text
	if (!opened)
		ImGui::Indent();
	ImGui::Indent();
	if (cfg->ShowFontPreview)
	{
		ImGui::PushFont(font, 0.0f);
		ImGui::Text("The quick brown fox jumps over the lazy dog");
		ImGui::PopFont();
	}
	if (!opened)
	{
		ImGui::Unindent();
		ImGui::Unindent();
		return;
	}
	if (ImGui::SmallButton("Set as default"))
	{
		_updateDefault = false;
		_defaultFont.reset();
		ImGui::GetIO().FontDefault = font;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear bakes"))
		ImFontAtlasFontDiscardBakes(atlas, font, 0);
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear unused"))
		ImFontAtlasFontDiscardBakes(atlas, font, 2);

	// Display details
	char c_str[5];
	ImTextCharToUtf8(c_str, font->FallbackChar);
	ImGui::Text("Fallback character: '%s' (U+%04X)", c_str, font->FallbackChar);
	ImTextCharToUtf8(c_str, font->EllipsisChar);
	ImGui::Text("Ellipsis character: '%s' (U+%04X)", c_str, font->EllipsisChar);

	for (int src_n = 0; src_n < font->Sources.Size; src_n++)
	{
		ImFontConfig* src = font->Sources[src_n];
		const ImFontLoader* loader = src->FontLoader ? src->FontLoader : atlas->FontLoader;
		if (ImGui::TreeNodeEx(src, ImGuiTreeNodeFlags_Leaf|ImGuiTreeNodeFlags_Bullet,
			"Input %d: \'%s\' [%d], Loader: '%s',\nOversample: %d,%d, PixelSnapH: %d, Offset: (%.1f,%.1f)",
			src_n, src->Name, src->FontNo, loader->Name ? loader->Name : "N/A",
			src->OversampleH, src->OversampleV, src->PixelSnapH, src->GlyphOffset.x, src->GlyphOffset.y))
		{
			ImGui::TreePop();
		}
	}

	// TODO: add Glyphs Overlap Detection tool in future

	// Display all glyphs of the fonts in separate pages of 256 characters
	for (int baked_n = 0; baked_n < atlas->Builder->BakedPool.Size; baked_n++)
	{
		ImFontBaked* baked = &atlas->Builder->BakedPool[baked_n];
		if (baked->OwnerFont != font)
			continue;
		ImGui::PushID(baked_n);
		if (ImGui::TreeNode("Glyphs", "Baked at { %.2fpx, d.%.2f }: %d glyphs%s", baked->Size, baked->RasterizerDensity, baked->Glyphs.Size, (baked->LastUsedFrame < atlas->Builder->FrameCount - 1) ? " *Unused*" : ""))
		{
			if (ImGui::SmallButton("Load all"))
				for (unsigned int base = 0; base <= IM_UNICODE_CODEPOINT_MAX; base++)
					baked->FindGlyph((ImWchar)base);

			const int surface_sqrt = (int)ImSqrt((float)baked->MetricsTotalSurface);
			ImGui::Text("Ascent: %f, Descent: %f, Ascent-Descent: %f", baked->Ascent, baked->Descent, baked->Ascent - baked->Descent);
			ImGui::Text("Texture Area: about %d px ~%dx%d px", baked->MetricsTotalSurface, surface_sqrt, surface_sqrt);
			for (int src_n = 0; src_n < font->Sources.Size; src_n++)
			{
				ImFontConfig* src = font->Sources[src_n];
				int oversample_h, oversample_v;
				ImFontAtlasBuildGetOversampleFactors(src, baked, &oversample_h, &oversample_v);
				ImGui::BulletText("Input %d: \'%s\', Oversample: (%d=>%d,%d=>%d), PixelSnapH: %d, Offset: (%.1f,%.1f)",
					src_n, src->Name, src->OversampleH, oversample_h, src->OversampleV, oversample_v, src->PixelSnapH, src->GlyphOffset.x, src->GlyphOffset.y);
			}

			ImGui::DebugNodeFontGlyphesForSrcMask(font, baked, ~0);
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	ImGui::TreePop();
	ImGui::Unindent();
}

// Demo helper function to select among loaded fonts.
// Here we use the regular BeginCombo()/EndCombo() api which is more the more flexible one.
void
mvFontManager::drawFontSelector(const char* label)
{
ImGuiIO& io = ImGui::GetIO();
ImFont* font_current = ImGui::GetFont();
if (ImGui::BeginCombo(label, font_current->GetDebugName()))
{
	for (int n = 0; n < io.Fonts->Fonts.Size; n++)
	{
		ImFont* font = io.Fonts->Fonts[n];
		ImGui::PushID((void*)font);
		if (ImGui::Selectable(font->GetDebugName(), font == font_current))
		{
			_updateDefault = false;
			_defaultFont.reset();
			io.FontDefault = font;
		}
		ImGui::PopID();
	}
	ImGui::EndCombo();
}
}

void 
mvFontManager::updateAtlas()
{
	ImGuiIO& io = ImGui::GetIO();

	// See if the built-in ProggyClean font is already there.  We need it just in case
	// all the DPG fonts (mvFont) are deleted and ImGui needs to render a frame.
    if (io.Fonts->Sources.Size == 0)
        io.Fonts->AddFontDefault();

	for (auto& root : GContext->itemRegistry->fontRegistryRoots)
	{
		// Tell the font registry to update any fonts that need it
		root->customAction(nullptr);
	}

	// Reset the current font so that NewFrame can pick up changes immediately
	if (_updateDefault)
	{
		ImGuiContext& g = *GImGui;
		auto item = _defaultFont.lock();
		IM_ASSERT(item->type == mvAppItemType::mvFont && "The default font must be a mvFont.");
		if (item)
		{
			auto font = static_cast<mvFont*>(item.get());
			io.FontDefault = font->getFontPtr();
			g.Style.FontSizeBase = font->getSize();
		}
		else
		{
			io.FontDefault = nullptr;
			g.Style.FontSizeBase = 0;
		}
	}
}

void 
mvFontManager::drawWidgets()
{

	drawFontSelector("Fonts##Selector");

	ImGuiIO& io = ImGui::GetIO();
	ImFontAtlas* atlas = io.Fonts;
	ImGui::PushItemWidth(120);
	for (int i = 0; i < atlas->Fonts.Size; i++)
	{
		ImFont* font = atlas->Fonts[i];
		ImGui::PushID(font);
		drawFontNode(font);
		ImGui::PopID();
	}
	if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexData->Width, atlas->TexData->Height))
	{
		if (ImGui::Button("Compact"))
			atlas->CompactCache();
		ImGui::SameLine();
		if (ImGui::Button("Grow"))
			ImFontAtlasTextureGrow(atlas);
		ImGui::SameLine();
		if (ImGui::Button("Clear All"))
			ImFontAtlasBuildClear(atlas);
		ImGui::SetItemTooltip("Destroy cache and custom rectangles.");

		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
		ImGui::PushStyleVar(ImGuiStyleVar_ImageBorderSize, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, border_col);
		ImGui::ImageWithBg(atlas->TexRef, ImVec2((float)atlas->TexData->Width, (float)atlas->TexData->Height), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tint_col);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::TreePop();
	}

	// Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
	// (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
	// As of ImGui 1.92, things start to change.  There's still a global scale factor and we expose it,
	// but DPI handling is different.
	const float MIN_SCALE = 0.3f;
	const float MAX_SCALE = 2.0f;
	if (ImGui::DragFloat("global scale", &getGlobalFontScale(), 0.005f, MIN_SCALE, MAX_SCALE, "%.2f")) // Scale everything
		getGlobalFontScale() = IM_MAX(getGlobalFontScale(), MIN_SCALE);
	ImGui::PopItemWidth();

}

void 
mvFontManager::setGlobalFontScale(float scale)
{
	_globalFontScale = scale;
}