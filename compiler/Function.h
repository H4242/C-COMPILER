#pragma once

#include <vector>
#include <string>

using namespace std;

class Function
{
public:
    bool operator!=(const Function &f) const
    {
        return name != f.name && returnType != f.returnType && paramsTypes != f.paramsTypes && paramsCount != f.paramsCount;
    }

    string name = "";
    string returnType = "";
    vector<string> paramsTypes = {};
    int paramsCount = 0;
};