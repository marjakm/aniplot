/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab : */

//// axis-aligned coordinate system frame. no rotation possible. only scaling and translation.
//struct AACoordSys2d {
//    AACoordSys2d(): xyaxis(1,1) {}
//    AACoordSys2d(const AACoordSys2d& cs): pos(cs.pos), xyaxis(cs.xyaxis) {}
//    AACoordSys2d(const ImVec2& _pos, const ImVec2& _xyaxis): pos(_pos), xyaxis(_xyaxis) {}
//    AACoordSys2d(const ImRect& rect): xyaxis(1./(rect.Max-rect.Min)) {
//        // TODO: think about this a bit more. can proj-in-out to achieve the same result?
//        pos = -xyaxis * rect.Min;
//    }
//    // AACoordSys2d(const ImRect& rect) {
//    //     pos = -rect.Min;
//    //     xyaxis = 1. / (rect.Max - rect.Min);
//    // }
//
//    AACoordSys2d proj_in(const AACoordSys2d& cs)  { return AACoordSys2d(proj_vin(cs.pos), cs.xyaxis / xyaxis); }
//    AACoordSys2d proj_out(const AACoordSys2d& cs) { return AACoordSys2d(proj_vout(cs.pos), cs.xyaxis * xyaxis); }
//
//    // Project the vector from world-space into this coordinate system.
//    inline ImVec2 proj_vin(const ImVec2& v)  { return (v - pos) / xyaxis; }
//    // Project the vector out from this coordinate system into world-space
//    inline ImVec2 proj_vout(const ImVec2& v) { return (v * xyaxis) + pos; }
//
//    ImVec2 pos;
//    ImVec2 xyaxis; // x parameter represents an axis (x, 0) and the y parameter (0, y)
//};
