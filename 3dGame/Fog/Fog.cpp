#include "Fog.h"
#include <GSgame.h>
#include <imgui/imgui.h>   

Fog::Fog() :
	fog_color_{ 0.1800f, 0.180f, 0.275, 1.000f },
	fog_start_{ 20.0f },
	fog_end_{ 80.0f }
{
}

void Fog::draw() const
{
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_START, fog_start_);
	glFogi(GL_FOG_END, fog_end_);
	glFogfv(GL_FOG_COLOR, fog_color_);
}
