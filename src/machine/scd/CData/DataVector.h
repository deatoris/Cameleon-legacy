#ifndef DATAVECTOR_H
#define DATAVECTOR_H
#include<CDataByFile.h>
using namespace std;
#include <vector>
ostream& operator << (ostream& out, const vector<shared_ptr<CData> > & v);
istream& operator >> (istream& in, vector<shared_ptr<CData> >& v);

class DataVector : public CDataByFile<vector<shared_ptr<CData> >  >
{
public:
    typedef shared_ptr<vector<shared_ptr<CData> > > DATAVECTOR;
    DataVector();
    static string KEY;
    virtual DataVector * clone();
    void setDataByFile(shared_ptr<vector<shared_ptr<CData> > > type);
    void setDataByCopy(shared_ptr<vector<shared_ptr<CData> > > type);
    shared_ptr<vector<shared_ptr<CData> > > getDataByFile();
};
#endif // DATAVECTOR_H
