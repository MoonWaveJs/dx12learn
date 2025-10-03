#pragma once
#include <iostream>
#include "FBXLoader.h"
#include <fbxsdk.h>

int main()
{
    GetFbxLoader()->FnOutputScene("E:/Project/dx12-learn/Assets/UnityTechnologies/Basic Asset Pack Interior/Models/BedDouble.FBX");
    std::cout << "Hello World!/n";
}
