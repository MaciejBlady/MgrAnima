//#include "AnimationWarper.h"
//
//
//AnimationWarper::AnimationWarper()
//{
//}
//
//
//AnimationWarper::~AnimationWarper()
//{
//}
//
//int i = 0;
//std::vector<int> offsetSpline;
//float splineKeyDistance = 0;
//
//float Scale(float time)
//{
//	//bez skalowania - najlepiej
//	return 1.0f;
//}
//
//float Offset(float time)
//{
//	//niech ta funkcja przyjmuje wartoœæi od 0, do porz¹danej wartoœci w danym punkcie (np. 3 dla t=70), a potem do 0 
//	return 30.0f;
//}
//
//
//double cubicSpecific(double x)
//{
//	return 24.0f *x - 4.8f*x*x;
//}
//
//
//void ProcessCurve(FbxAnimCurve* curve, float(*scale)(float), float(*offset)(float), int keyBegin, int keyEnd)
//{
//	if (curve != NULL)
//	{
//		SaveCurveToFile(*curve, std::to_string(i) + "before.csv");
//
//		FbxTime beginTime = curve->KeyGetTime(keyBegin);
//		FbxTime endTime = curve->KeyGetTime(keyEnd);
//		FbxTime halfTime;// = curve->KeyGetTime((int)((keyBegin + keyEnd) / 2.0f));
//
//		double beginTimeSec = beginTime.GetSecondDouble();
//		double endTimeSec = endTime.GetSecondDouble();
//
//		//double halfTimeSeconds = beginTime.GetSecondDouble() + (endTime.GetSecondDouble() - beginTime.GetSecondDouble()) / 2.0d;
//		double halfTimeSeconds = beginTimeSec + (endTimeSec - beginTimeSec) / 2.0f;
//		halfTime.SetSecondDouble(halfTimeSeconds);
//
//		float beginValue = curve->EvaluateIndex(keyBegin);
//		float endValue = curve->EvaluateIndex(keyEnd);
//
//		float calculatedScale = scale(halfTime.GetSecondDouble());
//		float calculatedOffset = offset(halfTime.GetSecondDouble());
//
//		float halfValue = randomDouble(beginValue, endValue) *calculatedScale + calculatedOffset;
//
//		//niekoniecznie taka wartoœæ (jaka?)
//		float halfTimeOffset = halfValue - curve->Evaluate(halfTime);
//		int newKeyIndex = InsertKey(curve, halfTime.GetSecondDouble(), halfValue);
//
//		curve->KeyModifyBegin();
//		for (size_t i = keyBegin; i < newKeyIndex; i++)
//		{
//			double t = curve->KeyGetTime(i).GetSecondDouble();
//			float interpolatedOffset = LinearInterpolation(0.0f, beginTime.GetSecondDouble(), halfTimeOffset, halfTime.GetSecondDouble(), t);
//			//cubicSpecific(t);//
//			//curve->KeyMultValue(i, 0.4f);
//			curve->KeyIncValue(i, interpolatedOffset);
//		}
//
//
//		for (size_t i = newKeyIndex + 1; i < keyEnd; i++)
//		{
//			double t = curve->KeyGetTime(i).GetSecondDouble();
//			float interpolatedOffset = LinearInterpolation(halfTimeOffset, halfTime.GetSecondDouble(), 0.0f, endTime.GetSecondDouble(), t);
//			cubicSpecific(t);//
//			//curve->KeyMultValue(i, 0.4f);
//			curve->KeyIncValue(i, interpolatedOffset);
//		}
//		curve->KeyModifyEnd();
//
//		SaveCurveToFile(*curve, std::to_string(i) + "after.csv");
//		i++;
//	}
//}
//
//void ProcessChannel(FbxAnimLayer* pAnimLayer, FbxNode* pNode)
//{
//	const char * name = pNode->GetName();
//	std::string str = name;
//	if (str.find("LeftLeg") != std::string::npos || str.find("LeftUpLeg") != std::string::npos)
//	{
//		FbxAnimCurve* rotX = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
//		FbxAnimCurve* rotY = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
//		FbxAnimCurve* rotZ = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z); //emilia blady  
//
//		ProcessCurve(rotZ, Scale, Offset, 100, 300);
//	}
//}
//
//float LinearInterpolation(float y0, float t0, float y1, float t1, float t)
//{
//	return y0 + (t - t0)*((y1 - y0) / (t1 - t0));
//}
//
//float lerp(float v0, float v1, float t)
//{
//	return (1 - t) * v0 + t * v1;
//}
//
//int InsertKey(FbxAnimCurve* curve, double seconds, double value)
//{
//	FbxTime time;
//	time.SetSecondDouble(seconds);
//	curve->KeyModifyBegin();
//
//	int newKeyIndex = curve->KeyAdd(time);
//	curve->KeySet(newKeyIndex, time, value, FbxAnimCurveDef::eInterpolationCubic);
//
//	curve->KeyModifyEnd();
//
//	return newKeyIndex;
//}
//
//void DisplayAnimation(FbxScene* pScene)
//{
//	int i;
//	for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
//	{
//		FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);
//		FbxString lOutputString = "Animation Stack Name: ";
//		//lOutputString += lAnimStack->GetName();
//		//lOutputString += "\n\n";
//		//FBXSDK_printf(lOutputString);
//		DisplayAnimation(lAnimStack, pScene->GetRootNode(), true);
//		DisplayAnimation(lAnimStack, pScene->GetRootNode());
//	}
//}
//void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
//{
//	int l;
//	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
//	FbxString lOutputString;
//	//lOutputString = "Animation stack contains ";
//	//lOutputString += nbAnimLayers;
//	//lOutputString += " Animation Layer(s)\n";
//	//FBXSDK_printf(lOutputString);
//	for (l = 0; l < nbAnimLayers; l++)
//	{
//		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);
//		//lOutputString = "AnimLayer ";
//		//lOutputString += l;
//		//lOutputString += "\n";
//		//FBXSDK_printf(lOutputString);
//		DisplayAnimation(lAnimLayer, pNode, isSwitcher);
//	}
//}
//void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
//{
//	int lModelCount;
//
//	FbxString lOutputString;
//	lOutputString = "     Node Name: ";
//	lOutputString += pNode->GetName();
//	lOutputString += "\n\n";
//	FBXSDK_printf(lOutputString);
//
//	ProcessChannel(pAnimLayer, pNode);
//	//FBXSDK_printf("\n");
//	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
//	{
//		DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
//	}
//}
//
//void SaveCurveToFile(FbxAnimCurve& curve, std::string filename)
//{
//	std::ofstream myfile;
//	myfile.open(filename);
//
//	for (size_t i = 0; i < curve.KeyGetCount(); i++)
//	{
//		std::string keyData = "";
//		keyData.append(std::to_string(curve.KeyGetTime(i).GetSecondDouble()));
//		keyData.append(",");
//		keyData.append(std::to_string(curve.EvaluateIndex(i)));
//		keyData.append("\n");
//		myfile << keyData;
//	}
//	myfile.close();
//}
//
//double randomDouble(double fMin, double fMax)
//{
//	double f = (double)rand() / RAND_MAX;
//	return fMin + f * (fMax - fMin);
//}
