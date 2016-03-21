/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CFACTORY_H
#define CFACTORY_H

#include<map>
#include<string>
#include<vector>
#include<exception>
using namespace std;

template<typename Key, typename Type>
class CFactory
{
protected:
    map<Key,Type *> _map;
public:
    ~CFactory()
    {
        typename map<Key,Type *>::iterator it;
        for ( it=_map.begin() ; it != _map.end(); it++ )
            delete (*it).second;
    }
    virtual void Register(Key key, Type * productcreator)
    {
        _map[key]=productcreator;
    }
    virtual void Unregister(Key key)
    {
        _map.erase (key);
    }
    virtual Type * createObject(Key key)throw(exception)
    {
        if(_map.find(key)!=_map.end())
            return _map[key]->clone();
        else
            throw(exception());
    }
    vector<string> listKey(){
        typename map<Key,Type *>::iterator it;
        vector<string> v;
        for ( it=_map.begin() ; it != _map.end(); it++ )
        {
            v.push_back((*it).first);
        }
        return v;
    }
    map<Key,Type *> &Map(){
        return _map;
    }

};
#endif // CFACTORY_H
