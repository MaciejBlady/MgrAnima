#include "fbxsdk.h"

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
#include <string>
#include <math.h>
#include <algorithm>
#include "interpolationPoints.h"

#include "AnimationWarper.h"

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
void ProcessChannel(FbxAnimLayer* pAnimLayer, FbxNode* pNode);
void ProcessCurve(FbxAnimCurve* curve);
void ProcessAnimationStack(FbxAnimStack* pAnimStack, FbxNode* pNode);

void SaveCurveToFile(FbxAnimCurve& curve, std::string filename)
{
	std::ofstream myfile;
	myfile.open(filename);

	for (size_t i = 0; i < curve.KeyGetCount(); i++)
	{
		std::string keyData = "";
		keyData.append(std::to_string(curve.KeyGetTime(i).GetSecondDouble()));
		keyData.append(",");
		keyData.append(std::to_string(curve.EvaluateIndex(i)));
		keyData.append("\n");
		myfile << keyData;
	}
	myfile.close();
}

double randomDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

float LinearInterpolation(float y0, float t0, float y1, float t1, float t)
{
	return y0 + (t - t0)*((y1 - y0) / (t1 - t0));
}

float lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}

int InsertKey(FbxAnimCurve* curve, double seconds, double value)
{
	FbxTime time;
	time.SetSecondDouble(seconds);
	curve->KeyModifyBegin();

	int newKeyIndex = curve->KeyAdd(time);
	curve->KeySet(newKeyIndex, time, value, FbxAnimCurveDef::eInterpolationCubic);

	curve->KeyModifyEnd();

	return newKeyIndex;
}

double CubicInterpolate(double y0, double y1,double y2, double y3, double t)
{
	double a0, a1, a2, a3, t2;

	t2 = t*t;
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;

	return(a0*t*t2 + a1*t2 + a2*t + a3);
}

int fileIndex = 0;
std::vector<int> offsetSpline;
float splineKeyDistance = 0;

float Scale(float time)
{
	//bez skalowania - najlepiej
	return 1.0f;
}


float _offset = 30.0f;
float Offset(float time)
{
	//niech ta funkcja przyjmuje wartoœæi od 0, do porz¹danej wartoœci w danym punkcie (np. 3 dla t=70), a potem do 0 
	return _offset;
}


double cubicSpecific(double x)
{
	return (24.0f *x - 4.8f*x*x) / 15.0f;
}

double x2(double t)
{
	return t*t + 1;
}

void ProcessCurve(FbxAnimCurve* curve, int keyBegin, int keyEnd)
{
	if (curve != NULL)
	{
		//SaveCurveToFile(*curve, std::to_string(i) + "before.csv");
		
		//FbxTime beginTime = curve->KeyGetTime(keyBegin);
		//FbxTime endTime = curve->KeyGetTime(keyEnd);
		//FbxTime halfTime;
		//double beginTimeSec = beginTime.GetSecondDouble();
		//double endTimeSec = endTime.GetSecondDouble();

		//double halfTimeSeconds = beginTime.GetSecondDouble() + (endTime.GetSecondDouble() - beginTime.GetSecondDouble()) / 2.0d;
		//double halfTimeSeconds = beginTimeSec + (endTimeSec - beginTimeSec) / 2.0f;
		//halfTime.SetSecondDouble(halfTimeSeconds);

		//float beginValue = curve->EvaluateIndex(keyBegin);
		//float endValue = curve->EvaluateIndex(keyEnd);

		//float calculatedScale = scale(halfTime.GetSecondDouble());
		//float calculatedOffset = offset(halfTime.GetSecondDouble());

		//float halfValue = randomDouble(beginValue, endValue) *calculatedScale + calculatedOffset;

		//niekoniecznie taka wartoœæ (jaka?)
		//float halfTimeOffset = halfValue - curve->Evaluate(halfTime);
		//int newKeyIndex = InsertKey(curve, halfTime.GetSecondDouble(), halfValue);

		InterpolationPoints points;
		points.GeneratePoints(1, curve);

		curve->KeyModifyBegin();
		int interval = keyEnd - keyBegin;

		for (size_t i = keyBegin; i < keyEnd; i++)
		{
			double intervalPercent = (double)(i - keyBegin) / (double)interval;
			double offset = points.GetSplineValue(intervalPercent);
			curve->KeyIncValue(i, offset);
		}


		//for (size_t i = keyBegin; i < newKeyIndex; i++)
		//{
		//	double t = curve->KeyGetTime(i).GetSecondDouble();
		//	float interpolatedOffset = LinearInterpolation(0.0f, beginTime.GetSecondDouble(), halfTimeOffset, halfTime.GetSecondDouble(), t);
		//	//cubicSpecific(t);//
		//	//curve->KeyMultValue(i, x2(t - beginTimeSec));
		//	//curve->KeyIncValue(i, interpolatedOffset);
		//	curve->KeyIncValue(i, x2(t - beginTimeSec));
		//}


		//for (size_t i = newKeyIndex + 1; i < keyEnd; i++)
		//{
		//	double t = curve->KeyGetTime(i).GetSecondDouble();
		//	float interpolatedOffset = LinearInterpolation(halfTimeOffset, halfTime.GetSecondDouble(), 0.0f, endTime.GetSecondDouble(), t);
		//	cubicSpecific(t);//
		//	//curve->KeyMultValue(i, x2(endTimeSec -t));
		//	//curve->KeyIncValue(i, interpolatedOffset);
		//	curve->KeyIncValue(i, x2(endTimeSec -t));
		//}

		curve->KeyModifyEnd();

		//SaveCurveToFile(*curve, std::to_string(i) + "after.csv");
		//i++;
	}
}

