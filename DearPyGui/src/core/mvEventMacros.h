#pragma once

// Event Categories
//-----------------------------------------------------------------------------
#define mvEVT_CATEGORY_APP      SID("GLOBAL")
#define mvEVT_CATEGORY_ITEM     SID("APP_ITEM_EVENTS")
#define mvEVT_CATEGORY_TEXTURE  SID("TEXTURE_EVENTS")
#define mvEVT_CATEGORY_INPUT    SID("INPUT_EVENTS")
#define mvEVT_CATEGORY_VIEWPORT SID("VIEWPORT_EVENTS")
#define mvEVT_CATEGORY_VALUES   SID("VALUE_EVENTS")

// mvEVT_CATEGORY_APP Events
//-----------------------------------------------------------------------------
#define mvEVT_FRAME             SID("SPECIFIC_FRAME")
#define mvEVT_END_FRAME         SID("END_FRAME")
#define mvEVT_PRE_RENDER        SID("PRE_RENDER")
#define mvEVT_PRE_RENDER_RESET  SID("PRE_RENDER_RESET")
#define mvEVT_RENDER            SID("RENDER")
								    
// mvEVT_CATEGORY_VALUES Events		    
//-----------------------------------------------------------------------------
#define mvEVT_PY_SET_VALUE      SID("PY_SET_VALUE")
								    
// mvEVT_CATEGORY_VIEWPORT Events	    
//-----------------------------------------------------------------------------
#define mvEVT_VIEWPORT_RESIZE   SID("VIEWPORT_RESIZE")

// mvEVT_CATEGORY_INPUT Events
//-----------------------------------------------------------------------------
#define mvEVT_KEY_PRESS         SID("KEY_PRESS")
#define mvEVT_KEY_DOWN          SID("KEY_DOWN")
#define mvEVT_KEY_RELEASE       SID("KEY_RELEASE")
#define mvEVT_MOUSE_WHEEL       SID("MOUSE_WHEEL")
#define mvEVT_MOUSE_DRAG        SID("MOUSE_DRAG")
#define mvEVT_MOUSE_CLICK       SID("MOUSE_CLICK")
#define mvEVT_MOUSE_DOWN        SID("MOUSE_DOWN")
#define mvEVT_MOUSE_DBL_CLK     SID("MOUSE_DOUBLE_CLICK")
#define mvEVT_MOUSE_RELEASE     SID("MOUSE_RELEASE")
#define mvEVT_MOUSE_MOVE        SID("MOUSE_MOVE")

// mvEVT_CATEGORY_ITEM Events
//-----------------------------------------------------------------------------
#define mvEVT_ADD_ITEM          SID("ADD_ITEM")
#define mvEVT_MOVE_ITEM         SID("MOVE_ITEM")
#define mvEVT_MOVE_ITEM_UP      SID("MOVE_ITEM_UP")
#define mvEVT_MOVE_ITEM_DOWN    SID("MOVE_ITEM_DOWN")
#define mvEVT_DELETE_ITEM       SID("DELETE_ITEM")

// mvEVT_CATEGORY_TEXTURE Events
//-----------------------------------------------------------------------------
#define mvEVT_DEC_TEXTURE       SID("DECREMENT_TEXTURE")
#define mvEVT_DELETE_TEXTURE    SID("DELETE_TEXTURE")