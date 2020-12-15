#pragma once
#include "mvAppItems.h"
#include "mvDrawArrowCmd.h"
#include "mvDrawBezierCurveCmd.h"
#include "mvDrawCircleCmd.h"
#include "mvDrawImageCmd.h"
#include "mvDrawLineCmd.h"
#include "mvDrawPolygonCmd.h"
#include "mvDrawPolylineCmd.h"
#include "mvDrawQuadCmd.h"
#include "mvDrawRectCmd.h"
#include "mvDrawTextCmd.h"
#include "mvDrawTriangleCmd.h"

namespace Marvel {

	void AddDrawingCommands(std::map<std::string, mvPythonParser>* parsers);
						         
}