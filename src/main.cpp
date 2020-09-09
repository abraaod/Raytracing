#include "../include/parser.hpp"

int main(int argc, char const *argv[])
{
    std::string path = "./scene/scene.xml";

    Parser parser(path);
    parser.loadScene();
    
    //parser.print();
    return 0;
}