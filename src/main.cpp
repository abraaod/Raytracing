#include "../include/parser.hpp"
#include <map>
#include <string>
#include "../include/vec.hpp"


int main(int argc, char const *argv[])
{
    // if(argc != 2){
    //     std::cout << "Ops, informe o arquivo de entrada.\nEX: ./main <inputfile>.xml\n";
    //     return 0;
    // }

    // std::cout << "Input file: " << argv[1] << std::endl;
    // std::string path = argv[1];

    std::string path = "./scene/scene.xml";

    Parser parser(path);
    parser.loadScene();
    parser.printLoadedScene();
    
    //parser.print();
    return 0;
}