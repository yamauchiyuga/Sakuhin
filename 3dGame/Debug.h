#pragma once
#include<string>
#include<vector>
#include<gslib.h>
using namespace std;

class Debug
{
public:
	Debug() = default;

	~Debug() {
		contents.clear();
	}

	void get_pos(const GSvector3& pos) {
		string temp = "x:" + to_string(pos.x) + "y:" + to_string(pos.y) + "z:" + to_string(pos.z);
		contents.push_back(temp);
	}

	static	void draw() {
		for (int i = 0; const auto & e:contents) {
			gsTextPos(pos.x, pos.y + verlical_width * i);
			gsDrawText(e.c_str());
		}
	}

	static	void clear() {
		contents.clear();
	}
private:

	static vector<string> contents;
	static const GSvector2 pos; 
	static const float verlical_width;
};

