//
// Created by 吴小宁 on 2019/10/4.
//

#include "ViewGL.h"
void ViewGL::view_outline(string algorithm, Polygon p) {
    Polygon view;
    clippingPolygon.polygon_clip_Suth_Hodg(wMin, wMax, p.poly_points.size(), p.poly_points, view.poly_points);
    Polygon::Point start, end;
    for (int i = 0; i < view.poly_points.size(); i++) {
        if (i == 0) {
            start = view.poly_points[0];
        }
        else {
            end  = view.poly_points[i];
            if (algorithm == "DDA") {
                view.lineDDA(start.x, start.y, end.x, end.y);
            }
            else if (algorithm == "Bres") {
                view.lineBres(start.x, start.y, end.x, end.y);
            }
            start = end;
        }
    }
    start = view.poly_points[view.poly_points.size() - 1];
    end  = view.poly_points[0];
    if (algorithm == "DDA") {
        view.lineDDA(start.x, start.y, end.x, end.y);
    }
    else if (algorithm == "Bres") {
        view.lineBres(start.x, start.y, end.x, end.y);
    }
}

void ViewGL::raster(Polygon p) {
    Polygon view;
    clippingPolygon.polygon_clip_Suth_Hodg(wMin, wMax, p.poly_points.size(), p.poly_points, view.poly_points);
    if (view.poly_points.size() == 0) {
        return;
    }
    view.fillPolygon();
}