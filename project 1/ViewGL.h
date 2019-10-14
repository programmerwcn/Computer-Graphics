//
// Created by 吴小宁 on 2019/10/4.
//

#ifndef PROJECT1_VIEWGL_H
#define PROJECT1_VIEWGL_H

#include "ModelGL.h"
#include "string"
#include "ClippingPolygon.h"


class ViewGL {
public:
    ClippingPolygon clippingPolygon;
    Polygon::Point wMin, wMax;
    void view_outline(string algorithm, Polygon p);
    void raster(Polygon p);

};


#endif //PROJECT1_VIEWGL_H
