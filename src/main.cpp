#include "../include/parser.hpp"
#include <map>
#include <string>
#include "../include/vec.hpp"
#include "../include/api.hpp"


int main(int argc, char const *argv[])
{
    if(argc != 2){
        std::cout << "Error: Insert input file.\nEX: ./main <inputfile>.xml\n(You can find some example sin scene path)\n";
        return 0;
    }

    // std::cout << "Input file: " << argv[1] << std::endl;
    std::string path = argv[1];

    Api api;
    
    Parser parser(path, api);
    parser.loadScene();
    // parser.printLoadedScene();
    api.render();
    
    std::cout << "Image generated with success.\n";

    return 0;
}