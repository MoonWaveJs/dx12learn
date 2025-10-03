#pragma once
#include <iostream>
#include "FBXLoader.h"

int main()
{
    GetFbxLoader()->FnOutputScene("C:/jiangshang/dx12learn/Assets/UnityTechnologies/Basic Asset Pack Interior/Models/BedDouble.FBX");
    std::cout << "Hello World!/n";
}
