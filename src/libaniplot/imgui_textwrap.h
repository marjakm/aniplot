/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
// Copyright (c) 2015, Elmo Trolla
// LICENSE: https://opensource.org/licenses/ISC

#ifndef __IMGUI_TEXTWRAP_H__
#define __IMGUI_TEXTWRAP_H__

//
// Makes some simple uses of imgui text rendering api a bit simpler.
// (text positioning, foreground/background colors)
//

#include <imgui.h>
//#include "imgui_impl_sdl.h"
#include "imgui_internal.h" // for custom graph renderer


class ImguiTextwrap
{
public:

	ImguiTextwrap();
	virtual ~ImguiTextwrap() {};

	void init(ImFont* font);

	// return string size in pixels
	ImVec2 size(const char* text);

	void set_fgcolor(const ImVec4& color);
	void set_bgcolor(const ImVec4& color);

	// top

	void drawtl(const char* text, float x, float y);
	void drawtr(const char* text, float x, float y);
	void drawtm(const char* text, float x, float y);

	// bottom

	void drawbl(const char* text, float x, float y);
	void drawbr(const char* text, float x, float y);
	void drawbm(const char* text, float x, float y);

	// middle

	void drawml(const char* text, float x, float y);
	void drawmr(const char* text, float x, float y);
	void drawmm(const char* text, float x, float y);

	// baseline

	void drawbll(const char* text, float x, float y);
	void drawblr(const char* text, float x, float y);
	void drawblm(const char* text, float x, float y);

	// if fgcolor or bgcolor or z are None, previous values are used.
	// assumes some kind of pixel-projection..
	// you almost never want to use this method directly.
	void draw(const char* text, float x, float y, int positioning);

	// readonly variables
	float height;
	ImVec4 fgcolor;
	ImVec4 bgcolor;
	ImFont* font;

private:

	void m_fix_pos(float x, float y, float w, int positioning, float* x_fix, float* y_fix);
};


#endif // IMGUI_TEXTWRAP_H
