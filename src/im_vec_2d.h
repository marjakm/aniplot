/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab : */
#ifndef __IM_VEC_2D_H__
#define __IM_VEC_2D_H__

static inline ImVec2 operator-(const ImVec2& v)                                 { return ImVec2(-v.x, -v.y); } // fdkz:
static inline ImVec2 operator/(const float lhs, const ImVec2& rhs)              { return ImVec2(lhs/rhs.x, lhs/rhs.y); } // fdkz:
static inline ImVec2 operator*(const float lhs, const ImVec2& rhs)              { return ImVec2(lhs*rhs.x, lhs*rhs.y); } // fdkz:
static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; } // fdkz:

struct ImVec2d
{
	double x, y;
	ImVec2d() { x = y = 0.0f; }
	ImVec2d(double _x, double _y) { x = _x; y = _y; }
	ImVec2d(const ImVec2& v) { x = v.x; y = v.y; }

	//const ImVec2 &toImVec2() { return ImVec2(x, y); };
	ImVec2 toImVec2() { return ImVec2(x, y); };
};

static inline ImVec2d operator*(const ImVec2d& lhs, const double rhs)             { return ImVec2d(lhs.x*rhs, lhs.y*rhs); }
static inline ImVec2d operator/(const ImVec2d& lhs, const double rhs)             { return ImVec2d(lhs.x/rhs, lhs.y/rhs); }
static inline ImVec2d operator+(const ImVec2d& lhs, const ImVec2d& rhs)           { return ImVec2d(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2d operator-(const ImVec2d& lhs, const ImVec2d& rhs)           { return ImVec2d(lhs.x-rhs.x, lhs.y-rhs.y); }
static inline ImVec2d operator*(const ImVec2d& lhs, const ImVec2d& rhs)           { return ImVec2d(lhs.x*rhs.x, lhs.y*rhs.y); }
static inline ImVec2d operator/(const ImVec2d& lhs, const ImVec2d& rhs)           { return ImVec2d(lhs.x/rhs.x, lhs.y/rhs.y); }
static inline ImVec2d operator-(const ImVec2d& v)                                 { return ImVec2d(-v.x, -v.y); }
static inline ImVec2d operator/(const double lhs, const ImVec2d& rhs)             { return ImVec2d(lhs/rhs.x, lhs/rhs.y); }
static inline ImVec2d operator*(const double lhs, const ImVec2d& rhs)             { return ImVec2d(lhs*rhs.x, lhs*rhs.y); }
static inline ImVec2d& operator*=(ImVec2d& lhs, const ImVec2d& rhs)               { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
static inline ImVec2d& operator+=(ImVec2d& lhs, const ImVec2d& rhs)               { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2d& operator-=(ImVec2d& lhs, const ImVec2d& rhs)               { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2d& operator*=(ImVec2d& lhs, const double rhs)                 { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2d& operator/=(ImVec2d& lhs, const double rhs)                 { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
//static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z, lhs.w-lhs.w); }
#endif
