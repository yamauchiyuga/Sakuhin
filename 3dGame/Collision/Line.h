#ifndef LINE_H_
#define LINE_H_

#include<gslib.h>

//
class Line {
public:
	//
	Line() = default;
	//
	Line(const GSvector3& start, const GSvector3& end) :
		start{ start }, end{ end }{};

public:
	//
	GSvector3 start{ 0.0f,0.0f,0.0f };
	//
	GSvector3 end{ 0.0f,0.0f,0.0f };
};

#endif // !LINE_H_
