/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CTREE_H
#define CTREE_H

#include<vector>
using namespace std;
template<typename Data>
class Node
{
private:
    Data * _data;
    vector<Node * > _v_node;
public:
    Node(Data* data)
        :_data(data)
    {
    }
    ~Node()
    {
        for(int i =0;i<(int)_v_node.size();i++){
            delete _v_node[i];
        }
    }
    int addNode(Node * node){
        _v_node.push_back(node);
        return (int)_v_node.size()-1;
    }

    Data * data()
    {
        return _data;
    }

    const Data * data()
    const
    {
        return _data;
    }

    Node * clone()
    {
        Node * n = new Node(_data);
        for(int i =0;i<(int)_v_node.size();i++){
            _v_node.push_back(_v_node[i]->clone());
        }
    }
    int getNbrChild()
    {
        return (int)_v_node.size();
    }
     int getNbrChild()
    const
    {
        return (int)_v_node.size();
    }

    Node *  operator [](int index)
    {
        return _v_node[index];
    }
    const Node *  operator [](int index)
    const
    {
        return _v_node[index];
    }
    void print(ostream &out,int depth){
        for(int i =0;i<depth;i++)
            out<<"\t";
        out<< *(this->_data)<<endl;


        for(int i =0;i<(int)_v_node.size();i++)
            (_v_node[i])->print(out,depth+1);
    }
};



template<typename Data>
void addBranchMerge(const vector<Data> & branch, Node<Data> & node)
{
    bool compare = true;

    Node<Data> * currentnode = &node;
    for(int i = 0;i<(int)branch.size();i++)
    {
        //search node
        int indexhit=-1;
        if(compare==true)
        {
            for(int j = 0;j<(*currentnode).getNbrChild();j++)
            {
                if(branch[i]==*((*currentnode)[j]->data()))
                {
                    indexhit = j;
                }
            }
            if(indexhit==-1)
                compare = false;
        }
        if(compare==true){
            currentnode = ((*currentnode)[indexhit]);
        }
        else{
            Data * d = new Data(branch[i]);
            Node<Data> * nodeadd = new Node<Data>(d);
            currentnode->addNode(nodeadd);
            currentnode =  nodeadd;
        }

    }
}
#endif // CTREE_H
