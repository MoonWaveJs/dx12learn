#pragma once
#include <iostream>

namespace Dx12RenderLearn
{
    class DxLog
    {
    public:
        static void Log(std::string msg)
        {
#ifdef _DEBUG
            std::cout<< msg << '\n';
#endif
        }    
    };

}
