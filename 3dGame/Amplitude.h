#ifndef AMPLITUDE_H_
#define AMPLITUDE_H_

#include"Actor/Actor.h"

class Amplitude
{
public:
	Amplitude() = default;
	~Amplitude() = default;
	int  amplitude_damage(Actor& other)const;
};

#endif // !AMPLITUDE_H_



