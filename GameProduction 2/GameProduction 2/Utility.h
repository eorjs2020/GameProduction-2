#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <class T>
void CleanVector(vector<T>& v, bool& b)
{
	if (!v.empty())
	{

		v.erase(remove(v.begin(), v.end(), nullptr), v.end());
		v.shrink_to_fit();
	}
	b = false;
}