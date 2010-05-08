// This file is part of fityk program. Copyright (C) Marcin Wojdyr
// Licence: GNU General Public License ver. 2+
// $Id$

#include "common.h"
#include "numfuncs.h"

#include <algorithm>

using namespace std;

vector<PointQ>::iterator
get_interpolation_segment(vector<PointQ> &bb,  fp x)
{
    //optimized for sequence of x = x1, x2, x3, x1 < x2 < x3...
    static vector<PointQ>::iterator pos = bb.begin();
    assert (size(bb) > 1);
    if (x <= bb.front().x)
        return bb.begin();
    if (x >= bb.back().x)
        return bb.end() - 2;
    if (pos < bb.begin() || pos >= bb.end())
        pos = bb.begin();
    // check if current pos is ok
    if (pos->x <= x) {
        //pos->x <= x and x < bb.back().x and bb is sorted  => pos < bb.end()-1
        if (x <= (pos+1)->x)
            return pos;
        // try again
        pos++;
        if (pos->x <= x && (pos+1 == bb.end() || x <= (pos+1)->x))
            return pos;
    }
    pos = lower_bound(bb.begin(), bb.end(), PointQ(x, 0)) - 1;
    // pos >= bb.begin() because x > bb.front().x
    return pos;
}

void prepare_spline_interpolation (vector<PointQ> &bb)
{
    //first wroten for bb interpolation, then generalized
    const int n = bb.size();
    if (n == 0)
        return;
        //find d2y/dx2 and put it in .q
    bb[0].q = 0; //natural spline
    vector<fp> u(n);
    for (int k = 1; k <= n-2; k++) {
        PointQ *b = &bb[k];
        fp sig = (b->x - (b-1)->x) / ((b+1)->x - (b-1)->x);
        fp t = sig * (b-1)->q + 2.;
        b->q = (sig - 1.) / t;
        u[k] = ((b+1)->y - b->y) / ((b+1)->x - b->x) - (b->y - (b-1)->y)
                            / (b->x - (b - 1)->x);
        u[k] = (6. * u[k] / ((b+1)->x - (b-1)->x) - sig * u[k-1]) / t;
    }
    bb.back().q = 0;
    for (int k = n-2; k >= 0; k--) {
        PointQ *b = &bb[k];
        b->q = b->q * (b+1)->q + u[k];
    }
}

fp get_spline_interpolation(vector<PointQ> &bb, fp x)
{
    if (bb.empty())
        return 0.;
    if (bb.size() == 1)
        return bb[0].y;
    vector<PointQ>::iterator pos = get_interpolation_segment(bb, x);
    // based on Numerical Recipes www.nr.com
    fp h = (pos+1)->x - pos->x;
    fp a = ((pos+1)->x - x) / h;
    fp b = (x - pos->x) / h;
    fp t = a * pos->y + b * (pos+1)->y + ((a * a * a - a) * pos->q
            + (b * b * b - b) * (pos+1)->q) * (h * h) / 6.;
    return t;
}

fp get_linear_interpolation(vector<PointQ> &bb, fp x)
{
    if (bb.empty())
        return 0.;
    if (bb.size() == 1)
        return bb[0].y;
    vector<PointQ>::iterator pos = get_interpolation_segment(bb, x);
    fp a = ((pos + 1)->y - pos->y) / ((pos + 1)->x - pos->x);
    return pos->y + a * (x  - pos->x);
}


// random number utilities
static const fp TINY = 1e-12; //only for rand_gauss() and rand_cauchy()

/// normal distribution, mean=0, variance=1
fp rand_gauss()
{
    static bool is_saved = false;
    static fp saved;
    if (!is_saved) {
        fp rsq, x1, x2;
        while(1) {
            x1 = rand_1_1();
            x2 = rand_1_1();
            rsq = x1 * x1 + x2 * x2;
            if (rsq >= TINY && rsq < 1)
                break;
        }
        fp f = sqrt(-2. * log(rsq) / rsq);
        saved = x1 * f;
        is_saved = true;
        return x2 * f;
    }
    else {
        is_saved = false;
        return saved;
    }
}

fp rand_cauchy()
{
    while (1) {
        fp x1 = rand_1_1();
        fp x2 = rand_1_1();
        fp rsq = x1 * x1 + x2 * x2;
        if (rsq >= TINY && rsq < 1 && fabs(x1) >= TINY)
            return (x2 / x1);
    }
}


void SimplePolylineConvex::push_point(PointQ const& p)
{
    if (vertices.size() < 2
            || is_left(*(vertices.end() - 2), *(vertices.end() - 1), p))
        vertices.push_back(p);
    else {
        // the middle point (the last one currently in vertices) is not convex
        // remove it and check again the last three points
        vertices.pop_back();
        push_point(p);
    }
}

