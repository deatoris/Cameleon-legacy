#ifndef DATAMAP_H
#define DATAMAP_H
#include<CDataByFile.h>
using namespace std;
#include <map>
 ostream& operator << (ostream& out, const map<string,shared_ptr<CData>  >  & m);
 istream& operator >> (istream& in, map<string,shared_ptr<CData>  > & m);

class DataMap : public CDataByFile<map<string,shared_ptr<CData>  >  >
{
public:
    typedef shared_ptr<map<string,shared_ptr<CData>  > > DATAMAP;
    DataMap();
    static string KEY;
    virtual DataMap * clone();
    void setDataByFile(shared_ptr<map<string,shared_ptr<CData>  > > type);
    void setDataByCopy(shared_ptr<map<string,shared_ptr<CData>  > > type);
    shared_ptr<map<string,shared_ptr<CData>  > > getDataByFile();
};

#endif // DATAMAP_H
