/*
 * main.cpp
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */


#include <iostream>
#include "Element.h"
#include "Expression.h"
#include "Interpreter.h"
#include <vector>
using namespace std;
int main()
{
	vector<int> v={1,2,3,4,5};
	vector<int> f(v.begin(),v.begin()+2);
	vector<int> n(v.begin()+2,v.end());
	cout<<f[f.size()-1]<<endl;
	cout<<n[0]<<endl;
	return 0;
}

