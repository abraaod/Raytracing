
#ifndef _PARAMSET_
#define _PARAMSET_

#include <map>
#include <string>
#include <iostream>

template<typename Key, typename Data>
class ParamSet{
    public:
        ParamSet(){
        }
        
        void add(const Key &_key, const Data &_data){
            params[_key] = _data;
        }
        
        void print(){
            for(auto it = params.begin(); it != params.end(); it++){
                std::cout << it->first << " " << it->second << std::endl;
            }
        }

    private:
        std::map<Key, Data> params;

};

#endif