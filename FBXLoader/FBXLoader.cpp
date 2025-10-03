// FBXLoader.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"
#include "FBXLoader.h"

#pragma region output helper


/* Tab character ("\t") counter */
int numTabs = 0;

/**
 * Print the required number of tabs.
 */
void PrintTabs() {
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}

/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch (type) {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}

/**
 * Print an attribute.
 */
void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if (!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

/**
 * Print a node, its attributes, and all its children recursively.
 */
void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
    );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}
#pragma endregion

static std::shared_ptr<FbxLoader> Instance;


FbxLoader::FbxLoader()
{
    lSdkManager = FbxManager::Create();
    lImporter = FbxImporter::Create(lSdkManager, "");
}

FbxLoader::~FbxLoader()
{
    lImporter->Destroy();
    lSdkManager->Destroy();
    Instance.reset();
    printf("FbxLoader Destroy./n");
}

void FbxLoader::FnFbxLoaderInit()
{
}

void FbxLoader::FnOutputScene(std::string fbx)
{
    FbxNode* rootNode;
    FbxScene* scene = FbxScene::Create(lSdkManager, "");

    if (LoadFbxScene(fbx, rootNode, scene))
    {
        if (rootNode) {
            for (int i = 0; i < rootNode->GetChildCount(); i++)
                PrintNode(rootNode->GetChild(i));
        }
        rootNode->Destroy();
        scene->Destroy();
    }
}

bool IsMeshNode(FbxNode* pNode) 
{
    auto count = pNode->GetNodeAttributeCount();
    for (int i = 0; i < count; i++)
    {
        auto attri = pNode->GetNodeAttributeByIndex(i);
        if (attri && attri->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            return true;
        }
	}
}

bool FbxLoader::LoadFbxScene(std::string fbx, FbxNode*& outRootNode, FbxScene* scene)
{
    bool lImportStatus = lImporter->Initialize(fbx.c_str(), -1);
    if (!lImportStatus)
    {
        printf("Call to FbxImporter::Initialize() failed./n");
        printf("Error returned: %s/n/n", lImporter->GetStatus().GetErrorString());
		return false;
    }
    lImporter->Import(scene);
    outRootNode = scene->GetRootNode();
	return true;
}

std::map<std::string, VertexData> GetVertexGroup(FbxNode* rootNode)
{
    for (int i = 0; i < rootNode->GetChildCount(); i++)
    {
        if (IsMeshNode(rootNode->GetChild(i)))
        {
            printf("Find Mesh Node: %s \n", rootNode->GetChild(i)->GetName());
        }
        GetVertexGroup(rootNode->GetChild(i));
    }
}

std::map<std::string, VertexData> FbxLoader::FnGetVertexGroup(std::string fbx)
{
    FbxNode* rootNode;
    FbxScene* scene = FbxScene::Create(lSdkManager, "");
    if (LoadFbxScene(fbx, rootNode, scene)) 
    {
        if (rootNode) 
        {
            GetVertexGroup(rootNode);
        }
        rootNode->Destroy();
        scene->Destroy();
    }
	return std::map<std::string, VertexData>();
}


void FbxLoader::Release()
{
    Instance.reset();
}


std::shared_ptr<FbxLoader> GetFbxLoader()
{
    if (!Instance)
    {
        Instance = std::make_shared<FbxLoader>();
    }
    return Instance;
}

