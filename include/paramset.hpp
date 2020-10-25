
#ifndef _PARAMSET_
#define _PARAMSET_

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <typeinfo>

template <typename Key, typename Data>
class Paramset
{
private:
    /* data */
private:
    std::map<Key, Data> params;

public:
    Paramset(/* args */);
    ~Paramset();

    void add(const Key &_key, const Data &_data);

    void print();

    Data find(const Key _key) const;

    void clear();
};

template <typename Key, typename Data>
Paramset<Key, Data>::Paramset(/* args */)
{
}

template <typename Key, typename Data>
Paramset<Key, Data>::~Paramset() = default;

template<typename Key, typename Data>
void Paramset<Key, Data>::add(const Key &_key, const Data &_data)
{
    params[_key] = _data;
}

template<typename Key, typename Data>
void Paramset<Key, Data>::print()
{
    for (auto it = params.begin(); it != params.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

template<typename Key, typename Data>
Data Paramset<Key, Data>::find(const Key _key) const{
    auto result = params.find(_key);
    if(result == params.end()){
        return "";
    }

    if(_key == "bl" ||_key == "tl" ||_key == "tr" ||_key == "br" || _key == "color"){
        if(result->second.find('.') == std::string::npos){
            std::vector<std::string> result_; 
            std::string valor = result->second;

            std::istringstream iss(valor); 
            std::vector<std::string> splited((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());
            float v1;
            std::istringstream(splited[0]) >> v1;
            float v2;
            std::istringstream(splited[1]) >> v2;
            float v3;
            std::istringstream(splited[2]) >> v3;
            
            std::string valor2 = std::to_string(v1/255.0) + " " + std::to_string(v2/255.0) + " " + std::to_string(v3/255.0);
            
            return valor2;

        }
    }
    //std::cout << result->second << std::endl;
    return result->second;
}

template<typename Key, typename Data>
void Paramset<Key, Data>::clear(){
    params.clear();
}

#endif