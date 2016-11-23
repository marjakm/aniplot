/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
#include "imgui_textwrap.h"

ImguiTextwrap::ImguiTextwrap()
{
	this->font = NULL;
	this->height = 0.;
	this->fgcolor = ImVec4(1., 1., 1., 1.);
	this->bgcolor = ImVec4(1., 1., 1., 0.);
}

// --------------------------------------------------------------------------
// ---- METHODS -------------------------------------------------------------
// --------------------------------------------------------------------------

void ImguiTextwrap::init(ImFont* font) {
	IM_ASSERT(font);
	this->font = font;
	this->height = font->FontSize;
}

// Calculate text size. Text can be multi-line.
ImVec2 ImguiTextwrap::size(const char* text)
{
	IM_ASSERT(this->font);
	ImVec2 text_size = this->font->CalcTextSizeA(this->font->FontSize, FLT_MAX, 0, text, NULL, NULL);
	//if (text_size.x > 0.0f)
	//    text_size.x -= character_spacing_x;
	return text_size;
}


void ImguiTextwrap::set_fgcolor(const ImVec4& color)
{
	fgcolor = color;
}


void ImguiTextwrap::set_bgcolor(const ImVec4& color)
{
	bgcolor = color;
}

// top

void ImguiTextwrap::drawtl(const char* text, float x, float y) { draw(text, x, y, 1+16); }
void ImguiTextwrap::drawtr(const char* text, float x, float y) { draw(text, x, y, 2+16); }
void ImguiTextwrap::drawtm(const char* text, float x, float y) { draw(text, x, y, 4+16); }

// bottom

void ImguiTextwrap::drawbl(const char* text, float x, float y) { draw(text, x, y, 1+32); }
void ImguiTextwrap::drawbr(const char* text, float x, float y) { draw(text, x, y, 2+32); }
void ImguiTextwrap::drawbm(const char* text, float x, float y) { draw(text, x, y, 4+32); }

// middle

void ImguiTextwrap::drawml(const char* text, float x, float y) { draw(text, x, y, 1+64); }
void ImguiTextwrap::drawmr(const char* text, float x, float y) { draw(text, x, y, 2+64); }
void ImguiTextwrap::drawmm(const char* text, float x, float y) { draw(text, x, y, 4+64); }

// baseline

void ImguiTextwrap::drawbll(const char* text, float x, float y) { draw(text, x, y, 1+8); }
void ImguiTextwrap::drawblr(const char* text, float x, float y) { draw(text, x, y, 3+8); }
void ImguiTextwrap::drawblm(const char* text, float x, float y) { draw(text, x, y, 4+8); }


void ImguiTextwrap::draw(const char* text, float x, float y, int positioning)
{
	IM_ASSERT(this->font);
	ImVec2 str_size = this->size(text);
	// position the text according to given flags. top-left, bottom-right, ..
	m_fix_pos(x, y, str_size.x, positioning, &x, &y);

	if (str_size.x > 0) {
		x = floor(x);
		y = floor(y);
		// draw the background
		if (bgcolor.w != 0) {
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + str_size.x + 1, y + this->font->FontSize), ImColor(bgcolor));
		}
		// draw the text
		ImGui::GetWindowDrawList()->AddText(this->font, this->font->FontSize, ImVec2(x, y), ImColor(fgcolor), text, NULL, 0, NULL);
	}
}


// --------------------------------------------------------------------------
// ---- PRIVATE -------------------------------------------------------------
// --------------------------------------------------------------------------


void ImguiTextwrap::m_fix_pos(float x, float y, float w, int positioning, float* x_fix, float* y_fix)
{
	IM_ASSERT(this->font);
	// positioning
	// first 3 bits - left-right-middle
	// next  4 bits - baseline-top-bottom-middle

	if (positioning) {
		if      (positioning & 1) {}            // left
		else if (positioning & 2) x -= w;       // right
		else if (positioning & 4) x -= w / 2.f; // middle

		if      (positioning & 8)  y -= this->font->Ascent;  // baseline
		else if (positioning & 16) {}                        // top
		else if (positioning & 32) y -= this->height;        // bottom
		else if (positioning & 64) y -= this->height / 2.f;  // middle
		//else if (positioning & 64) y += this->height / 2.f - this->font->Descent; // middle
	}

	//*x_fix = floor(x + 0.5f); // round(x); // round missing in visual studio express 2008
	//*y_fix = floor(y + 0.5f); // round(y);
	//*x_fix = round(x);
	//*y_fix = round(y);
	*x_fix = x;
	*y_fix = y;
}


