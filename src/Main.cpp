#include <iostream>
#include "Engine/Engine.h"
#include <sol/sol.hpp>

void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/test.lua");
    
    int someVariable = lua["some_variable"];
    // Getting the value from the lua script.
    std::cout << "The value of someVariable is " << someVariable << " (C++)" << std::endl;

    // This is how we can get the values from inside a lua table.
    bool isFullscreen = lua["config"]["fullscreen"];

    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];

    std::cout << "The value of isFullscreen is " << isFullscreen << " (C++)" << std::endl;
}

int main(int argc, char* argv[]) {
    
    /*Engine engine;

    engine.Init();
    engine.Run();
    engine.Destroy();*/

    TestLua();

    return 0;
  

}
