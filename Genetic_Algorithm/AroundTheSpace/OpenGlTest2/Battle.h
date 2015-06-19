#pragma once
#include <vector>

using namespace std;

class Battle
{
public:
	int _chess;

	vector<GLfloat> s;
	vector<GLfloat> s2;
	vector<GLfloat> s3;
	vector<GLfloat> s4;
	vector<GLfloat> s5;

	Battle(string _str, int chess);
	~Battle();
};

