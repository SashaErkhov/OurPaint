//
// Created by Eugene Bychkov on 02.04.2024.
//

#include "BMPpainter.h"
BMPpainter::BMPpainter(/*const char* file*/){
    //filename = file;
}
void BMPpainter::drawSection(section s, bool isWhite){
    //Алгоритм Брезенхема
    int x0 = s.beg->x;
    int y0 = s.beg->y;
    int x1 = s.end->x;
    int y1 = s.end->y;
    int deltaX = abs(x1 - x0);
    int dirX = x0 < x1 ? 1 : -1;
    int deltaY = -abs(y1 - y0);
    int dirY = y0 < y1 ? 1 : -1;
    int err = deltaX + deltaY;
    int e2;
    for (;;) {
        filename.setPixel(x0, y0, isWhite);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 <= deltaX) { err += deltaX; y0 += dirY; } /* e_xy+e_y < 0 */
        if (e2 >= deltaY) { err += deltaY; x0 += dirX; } /* e_xy+e_x > 0 */
    }
}

void BMPpainter::drawPoint(point p, bool isWhite){
    filename.setPixel(p.x, p.y, isWhite);
}
void BMPpainter::drawCircle(circle c, bool isWhite){
    int x = 0;
    int y = static_cast<int>(c.R);
    int delta = 1 - 2 * y;
    int error = 0;
    while (y >= x) {
        filename.setPixel(c.center->x + x, c.center->y + y, isWhite);
        filename.setPixel(c.center->x + x, c.center->y - y, isWhite);
        filename.setPixel(c.center->x - x, c.center->y + y, isWhite);
        filename.setPixel(c.center->x - x, c.center->y - y, isWhite);
        filename.setPixel(c.center->x + y, c.center->y + x, isWhite);
        filename.setPixel(c.center->x + y, c.center->y - x, isWhite);
        filename.setPixel(c.center->x - y, c.center->y + x, isWhite);
        filename.setPixel(c.center->x - y, c.center->y - x, isWhite);
        error = (delta + y) * 2 - 1;
        if ((delta < 0) && (error <= 0)) {
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0)) {
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }


}
void BMPpainter::saveBMP(const char* file){
    filename.saveBmp(file);
}
