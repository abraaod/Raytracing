#include "../include/parser.hpp"
#include <map>
#include <string>

int main(int argc, char const *argv[])
{
    std::string path = "./scene/scene.xml";

    Parser parser(path);
    parser.loadScene();
    parser.printLoadedScene();
    
    //parser.print();
    return 0;
}