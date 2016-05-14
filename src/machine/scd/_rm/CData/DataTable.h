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
//    void setDataByFile(Table * type);
       Table *  cloneData(Table * type);
//    Table * getDataByFile();
};

#endif // DATATABLE_H
