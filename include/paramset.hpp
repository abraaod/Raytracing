
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
        
    private:
        std::map<Key, Data> params;

};

#endif