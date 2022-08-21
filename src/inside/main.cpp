// Tomas Ortega Rojas
// Computer Graphics Assignment 1 Fall 2021
// Implementation of a function to test whether a point is inside a polygon in 2D.
//
////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

double inline det(const Point &u, const Point &v) {
	return u.real()*v.imag() - v.real()*u.imag();
}

double distance(const Point &a, const Point &b){
    return sqrt(pow(b.real() - a.real(), 2) + pow(b.imag() - a.imag(), 2));
}


// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {

	// check if a vector intersects with another vector
	double x = (c.imag()-d.imag())*(c.real()-a.real()) + (d.real()-c.real())*(c.imag()-a.imag());
    double y = (a.imag()-b.imag())*(c.real()-a.real()) + (b.real()-a.real())*(c.imag()-a.imag());

    if (det(b-a, c-d) == 0){
        return false;
    }
    Point p = (1/det(b-a, c-d))*Point(x, y);
    ans = a + p.real()*(b-a);
    ans = p;
    // We apply this check to avoid counting an intersect that occurs outside the polygon. Meaning tha the edges have been extended to meet in a galaxy far, far away.
    if(p.real() < 1 && p.real() > 0 && p.imag() > 0 && p.imag() < 1){
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool is_inside(const Polygon &poly, const Point &query) {
    // 1. Compute bounding box and set coordinate of a point outside the polygon
    double leftmost = poly[0].real();
    double rightmost = poly[0].real();
    double highest = poly[0].imag();
    double lowest = poly[0].imag();
    for(Point p : poly){
        if(p.real() < leftmost){
            leftmost = p.real();
        }
        if(p.real() > rightmost){
            rightmost = p.real();
        }
        if(p.imag() < lowest){
            lowest = p.imag();
        }
        if(p.imag() > highest){
            highest = p.imag();
        }
    }
    // TODO: 2. Cast a ray from the query point to the 'outside' point, count number of intersections
    double x, y;
    if(rand()>0.5){
        x = rand()% int(20*(rightmost - leftmost)) + rightmost;
    }else{
        x = -(rand()% int(20*(rightmost - leftmost) + leftmost));
    }
    if(rand()>0.5){
        y = rand()% int(20*(highest - lowest)) + highest;
    }else{
        y = -(rand() % int(20*(highest - lowest)) + lowest);
    }
    Point outside(x, y);
    int inters = 0;
    for(int i = 0; i < poly.size(); i++){
        Point a, b;
        while(true){
            a = poly[i%poly.size()];
            b = poly[(i+1)%poly.size()];
            if(a != b){ break; }
            i++;
        }
        Point ans;
        if (intersect_segment(a, b, query, outside, ans)){
            inters++;
        }
    }
    if(inters%2 == 0){
        return false;
    }
	return true;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
	std::vector<Point> points;
	std::ifstream in(filename);
	int num_points;
	in >> num_points;
	float a, b, c;
	for(int i = 0; i < num_points; i++){
	    in >> a >> b >> c;
	    points.push_back(Point(a, b));
	}
	return points;
}

Polygon load_obj(const std::string &filename) {
    Polygon poly;
	std::ifstream in(filename);
	char c;
	double x, y, z;
    std::string s;
    while(in >> c){
	    if(c == '#'){
            in >> s;
	    }else if(c == 'v'){
	        in >> x >> y >> z;
	        poly.push_back(Point(x, y));
	    }else{
	        // For now we will assume that the order in which the vertices come is the order in which they are connected.
	        in >> s;
	        // TODO: figure out if we need to pay attention to the faces or not
	    }
	}
    return poly;
}

// Save the points that are inside the polygon
void save_xyz(const std::string &filename, const std::vector<Point> &points) {
    std::ofstream out(filename);
    out << points.size() << std::endl;
    for(Point p: points){
        out << p.real() << " " << p.imag() << " " << 0 << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);
	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
}
