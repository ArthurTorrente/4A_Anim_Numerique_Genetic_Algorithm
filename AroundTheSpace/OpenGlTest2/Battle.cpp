#include "stdafx.h"
/*#include "Battle.h"

#include <iostream>     // cout, endl
#include <fstream>      // fstream

#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;




Battle::Battle(string _str, int chess) : _chess(chess)
{
	typedef tokenizer< escaped_list_separator<char> > Tokenizer3;
	vector< string > vec3;
	string line3;

	string data(_str);

	ifstream in(data.c_str());
	if (!in.is_open())
	{

	}

	while (getline(in, line3))
	{
		Tokenizer3 tok(line3);
		vec3.assign(tok.begin(), tok.end());
		int x = ::atof(vec3.at(0).c_str());
		GLfloat x1 = x;
		int y = ::atof(vec3.at(1).c_str());
		GLfloat y1 = y;
		int z = ::atof(vec3.at(2).c_str());
		GLfloat z1 = z;
		int t = ::atof(vec3.at(3).c_str());
		GLfloat t1 = t;
		int u = ::atof(vec3.at(4).c_str());
		GLfloat u1 = u;


		s.push_back(x1);
		s2.push_back(y1);
		s3.push_back(z1);
		s4.push_back(t1);
		s5.push_back(u1);
	}
}


Battle::~Battle()
{
}*/