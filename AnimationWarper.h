#pragma once

#include "fbxsdk.h"
#include <string>
#include <math.h>

#include "Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"

#include <iostream>
#include <fstream>
#include <vector>


class AnimationWarper
{
public:
	AnimationWarper();
	~AnimationWarper();
	void SaveCurveToFile(FbxAnimCurve& curve, std::string filename);
	double RandomDouble(double fMin, double fMax);
	float LinearInterpolation(float y0, float t0, float y1, float t1, float t);
	float Lerp(float v0, float v1, float t);
	int InsertKey(FbxAnimCurve* curve, double seconds, double value);

	void DisplayAnimation(FbxScene* pScene);
	void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher);
	void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher);

	void ProcessCurve(FbxAnimCurve* curve, float(*scale)(float), float(*offset)(float), int keyBegin, int keyEnd);
	void ProcessChannel(FbxAnimLayer* pAnimLayer, FbxNode* pNode);

};

