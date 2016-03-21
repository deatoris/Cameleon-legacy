#ifndef DATATABLE_H
#define DATATABLE_H
#include "table.h"
#include<CDataByFile.h>

using namespace std;
#include <vector>

class DataTable :public CDataByFile<Table>
{
public:
    DataTable();
    static string KEY;
    virtual DataTable * clone();
    void setDataByFile(shared_ptr<Table > type);
    void setDataByCopy(shared_ptr<Table > type);
    shared_ptr<Table > getDataByFile();
};

#endif // DATATABLE_H