std::vector<std::string> namesToProcess;
int _begin = 0;
int _end = 0;
std::string _mode;
int recursionLevel = 0;
int _modifiedIntervalsCount = 0;
void ProcessChannel(FbxAnimLayer* pAnimLayer, FbxNode* pNode)
{
	const char * name = pNode->GetName();
	std::string str = name;

	for (size_t i = 0; i < namesToProcess.size(); i++)
	{
		if (str.find(namesToProcess[i]) != std::string::npos && str.find("Toe") == std::string::npos)
		{
			FbxAnimCurve* rotX = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			FbxAnimCurve* rotY = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			FbxAnimCurve* rotZ = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z); //emilia blady  

			FbxAnimCurve* chosen;
			if (_mode == "x")
			{
				if (rotX->KeyGetCount() > 2)
				{
					chosen = rotX;
				}
				else
				{
					_mode = "y";
					recursionLevel++;
					ProcessChannel(pAnimLayer, pNode);
					return;
				}
				
			}
			else if (_mode == "y")
			{
				if (rotY->KeyGetCount() > 2)
				{
					chosen = rotY;
				}
				else
				{
					_mode = "z";
					recursionLevel++;
					ProcessChannel(pAnimLayer, pNode);
					return;
				}
			}
			else
			{
				if (rotZ->KeyGetCount() > 2)
				{
					chosen = rotY;
				}
				else if (recursionLevel < 2)
				{
					_mode = "x";
					recursionLevel++;
					ProcessChannel(pAnimLayer, pNode);					
					return;
				}
				else
				{
					recursionLevel = 0;
					return;
				}
			}
			recursionLevel = 0;


			int keyCount = chosen->KeyGetCount();
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(0, (int)(keyCount*0.8));

			SaveCurveToFile(*chosen, std::to_string(fileIndex) + "before.csv");

			for (size_t i = 0; i < _modifiedIntervalsCount; i++)
			{
				int intervalMiddle = uni(rng);
				_begin = intervalMiddle - (int)(0.1f*keyCount);
				_begin = std::max(0, _begin);

				_end = intervalMiddle + (int)(0.1f*keyCount);
				_end = std::min(keyCount - 1, _end);

				if (_end < _begin)
				{
					int temp = _end;
					_end = _begin;
					_begin = temp;
				}

				ProcessCurve(chosen, _begin, _end);
			}		

			SaveCurveToFile(*chosen, std::to_string(fileIndex) + "after.csv");
			fileIndex++;
		}
	}
}

