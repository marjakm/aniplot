/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
#ifndef __IMVEC2D_PORTALRECT_H__
#define __IMVEC2D_PORTALRECT_H__


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




// A rectangle with some helper functions to enable coordinatesystem-like behaviour.
// (min, max) is always mapped to (0, 1) of parent space.
struct PortalRect {
	ImVec2d min;
	ImVec2d max;

	PortalRect()                                          : min(0.,0.), max(1.,1.)   {}
	PortalRect(const ImVec2d& _min, const ImVec2d& _max)  : min(_min), max(_max)     {}
	PortalRect(double x1, double y1, double x2, double y2): min(x1, y1), max(x2, y2) {}
	PortalRect(const ImRect& r) : min(r.Min), max(r.Max) {}

	ImVec2d center()                  const { return ImVec2d((min.x+max.x)*0.5f, (min.y+max.y)*0.5f); }
	ImVec2d size()                    const { return ImVec2d(max.x-min.x, max.y-min.y); }
	double  w()                       const { return max.x-min.x; }
	double  h()                       const { return max.y-min.y; }
	bool   contains(const ImVec2d& p) const { return p.x >= min.x  &&  p.y >= min.y  &&  p.x < max.x  &&  p.y < max.y; }
	//ImRect get_imrect()               const { return ImRect(min.x, min.y, max.x, max.y); } // too unprecise

	void translate(ImVec2d delta) { min += delta; max += delta; }

	// ratio 1: no change. ratio 1.3: 100 -> 130. ratio 1/1.3: 130 -> 100. ratio 2: 100 -> 200, ratio 0.5: 200 -> 100
	void resize_by_ratio(const ImVec2d& _center, const ImVec2d& ratio) { min = _center + (min - _center) * ratio; max = _center + (max - _center) * ratio; }
	void clip(const PortalRect& _clip) { if (min.x < _clip.min.x) min.x = _clip.min.x; if (min.y < _clip.min.y) min.y = _clip.min.y; if (max.x > _clip.max.x) max.x = _clip.max.x; if (max.y > _clip.max.y) max.y = _clip.max.y; }

	PortalRect proj_in(const PortalRect& cs)   const { return PortalRect(proj_vin(cs.min), proj_vin(cs.max)); }
	PortalRect proj_out(const PortalRect& cs)  const { return cs.proj_in(*this); }

	// Project the vector from world-space into this coordinate system. (Coordinates between 0..1 appear inside this rectangle.)
	inline ImVec2d proj_vin(const ImVec2d& v)  const { return v * size() + min; }
	inline ImVec2d proj_vout(const ImVec2d& v) const { return (v - min) / size(); }
};
#endif
