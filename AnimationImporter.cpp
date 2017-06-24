//#include "AnimationImporter.h"
//
//AnimationImporter::AnimationImporter(char* filename)
//{
//	FbxManager* sdkManager = NULL;
//	FbxScene* scene = NULL;
//	bool lResult;
//
//	// Prepare the FBX SDK.
//	InitializeSdkObjects(sdkManager, scene);
//	// Load the scene.
//	
//	// The example can take a FBX file as an argument.
//	FbxString lFilePath(filename);//"D:\\Animations\\B3010@Kopanie-E19-R02-T01.fbx");
//	if (lFilePath.IsEmpty())
//	{
//		lResult = false;
//		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
//	}
//	else
//	{
//		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
//		lResult = LoadScene(sdkManager, scene, lFilePath.Buffer());
//	}
//
//	if (lResult == false)
//	{
//		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
//	}
//	else
//	{
//		for (int i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); i++)
//		{
//			FbxAnimStack* lAnimStack = scene->GetSrcObject<FbxAnimStack>(i);
//
//			FbxString lOutputString = "Animation Stack Name: ";
//			lOutputString += lAnimStack->GetName();
//			lOutputString += "\n\n";
//			FBXSDK_printf(lOutputString);
//
//			int nbAnimLayers = lAnimStack->GetMemberCount<FbxAnimLayer>();
//
//			FbxNode* root = scene->GetRootNode(); //root, dla ka¿dego dziecka rekurencyjnie sprawdzamy dan¹ warstwê
//			int childCount = root->GetChildCount();
//
//			for (size_t j = 0; j < nbAnimLayers; j++)
//			{
//				FbxAnimLayer* layer = lAnimStack->GetMember<FbxAnimLayer>(j);
//
//				for (size_t k = 0; k < childCount; k++)
//				{
//					FbxNode* child = root->GetChild(k);
//					//krzywe rotacjii dla danego dziecka (na razie za p³ytko, trzeba to wywo³ac dla wszystkich dzieci itd.))
//					FbxAnimCurve* rotX = child->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
//					FbxAnimCurve* rotY = child->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
//					FbxAnimCurve* rotZ = child->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z); //emilia blady  
//
//					//rotY->KeyIncValue(0, 1);
//
//					
//				}
//				
//			}
//		}
//	}
//}
//
//
//AnimationImporter::~AnimationImporter()
//{
//}
