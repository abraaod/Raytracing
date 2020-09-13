
#ifndef _PARAMSET_
#define _PARAMSET_

#include <map>
#include <string>
#include <iostream>

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
    //std::cout << result->second << std::endl;
    return result->second;
}

#endif