void ProcessAnimation(FbxScene* pScene)
{
	int i;
	for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);
		FbxString lOutputString = "Animation Stack Name: ";

		//ProcessAnimationStack(lAnimStack, pScene->GetRootNode());

		lOutputString += lAnimStack->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);
		DisplayAnimation(lAnimStack, pScene->GetRootNode(), true);
		DisplayAnimation(lAnimStack, pScene->GetRootNode());
	}
}

void ProcessAnimationStack(FbxAnimStack* pAnimStack, FbxNode* pNode)
{
	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	for (int l = 0; l < nbAnimLayers; l++)
	{
		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);
		ProcessChannel(lAnimLayer, pNode);
	}
}

void ProcessAnimationLayer(FbxAnimLayer* pAnimLayer, FbxNode* pNode)
{
	FbxString lOutputString;
	lOutputString = "Node name: ";
	lOutputString += (pNode->GetName());
	lOutputString += "\n";
	FBXSDK_printf(lOutputString);

	ProcessChannel(pAnimLayer, pNode);
	for (int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount));
	}
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
	int l;
	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	FbxString lOutputString;
	//lOutputString = "Animation stack contains ";
	//lOutputString += nbAnimLayers;
	//lOutputString += " Animation Layer(s)\n";
	//FBXSDK_printf(lOutputString);
	for (l = 0; l < nbAnimLayers; l++)
	{
		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);
		//lOutputString = "AnimLayer ";
		//lOutputString += l;
		//lOutputString += "\n";
		//FBXSDK_printf(lOutputString);
		DisplayAnimation(lAnimLayer, pNode, isSwitcher);
	}
}

void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
	int lModelCount;

	FbxString lOutputString;
	lOutputString = "     Node Name: ";
	lOutputString += pNode->GetName();
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	ProcessChannel(pAnimLayer, pNode);
	//FBXSDK_printf("\n");
	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
	}
}

void Process(FbxScene* lScene, std::string tag,  std::string mode)
{	
	namesToProcess.clear();
	namesToProcess.push_back(tag);
	_mode = mode;
	ProcessAnimation(lScene);
}

std::string bodyParts[] = { 
	"Head",
	"Neck",
	"Spine",
	"Shoulder",

	"RightArm",
	"RightForeArm",
	//"RightHand",
	"RightUpLeg",
	"RightLeg",
	"RightFoot",

	"LeftShoulder",
	"LeftArm",
	"LeftForeArm",
	//"LeftHand",
	"LeftUpLeg",
	"LeftLeg",
	"LeftFoot",
};

int main(int argc, char** argv)
{
	srand(time(NULL));	
	std::vector<std::string> bodyPartsVec(bodyParts, bodyParts + sizeof(bodyParts) / sizeof(bodyParts[0]));
	std::vector<std::string> partsToProcess;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, bodyPartsVec.size()-1);

	_modifiedIntervalsCount = 3;
	int partsToModifyCount = 3;

	while (partsToProcess.size() < partsToModifyCount)
	{
		int randIndex = uni(rng);
		std::string selectedPart = bodyPartsVec[randIndex];

		if (std::find(partsToProcess.begin(), partsToProcess.end(), selectedPart) == partsToProcess.end())
		{
			partsToProcess.push_back(selectedPart);
		}
	}

	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;
	InitializeSdkObjects(lSdkManager, lScene);

	//FbxString lFilePath("D:\\Animations\\B3010@Kopanie-E19-R02-T01.fbx");
	//FbxString lFilePath("D:\\Animations\\female_walk.fbx");
	FbxString lFilePath("D:\\Animations\\martelo_3.fbx");
	
	lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());

	for (size_t i = 0; i < partsToProcess.size(); i++)
	{
		std::string mode;
		if (partsToProcess[i].find("Leg") != std::string::npos)
		{
			mode = "x";
		}
		else 
		{
			int randomMode = rand() % 3;
			switch (randomMode)
			{
			case 0:
				mode = "x";
				break;
			case 1:
				mode = "y";
				break;
			case 2:
				mode = "z";
				break;
			default:
				mode = "x";
				break;
			}
		}
		
		Process(lScene, partsToProcess[i], mode);
	}
	lResult = SaveScene(lSdkManager, lScene, "transformed.fbx");


	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}
	else
	{
		return 0;
	}
}


