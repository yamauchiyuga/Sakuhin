#include "Amplitude.h"

const int SkeketonSlash{ 4 };
const int WitchSpitFire{ 5 };
const int WitchThunder{ 10 };
const int DragonBite{ 7 };
const int DragonTail{ 8 };
const int DragonSpitFire{ 9 };
const int DragonFlySpitFire{ 10 };

int Amplitude::amplitude_damage(Actor& other) const
{
	const float Rand = gsRandf(0.7f, 1.3f);
	if (other.name() == "SkeketonSlash") return Rand * SkeketonSlash;
	if(other.name()=="WitchSpitFire") return Rand * WitchSpitFire;
	if(other.name()=="WitchThunder")return Rand * WitchThunder;
	if(other.name()=="DragonBite")return Rand * DragonBite;
	if(other.name()=="DragonTail")return Rand * DragonTail;
	if(other.name()=="DragonSpitFire")return Rand * DragonSpitFire;
	if (other.name() == "DragonFlySpitFire")return Rand * DragonFlySpitFire;
	return 0;
}