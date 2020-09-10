
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

    std::string find(std::string search) const;
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
std::string Paramset<Key, Data>::find(std::string search) const{
    return params[search];
}

#endif