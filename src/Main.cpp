#include <iostream>
#include "Engine/Engine.h"
#include <sol/sol.hpp>


int main(int argc, char* argv[]) {
    
    Engine engine;

    engine.Init();
    engine.Run();
    engine.Destroy();

    return 0;
}
