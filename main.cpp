#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <tuple>

#include <cmath>
#include <cstdlib>
#include <iterator>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// Generated code -- CC0 -- No Rights Reserved -- http://www.redblobgames.com/grids/hexagons/

//#include <cmath>
//#include <cstdlib>
//#include <vector>
//#include <algorithm>
//#include <iterator>
//using std::abs;
//using max;
//using std::vector;


struct Point
{
    const double x;
    const double y;
    Point(double x_, double y_): x(x_), y(y_) {}
};


struct Hex
{
    int q;
    int r;
    int s;
    vector<Hex>neighbors;
    int cell_index;
    int richness;
    Hex(int q_, int r_, int s_): q(q_), r(r_), s(s_) {
        if (q + r + s != 0) throw "q + r + s must be 0";
    }
};


struct FractionalHex
{
    const double q;
    const double r;
    const double s;
    FractionalHex(double q_, double r_, double s_): q(q_), r(r_), s(s_) {
        if (round(q + r + s) != 0) throw "q + r + s must be 0";
    }
};


struct OffsetCoord
{
    const int col;
    const int row;
    OffsetCoord(int col_, int row_): col(col_), row(row_) {}
};


struct DoubledCoord
{
    const int col;
    const int row;
    DoubledCoord(int col_, int row_): col(col_), row(row_) {}
};


struct Orientation
{
    const double f0;
    const double f1;
    const double f2;
    const double f3;
    const double b0;
    const double b1;
    const double b2;
    const double b3;
    const double start_angle;
    Orientation(double f0_, double f1_, double f2_, double f3_, double b0_, double b1_, double b2_, double b3_, double start_angle_): f0(f0_), f1(f1_), f2(f2_), f3(f3_), b0(b0_), b1(b1_), b2(b2_), b3(b3_), start_angle(start_angle_) {}
};


struct Layout
{
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation_, Point size_, Point origin_): orientation(orientation_), size(size_), origin(origin_) {}
};


// Forward declarations


Hex hex_add(Hex a, Hex b)
{
    return Hex(a.q + b.q, a.r + b.r, a.s + b.s);
}


Hex hex_subtract(Hex a, Hex b)
{
    return Hex(a.q - b.q, a.r - b.r, a.s - b.s);
}


Hex hex_scale(Hex a, int k)
{
    return Hex(a.q * k, a.r * k, a.s * k);
}


Hex hex_rotate_left(Hex a)
{
    return Hex(-a.s, -a.q, -a.r);
}


Hex hex_rotate_right(Hex a)
{
    return Hex(-a.r, -a.s, -a.q);
}


const vector<Hex> hex_directions = {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)};
Hex hex_direction(int direction)
{
    return hex_directions[direction];
}


Hex hex_neighbor(Hex hex, int direction)
{
    return hex_add(hex, hex_direction(direction));
}


const vector<Hex> hex_diagonals = {Hex(2, -1, -1), Hex(1, -2, 1), Hex(-1, -1, 2), Hex(-2, 1, 1), Hex(-1, 2, -1), Hex(1, 1, -2)};
Hex hex_diagonal_neighbor(Hex hex, int direction)
{
    return hex_add(hex, hex_diagonals[direction]);
}


int hex_length(Hex hex)
{
    return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}


int hex_distance(Hex a, Hex b)
{
    return hex_length(hex_subtract(a, b));
}



Hex hex_round(FractionalHex h)
{
    int qi = int(round(h.q));
    int ri = int(round(h.r));
    int si = int(round(h.s));
    double q_diff = abs(qi - h.q);
    double r_diff = abs(ri - h.r);
    double s_diff = abs(si - h.s);
    if (q_diff > r_diff && q_diff > s_diff)
    {
        qi = -ri - si;
    }
    else
    if (r_diff > s_diff)
    {
        ri = -qi - si;
    }
    else
    {
        si = -qi - ri;
    }
    return Hex(qi, ri, si);
}


FractionalHex hex_lerp(FractionalHex a, FractionalHex b, double t)
{
    return FractionalHex(a.q * (1.0 - t) + b.q * t, a.r * (1.0 - t) + b.r * t, a.s * (1.0 - t) + b.s * t);
}


vector<Hex> hex_linedraw(Hex a, Hex b)
{
    int N = hex_distance(a, b);
    FractionalHex a_nudge = FractionalHex(a.q + 1e-06, a.r + 1e-06, a.s - 2e-06);
    FractionalHex b_nudge = FractionalHex(b.q + 1e-06, b.r + 1e-06, b.s - 2e-06);
    vector<Hex> results = {};
    double step = 1.0 / max(N, 1);
    for (int i = 0; i <= N; i++)
    {
        results.push_back(hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
    }
    return results;
}



const int EVEN = 1;
const int ODD = -1;
OffsetCoord qoffset_from_cube(int offset, Hex h)
{
    int col = h.q;
    int row = h.r + int((h.q + offset * (h.q & 1)) / 2);
    if (offset != EVEN && offset != ODD)
    {
        throw "offset must be EVEN (+1) or ODD (-1)";
    }
    return OffsetCoord(col, row);
}


Hex qoffset_to_cube(int offset, OffsetCoord h)
{
    int q = h.col;
    int r = h.row - int((h.col + offset * (h.col & 1)) / 2);
    int s = -q - r;
    if (offset != EVEN && offset != ODD)
    {
        throw "offset must be EVEN (+1) or ODD (-1)";
    }
    return Hex(q, r, s);
}


OffsetCoord roffset_from_cube(int offset, Hex h)
{
    int col = h.q + int((h.r + offset * (h.r & 1)) / 2);
    int row = h.r;
    if (offset != EVEN && offset != ODD)
    {
        throw "offset must be EVEN (+1) or ODD (-1)";
    }
    return OffsetCoord(col, row);
}


Hex roffset_to_cube(int offset, OffsetCoord h)
{
    int q = h.col - int((h.row + offset * (h.row & 1)) / 2);
    int r = h.row;
    int s = -q - r;
    if (offset != EVEN && offset != ODD)
    {
        throw "offset must be EVEN (+1) or ODD (-1)";
    }
    return Hex(q, r, s);
}



DoubledCoord qdoubled_from_cube(Hex h)
{
    int col = h.q;
    int row = 2 * h.r + h.q;
    return DoubledCoord(col, row);
}


Hex qdoubled_to_cube(DoubledCoord h)
{
    int q = h.col;
    int r = int((h.row - h.col) / 2);
    int s = -q - r;
    return Hex(q, r, s);
}


DoubledCoord rdoubled_from_cube(Hex h)
{
    int col = 2 * h.q + h.r;
    int row = h.r;
    return DoubledCoord(col, row);
}


Hex rdoubled_to_cube(DoubledCoord h)
{
    int q = int((h.col - h.row) / 2);
    int r = h.row;
    int s = -q - r;
    return Hex(q, r, s);
}




const Orientation layout_pointy = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const Orientation layout_flat = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);
Point hex_to_pixel(Layout layout, Hex h)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    double x = (M.f0 * h.q + M.f1 * h.r) * size.x;
    double y = (M.f2 * h.q + M.f3 * h.r) * size.y;
    return Point(x + origin.x, y + origin.y);
}


FractionalHex pixel_to_hex(Layout layout, Point p)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    Point origin = layout.origin;
    Point pt = Point((p.x - origin.x) / size.x, (p.y - origin.y) / size.y);
    double q = M.b0 * pt.x + M.b1 * pt.y;
    double r = M.b2 * pt.x + M.b3 * pt.y;
    return FractionalHex(q, r, -q - r);
}


Point hex_corner_offset(Layout layout, int corner)
{
    Orientation M = layout.orientation;
    Point size = layout.size;
    double angle = 2.0 * M_PI * (M.start_angle - corner) / 6.0;
    return Point(size.x * cos(angle), size.y * sin(angle));
}


vector<Point> polygon_corners(Layout layout, Hex h)
{
    vector<Point> corners = {};
    Point center = hex_to_pixel(layout, h);
    for (int i = 0; i < 6; i++)
    {
        Point offset = hex_corner_offset(layout, i);
        corners.push_back(Point(center.x + offset.x, center.y + offset.y));
    }
    return corners;
}




// Tests

#include <iostream>

void complain(const char* name)
{
    std::cout << "FAIL " << name << std::endl;
}


void equal_hex(const char* name, Hex a, Hex b)
{
    if (!(a.q == b.q && a.s == b.s && a.r == b.r))
    {
        complain(name);
    }
}

bool equal_hex( Hex a, Hex b)
{
    if (!(a.q == b.q && a.s == b.s && a.r == b.r))
    {
        return false;
    }
    return true;
}


void equal_offsetcoord(const char* name, OffsetCoord a, OffsetCoord b)
{
    if (!(a.col == b.col && a.row == b.row))
    {
        complain(name);
    }
}


void equal_doubledcoord(const char* name, DoubledCoord a, DoubledCoord b)
{
    if (!(a.col == b.col && a.row == b.row))
    {
        complain(name);
    }
}


void equal_int(const char* name, int a, int b)
{
    if (!(a == b))
    {
        complain(name);
    }
}


void equal_hex_array(const char* name, vector<Hex> a, vector<Hex> b)
{
    equal_int(name, a.size(), b.size());
    for (int i = 0; i < a.size(); i++)
    {
        equal_hex(name, a[i], b[i]);
    }
}


void test_hex_arithmetic()
{
    equal_hex("hex_add", Hex(4, -10, 6), hex_add(Hex(1, -3, 2), Hex(3, -7, 4)));
    equal_hex("hex_subtract", Hex(-2, 4, -2), hex_subtract(Hex(1, -3, 2), Hex(3, -7, 4)));
}


void test_hex_direction()
{
    equal_hex("hex_direction", Hex(0, -1, 1), hex_direction(2));
}


void test_hex_neighbor()
{
    equal_hex("hex_neighbor", Hex(1, -3, 2), hex_neighbor(Hex(1, -2, 1), 2));
}


void test_hex_diagonal()
{
    equal_hex("hex_diagonal", Hex(-1, -1, 2), hex_diagonal_neighbor(Hex(1, -2, 1), 3));
}


void test_hex_distance()
{
    equal_int("hex_distance", 7, hex_distance(Hex(3, -7, 4), Hex(0, 0, 0)));
}


void test_hex_rotate_right()
{
    equal_hex("hex_rotate_right", hex_rotate_right(Hex(1, -3, 2)), Hex(3, -2, -1));
}


void test_hex_rotate_left()
{
    equal_hex("hex_rotate_left", hex_rotate_left(Hex(1, -3, 2)), Hex(-2, -1, 3));
}


void test_hex_round()
{
    FractionalHex a = FractionalHex(0.0, 0.0, 0.0);
    FractionalHex b = FractionalHex(1.0, -1.0, 0.0);
    FractionalHex c = FractionalHex(0.0, -1.0, 1.0);
    equal_hex("hex_round 1", Hex(5, -10, 5), hex_round(hex_lerp(FractionalHex(0.0, 0.0, 0.0), FractionalHex(10.0, -20.0, 10.0), 0.5)));
    equal_hex("hex_round 2", hex_round(a), hex_round(hex_lerp(a, b, 0.499)));
    equal_hex("hex_round 3", hex_round(b), hex_round(hex_lerp(a, b, 0.501)));
    equal_hex("hex_round 4", hex_round(a), hex_round(FractionalHex(a.q * 0.4 + b.q * 0.3 + c.q * 0.3, a.r * 0.4 + b.r * 0.3 + c.r * 0.3, a.s * 0.4 + b.s * 0.3 + c.s * 0.3)));
    equal_hex("hex_round 5", hex_round(c), hex_round(FractionalHex(a.q * 0.3 + b.q * 0.3 + c.q * 0.4, a.r * 0.3 + b.r * 0.3 + c.r * 0.4, a.s * 0.3 + b.s * 0.3 + c.s * 0.4)));
}


void test_hex_linedraw()
{
    equal_hex_array("hex_linedraw", {Hex(0, 0, 0), Hex(0, -1, 1), Hex(0, -2, 2), Hex(1, -3, 2), Hex(1, -4, 3), Hex(1, -5, 4)}, hex_linedraw(Hex(0, 0, 0), Hex(1, -5, 4)));
}


void test_layout()
{
    Hex h = Hex(3, 4, -7);
    Layout flat = Layout(layout_flat, Point(10.0, 15.0), Point(35.0, 71.0));
    equal_hex("layout", h, hex_round(pixel_to_hex(flat, hex_to_pixel(flat, h))));
    Layout pointy = Layout(layout_pointy, Point(10.0, 15.0), Point(35.0, 71.0));
    equal_hex("layout", h, hex_round(pixel_to_hex(pointy, hex_to_pixel(pointy, h))));
}


void test_offset_roundtrip()
{
    Hex a = Hex(3, 4, -7);
    OffsetCoord b = OffsetCoord(1, -3);
    equal_hex("conversion_roundtrip even-q", a, qoffset_to_cube(EVEN, qoffset_from_cube(EVEN, a)));
    equal_offsetcoord("conversion_roundtrip even-q", b, qoffset_from_cube(EVEN, qoffset_to_cube(EVEN, b)));
    equal_hex("conversion_roundtrip odd-q", a, qoffset_to_cube(ODD, qoffset_from_cube(ODD, a)));
    equal_offsetcoord("conversion_roundtrip odd-q", b, qoffset_from_cube(ODD, qoffset_to_cube(ODD, b)));
    equal_hex("conversion_roundtrip even-r", a, roffset_to_cube(EVEN, roffset_from_cube(EVEN, a)));
    equal_offsetcoord("conversion_roundtrip even-r", b, roffset_from_cube(EVEN, roffset_to_cube(EVEN, b)));
    equal_hex("conversion_roundtrip odd-r", a, roffset_to_cube(ODD, roffset_from_cube(ODD, a)));
    equal_offsetcoord("conversion_roundtrip odd-r", b, roffset_from_cube(ODD, roffset_to_cube(ODD, b)));
}


void test_offset_from_cube()
{
    equal_offsetcoord("offset_from_cube even-q", OffsetCoord(1, 3), qoffset_from_cube(EVEN, Hex(1, 2, -3)));
    equal_offsetcoord("offset_from_cube odd-q", OffsetCoord(1, 2), qoffset_from_cube(ODD, Hex(1, 2, -3)));
}


void test_offset_to_cube()
{
    equal_hex("offset_to_cube even-", Hex(1, 2, -3), qoffset_to_cube(EVEN, OffsetCoord(1, 3)));
    equal_hex("offset_to_cube odd-q", Hex(1, 2, -3), qoffset_to_cube(ODD, OffsetCoord(1, 2)));
}


void test_doubled_roundtrip()
{
    Hex a = Hex(3, 4, -7);
    DoubledCoord b = DoubledCoord(1, -3);
    equal_hex("conversion_roundtrip doubled-q", a, qdoubled_to_cube(qdoubled_from_cube(a)));
    equal_doubledcoord("conversion_roundtrip doubled-q", b, qdoubled_from_cube(qdoubled_to_cube(b)));
    equal_hex("conversion_roundtrip doubled-r", a, rdoubled_to_cube(rdoubled_from_cube(a)));
    equal_doubledcoord("conversion_roundtrip doubled-r", b, rdoubled_from_cube(rdoubled_to_cube(b)));
}


void test_doubled_from_cube()
{
    equal_doubledcoord("doubled_from_cube doubled-q", DoubledCoord(1, 5), qdoubled_from_cube(Hex(1, 2, -3)));
    equal_doubledcoord("doubled_from_cube doubled-r", DoubledCoord(4, 2), rdoubled_from_cube(Hex(1, 2, -3)));
}


void test_doubled_to_cube()
{
    equal_hex("doubled_to_cube doubled-q", Hex(1, 2, -3), qdoubled_to_cube(DoubledCoord(1, 5)));
    equal_hex("doubled_to_cube doubled-r", Hex(1, 2, -3), rdoubled_to_cube(DoubledCoord(4, 2)));
}


void test_all()
{
    test_hex_arithmetic();
    test_hex_direction();
    test_hex_neighbor();
    test_hex_diagonal();
    test_hex_distance();
    test_hex_rotate_right();
    test_hex_rotate_left();
    test_hex_round();
    test_hex_linedraw();
    test_layout();
    test_offset_roundtrip();
    test_offset_from_cube();
    test_offset_to_cube();
    test_doubled_roundtrip();
    test_doubled_from_cube();
    test_doubled_to_cube();
}

vector<Hex> inserNeighbours(vector<Hex> &arr){
    for (int i = 0; i < arr.size(); ++i) {
        arr[i].neighbors.push_back(hex_neighbor(arr[i],0));
        arr[i].neighbors.push_back(hex_neighbor(arr[i],1));
        arr[i].neighbors.push_back(hex_neighbor(arr[i],2));
        arr[i].neighbors.push_back(hex_neighbor(arr[i],3));
        arr[i].neighbors.push_back(hex_neighbor(arr[i],4));
        arr[i].neighbors.push_back(hex_neighbor(arr[i],5));
    }
    return arr;
}


class Cell {
public:
    Cell () {
        neighbors.resize(6);
    }

    void input() {
        cin >> cell_index >> richness;
        //cerr << cell_index << endl;
        for (auto& n: neighbors) {
            cin >> n;
        }
    }
    int cell_index;
    int richness;
    vector<int> neighbors; // from right counter clock vise. if no cell then -1
};

class Tree {
public:
    Tree () = default;
    Tree (int cell_index, int size, bool is_mine, bool is_dormant, vector<int> shadows ) :
            cell_index{cell_index}, size{size}, is_mine{is_mine}, is_dormant{is_dormant}, shadows{shadows} {}
    void input() {
        cin >> cell_index >> size >> is_mine >> is_dormant;
//        cerr << right << setw(10) << "cellIndex: " << cell_index << "  size: " << size
//        << "  isMine: " << is_mine << "  isDormant: " << is_dormant<< endl;
    }
    int cell_index;
    int size;
    bool is_mine;
    bool is_dormant;
    vector<int> shadows; // 0 false, 1 true
};

class Game {
private:
    int day = 0;
    int nutrients = 0;
    vector<Cell> board;
    vector<Tree> trees;
    vector<tuple<string,int,int>> possible_actions;
    int mySun;
    int oppSun;
    int score;
    int oppScore;
    int oppIsWaiting;

public:
    vector<tuple<string,int,int>> previous_actions;

    void inputInitData() {
        int numberOfCells;
        cin >> numberOfCells;
        for (int i = 0; i < numberOfCells; i++) {
            Cell cell;
            cell.input();
            board.push_back(cell);
        }
    }

    void inputInfo() {
        // input game info
        cin >> day;
        cin >> nutrients;
        cin >> mySun >> score;
        cin >> oppSun >> oppScore >> oppIsWaiting;

//        cerr << right << setw(12) << "nutrients" << nutrients << endl;
//        cerr << right << setw(12) << "day: " << day << endl;
//        cerr << right << setw(12) << "sun: " << mySun << endl;
//        cerr << right << setw(12) << "score: " << score << endl;
//        cerr << right << setw(12) << "oppSun: " << oppSun << " oppScore: " << oppScore << " oppIsWaiting: " << oppIsWaiting << endl;


        // input trees info
        trees.clear();
        int numberOfTrees;
        cin >> numberOfTrees;
        //cerr << right << setw(10) << "numberOfTree: " << numberOfTrees << endl;
        for (int i = 0; i < numberOfTrees; i++) {
            Tree tree;
            tree.input();
            trees.push_back(tree);
        }

        // input possible actions
        possible_actions.clear();
        int numberOfPossibleMoves;
        cin >> numberOfPossibleMoves;
        ///cerr << right << setw(10) << "numberOfPossibleMoves: "<< numberOfPossibleMoves << endl;
        for (int i = 0; i < numberOfPossibleMoves; i++) {
            string type;
            int arg1 = 0;
            int arg2 = 0;
            cin >> type;
            ///cerr << right << setw(10) << "possibileMove: " << type << endl;
            if (type == "WAIT") {
                possible_actions.push_back(make_tuple(type, arg1,arg2));
            } else if (type == "COMPLETE"){
                cin >> arg1;
                possible_actions.push_back(make_tuple(type, arg1,arg2));
            }
            else if (type == "GROW") {
                cin >> arg1;
                possible_actions.push_back(make_tuple(type, arg1,arg2));
            }
            else if (type == "SEED") {
                cin >> arg1;
                cin >> arg2;
                possible_actions.push_back(make_tuple(type, arg1,arg2));
            }
            // TODO: Add "GROW" and "SEED" once they become available
        }
    }

    //takes Id, returns richness
    int richness(int i){
        return board[i].richness;
    }
    //takes arr and Id, returns Hex
    Hex findById(vector<Hex> arr, int id) {
        for (int i = 0; i < arr.size(); i++) {
            if (id == arr[i].cell_index) {
                return arr[i];
            }
        }
        cerr << "findById - FAILED";
        return arr[18]; // if not found, return center...
    }

    void updateDataByHex(vector<Hex> &updating, vector<Hex> parent){
        for (vector<Hex>::iterator it = updating.begin(); it != updating.end(); ++it) {
            for (vector<Hex>::iterator its = parent.begin(); its != parent.end(); ++its) {
                if (equal_hex(*it, *its)){
                    it->cell_index = its->cell_index;
                    it->richness = its->richness;
//                    cerr << "TEST it->richness " << it->richness << endl;
                }
            }
        }
    }

    //takes Id, returns size
    int getTreeSize(int id) {
        for (vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
            if (id == it->cell_index) {
                return it->size;
            }
        }
        cerr << "getTreeSize - FAILED!!!!!!!!!!!!!!!!!!!!!" << endl;
        return -1;
    }

    bool isItEnoughPoints(int medis0, int medis1, int medis2, int medis3, int size){
        int pointsNeeded = 0;
        if (size == 0){
            pointsNeeded = + medis0;
            return pointsNeeded <= mySun ? true : false;
        }
        else if (size == 1){
            pointsNeeded =+ 1 + medis1;
            return pointsNeeded <= mySun ? true : false;
        }
        else if (size == 2){
            pointsNeeded =+ 3 + medis2;
            return pointsNeeded <= mySun ? true : false;
        }
        else if (size == 3){
            pointsNeeded =+ 7 + medis3;
            return pointsNeeded <= mySun ? true : false;
        }
        else if (size == 4){ // COMPLETE
            pointsNeeded =+ 4;
            return pointsNeeded <= mySun ? true : false;
        }
    }

    void medziuKiekis(int &medis0, int &medis1, int &medis2, int &medis3, string &gameBalance){
        for (int k = 0; k < trees.size(); k++){
            if (trees[k].is_mine == 1) {
                if (trees[k].size == 0){medis0++; }
                else if (trees[k].size == 1){ medis1++; }
                else if (trees[k].size == 2){ medis2++; }
                else{ medis3++; }
            }
        }
        if (medis0 == 2){gameBalance = "GROW0";}
        if (medis1 == 2){gameBalance = "GROW1";}
        if (medis2 == 2){gameBalance = "GROW2";}

        if (medis0 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 1)){gameBalance = "GROW0";}
        if (medis1 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 2)){gameBalance = "GROW1";}
        if (medis2 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 3)){gameBalance = "GROW2";}
        if (gameBalance == ""){gameBalance = "WAIT";}
        if (medis0 == 0 ){gameBalance = "";} // make a seed
//        cerr << "isItEnoughPoints: " << isItEnoughPoints(medis0, medis1, medis2, medis3, 1) << "   medis0123 ...: "<< medis0<< " "<<medis1 <<" "<<medis2<< " "<<medis3<< "\n" <<(medis0 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 1))<<endl;
    }

    void medziuKiekis1(int &medis0, int &medis1, int &medis2, int &medis3, string &gameBalance){
        for (int k = 0; k < trees.size(); k++){
            if (trees[k].is_mine == 1) {
                if (trees[k].size == 0){medis0++; }
                else if (trees[k].size == 1){ medis1++; }
                else if (trees[k].size == 2){ medis2++; }
                else{ medis3++; }
            }
        }

        if (medis0 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 1)){gameBalance = "GROW0";}
        if (day > 15 && medis1 < 1) {
            if (medis0 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 1)){gameBalance = "GROW0";}
        }
        if (medis2 < 2) {
            if (medis1 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 2)) { gameBalance = "GROW1"; }
        }
        if (medis2 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 3)){gameBalance = "GROW2";}
        if (gameBalance == ""){gameBalance = "WAIT";}
        if (medis0 == 0 ){gameBalance = "";} // make a seed
        if (oppScore > score ){
            if (day > 11 && medis3 > 3 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
            if (day > 19 && medis3 > 1 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
        }

        if (day > 13 && medis3 > 4 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}

        cerr << "gameBalance: " << gameBalance<< endl;
        cerr << "day: " << day << endl;
    }
    bool isNotDormant(int medis0, int medis1, int medis2, int medis3, string gameBalance){
        if ( gameBalance == "GROW0" || gameBalance == "GROW1"|| gameBalance == "GROW2"){// check is it not dormant tree for this decision
            for (int k = 0; k < trees.size(); k++){
                if (trees[k].is_mine == 1 && trees[k].is_dormant == 0) {
                    if (trees[k].size == 0){medis0++; }
                    else if (trees[k].size == 1){ medis1++; }
                    else if (trees[k].size == 2){ medis2++; }
                    else{ medis3++; }
                }
            }

            if ( gameBalance == "GROW0" && medis0 == 0){
                return false;
            }
            else if ( gameBalance == "GROW1" && medis0 == 0){
                return false;
            }
            else if ( gameBalance == "GROW2" && medis0 == 0){
                return false;
            }
        }
        return true;
    }

    void medziuKiekis2(int &medis0, int &medis1, int &medis2, int &medis3, string &gameBalance){
        for (int k = 0; k < trees.size(); k++){
            if (trees[k].is_mine == 1) {
                if (trees[k].size == 0){medis0++; }
                else if (trees[k].size == 1){ medis1++; }
                else if (trees[k].size == 2){ medis2++; }
                else{ medis3++; }
            }
        }

        //if (medis0 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 1)){gameBalance = "GROW0";}
        if (medis1 < 1) {
            if (medis0 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 1 ) && isNotDormant(medis0, medis1, medis2, medis3, "GROW0" )){gameBalance = "GROW0";}
        }
        if (medis2 < 2) {
            if (medis1 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 2) && isNotDormant(medis0, medis1, medis2, medis3, "GROW1" )) { gameBalance = "GROW1"; }
        }
        if (medis2 > 0 && isItEnoughPoints(medis0, medis1, medis2, medis3, 3) && isNotDormant(medis0, medis1, medis2, medis3, "GROW2" )){gameBalance = "GROW2";}
        if (gameBalance == ""){gameBalance = "WAIT";}
        if (medis0 == 0 && day <= 20 ){gameBalance = "";} // make a seed
        if (day >= 21 && gameBalance == "GROW0" || day >= 21 && gameBalance == "GROW1"){gameBalance = "WAIT";}
        if (oppScore > score ){
            if (day > 15 && medis3 > 4 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
            if (day > 19 && medis3 > 1 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
        }
        if (day > 15 && medis3 > 4 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
        if (day >= 20 && medis3 > 4 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
        if (day >= 21 && medis3 > 3 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
        if (day >= 22 && medis3 > 2 && isItEnoughPoints(medis0, medis1, medis2, medis3, 4)){gameBalance = "COMPLETE3";}
        if (day >= 23 ){gameBalance = "COMPLETE3";}

        if (day == 23 && gameBalance == "WAIT" && oppScore == score  && oppSun < mySun && (medis3+medis0+medis1+medis2 >0)){
            int myniTrees = medis3+medis0+medis1+medis2;
            int forinerTrees =0;
            for (int k = 0; k < trees.size(); k++){
                if (trees[k].is_mine == 0) {
                    if (trees[k].size == 0){forinerTrees++; }
                    else if (trees[k].size == 1){ forinerTrees++; }
                    else if (trees[k].size == 2){ forinerTrees++; }
                    else{ forinerTrees++; }
                }
            }
            if (myniTrees < forinerTrees && oppSun <= mySun){
                gameBalance == "SEED";
            }

        }



        cerr << "gameBalance: " << gameBalance<< "     Nutrients: "<< nutrients<< endl;
        cerr << "day: " << day << endl;
        if (gameBalance == "COMPLETE3" && medis3 == 0) {
            gameBalance = "WAIT";
        }


//todo        neaugint daugiau kaip 3 ir nekerta is centro ir
    }




    // per letas
    vector<Hex> shadowsCheck(vector<Hex> mapOfRichCells, vector<Tree> &trees, int futureDay){

//        int i=0;
//        while(i < 6){
//            i++;
//            futureDay++;
//            futureDay = futureDay % 6;
//            // get Cells in shadow
//            if (futureDay == 0){
//                vector<vector<Hex>> day0{ {hex_linedraw(Hex(0,-3,3),Hex(3,-3,0))}, //returns just hex way - without other values
//                                            {hex_linedraw(Hex(-1,-2,3),Hex(3,-2,-1))},
//                                            {hex_linedraw(Hex(-2,-1,3),Hex(3,-1,-2))},
//                                            {hex_linedraw(Hex(-3,0,3),Hex(3,0,-3))},
//                                            {hex_linedraw(Hex(-3,1,2),Hex(2,1,-3))},
//                                            {hex_linedraw(Hex(-3,2,1),Hex(1,2,-3))},
//                                            {hex_linedraw(Hex(-3,3,0),Hex(0,3,-3))}};
//
//            }
//            else if (futureDay == 1){
//                //Hexagona suskirstome i eilutes
//                vector<vector<Hex>> day1 { {hex_linedraw(Hex(-3,0,3),Hex(0,-3,3))},
//                                           {hex_linedraw(Hex(-3,1,2),Hex(1,-3,2))},
//                                           {hex_linedraw(Hex(-3,2,1),Hex(2,-3,1))},
//                                           {hex_linedraw(Hex(-3,3,0),Hex(3,-3,0))},
//                                           {hex_linedraw(Hex(-2,3,-1),Hex(3,-2,-1))},
//                                           {hex_linedraw(Hex(-1,3,-2),Hex(3,-1,-2))},
//                                           {hex_linedraw(Hex(0,3,-3),Hex(3,0,-3))}};
//
////                        for (vector<Hex> :: iterator it = day1.begin(); it != day1.end(); it++) {
////                            cerr << it->q << " " << it->r << " " << it->s << "    ";
////                        }
////                        cerr << endl;
//            }
//            else if (futureDay == 2){
//                vector<vector<Hex>> day2  { {hex_linedraw(Hex(-3, 3, 0), Hex(-3, 0, 3))},
//                                            { hex_linedraw(Hex(-2, 3, -1), Hex(-2, -1, 3)) },
//                                            { hex_linedraw(Hex(-1, 3, -2), Hex(-1, -2, 3)) },
//                                            { hex_linedraw(Hex(0, 3, -3), Hex(0, -3, 3)) },
//                                            { hex_linedraw(Hex(1, 2, -3), Hex(1, -3, 2)) },
//                                            { hex_linedraw(Hex(2, 1, -3), Hex(2, -3, 1)) },
//                                            { hex_linedraw(Hex(3, 0, -3), Hex(3, -3, 0)) }};
//            }
//            else if (futureDay == 3){
//                vector<vector<Hex>> day3 { {hex_linedraw(Hex(3, -3, 0), Hex(0, -3, 3))},
//                                     {hex_linedraw(Hex(3, -2, -1), Hex(-1, -2, 3))},
//                                     {hex_linedraw(Hex(3, -1, -2), Hex(-2, -1, 3))},
//                                     {hex_linedraw(Hex(3, 0, -3), Hex(-3, 0, 3))},
//                                     {hex_linedraw(Hex(2, 1, -3), Hex(-3, 1, 2))},
//                                     {hex_linedraw(Hex(1, 2, -3), Hex(-3, 2, 1))},
//                                     {hex_linedraw(Hex(0, 3, -3), Hex(-3, 3, 0))}};
//            }
//            else if (futureDay == 4){
//                vector<vector<Hex>>  day4 { {hex_linedraw(Hex(0, -3, 3), Hex(-3, 0, 3))},
//                                          {hex_linedraw(Hex(1, -3, 2), Hex(-3, 1, 2))},
//                                          {hex_linedraw(Hex(2, -3, 1), Hex(-3, 2, 1))},
//                                          {hex_linedraw(Hex(3, -3, 0), Hex(-3, 3, 0))},
//                                          {hex_linedraw(Hex(3, -2, -1), Hex(-2, 3, -1))},
//                                          {hex_linedraw(Hex(3, -1, -2), Hex(-1, 3, -2))},
//                                          {hex_linedraw(Hex(3, 0, -3), Hex(0, 3, -3))}};
//            }
//            else if (futureDay == 5){
//                vector<vector<Hex>> day5 { {hex_linedraw(Hex(-3, 0, 3), Hex(-3, 3, 0))},
//                                          {hex_linedraw(Hex(-2, -1, 3), Hex(-2, 3, -1))},
//                                          {hex_linedraw(Hex(-1, -2, 3), Hex(-1, 3, -2))},
//                                          {hex_linedraw(Hex(0, -3, 3), Hex(0, 3, -3))},
//                                          {hex_linedraw(Hex(1, -3, 2), Hex(1, 2, -3))},
//                                          {hex_linedraw(Hex(2, -3, 1), Hex(2, 1, -3))},
//                                          {hex_linedraw(Hex(3, -3, 0), Hex(3, 0, -3))}};
//            }
//        }
        vector<vector<Hex>> day0 { {hex_linedraw(Hex(0,-3,3),Hex(3,-3,0))}, //returns just hex way - without other values
                                  {hex_linedraw(Hex(-1,-2,3),Hex(3,-2,-1))},
                                  {hex_linedraw(Hex(-2,-1,3),Hex(3,-1,-2))},
                                  {hex_linedraw(Hex(-3,0,3),Hex(3,0,-3))},
                                  {hex_linedraw(Hex(-3,1,2),Hex(2,1,-3))},
                                  {hex_linedraw(Hex(-3,2,1),Hex(1,2,-3))},
                                  {hex_linedraw(Hex(-3,3,0),Hex(0,3,-3))}};
        vector<vector<Hex>> day1 { {hex_linedraw(Hex(-3,0,3),Hex(0,-3,3))},
                                   {hex_linedraw(Hex(-3,1,2),Hex(1,-3,2))},
                                   {hex_linedraw(Hex(-3,2,1),Hex(2,-3,1))},
                                   {hex_linedraw(Hex(-3,3,0),Hex(3,-3,0))},
                                   {hex_linedraw(Hex(-2,3,-1),Hex(3,-2,-1))},
                                   {hex_linedraw(Hex(-1,3,-2),Hex(3,-1,-2))},
                                   {hex_linedraw(Hex(0,3,-3),Hex(3,0,-3))}};
        vector<vector<Hex>> day2  { {hex_linedraw(Hex(-3, 3, 0), Hex(-3, 0, 3))},
                                    { hex_linedraw(Hex(-2, 3, -1), Hex(-2, -1, 3)) },
                                    { hex_linedraw(Hex(-1, 3, -2), Hex(-1, -2, 3)) },
                                    { hex_linedraw(Hex(0, 3, -3), Hex(0, -3, 3)) },
                                    { hex_linedraw(Hex(1, 2, -3), Hex(1, -3, 2)) },
                                    { hex_linedraw(Hex(2, 1, -3), Hex(2, -3, 1)) },
                                    { hex_linedraw(Hex(3, 0, -3), Hex(3, -3, 0)) }};
        vector<vector<Hex>> day3 { {hex_linedraw(Hex(3, -3, 0), Hex(0, -3, 3))},
                                   {hex_linedraw(Hex(3, -2, -1), Hex(-1, -2, 3))},
                                   {hex_linedraw(Hex(3, -1, -2), Hex(-2, -1, 3))},
                                   {hex_linedraw(Hex(3, 0, -3), Hex(-3, 0, 3))},
                                   {hex_linedraw(Hex(2, 1, -3), Hex(-3, 1, 2))},
                                   {hex_linedraw(Hex(1, 2, -3), Hex(-3, 2, 1))},
                                   {hex_linedraw(Hex(0, 3, -3), Hex(-3, 3, 0))}};
        vector<vector<Hex>>  day4 { {hex_linedraw(Hex(0, -3, 3), Hex(-3, 0, 3))},
                                    {hex_linedraw(Hex(1, -3, 2), Hex(-3, 1, 2))},
                                    {hex_linedraw(Hex(2, -3, 1), Hex(-3, 2, 1))},
                                    {hex_linedraw(Hex(3, -3, 0), Hex(-3, 3, 0))},
                                    {hex_linedraw(Hex(3, -2, -1), Hex(-2, 3, -1))},
                                    {hex_linedraw(Hex(3, -1, -2), Hex(-1, 3, -2))},
                                    {hex_linedraw(Hex(3, 0, -3), Hex(0, 3, -3))}};
        vector<vector<Hex>> day5 { {hex_linedraw(Hex(-3, 0, 3), Hex(-3, 3, 0))},
                                   {hex_linedraw(Hex(-2, -1, 3), Hex(-2, 3, -1))},
                                   {hex_linedraw(Hex(-1, -2, 3), Hex(-1, 3, -2))},
                                   {hex_linedraw(Hex(0, -3, 3), Hex(0, 3, -3))},
                                   {hex_linedraw(Hex(1, -3, 2), Hex(1, 2, -3))},
                                   {hex_linedraw(Hex(2, -3, 1), Hex(2, 1, -3))},
                                   {hex_linedraw(Hex(3, -3, 0), Hex(3, 0, -3))}};





            //debug---
            //kiekvienai eilutej, kiekvienam langeliui priskirem cell_index
//            for (int i = 0; i < day0.size(); i++) {
//                for (int j = 0; j < day0[i].size(); j++){
//                    for (vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
//                        Hex treeHex = findById(mapOfRichCells, it->cell_index);
//
//                        if (equal_hex(day0[i][j], treeHex)) {
//                            day0[i][j].cell_index = it->cell_index;
//                            day0[i][j].richness = it->size; // pasinaudoju situo kaip kad medziu dydzio informacija
//
//                        } else {
//                            day0[i][j].richness = 0; // pasinaudoju situo kaip kad medziu dydzio informacija
//                        }
//                    }
//                }
//            }


//        for (int i = 0; i < day1.size(); i++) {
//            for (int j = 0; j < day1[i].size(); j++) {
//
//                cerr << " Shadow TEST 1 it cell_index: " << day1[i][j].cell_index
//                     << "   is_mine (richness): " << day0[i][j].richness << endl;
//            }
//        }


//                it->shadows.push_back(1);
            // set nuber of days if shadow for Tree




    }

    //todo Manuoal inser data (laiter fix)
    void inserData(vector<Hex> &mapas, vector<Cell> board){
        //iterpia duomenis
        mapas[0].richness = board[28].richness;
        mapas[0].cell_index = board[28].cell_index;
        mapas[1].richness = board[29].richness;
        mapas[1].cell_index = board[29].cell_index;
        mapas[2].richness = board[30].richness;
        mapas[2].cell_index = board[30].cell_index;
        mapas[3].richness = board[31].richness;
        mapas[3].cell_index = board[31].cell_index;
        mapas[4].richness = board[27].richness;
        mapas[4].cell_index = board[27].cell_index;
        mapas[5].richness = board[13].richness;
        mapas[5].cell_index = board[13].cell_index;
        mapas[6].richness = board[14].richness;
        mapas[6].cell_index = board[14].cell_index;
        mapas[7].richness = board[15].richness;
        mapas[7].cell_index = board[15].cell_index;
        mapas[8].richness = board[32].richness;
        mapas[8].cell_index = board[32].cell_index;
        mapas[9].richness = board[26].richness;
        mapas[9].cell_index = board[26].cell_index;
        mapas[10].richness = board[12].richness;
        mapas[10].cell_index = board[12].cell_index;
        mapas[11].richness = board[4].richness;
        mapas[11].cell_index = board[4].cell_index;
        mapas[12].richness = board[5].richness;
        mapas[12].cell_index = board[5].cell_index;
        mapas[13].richness = board[16].richness;
        mapas[13].cell_index = board[16].cell_index;
        mapas[14].richness = board[33].richness;
        mapas[14].cell_index = board[33].cell_index;
        mapas[15].richness = board[25].richness;
        mapas[15].cell_index = board[25].cell_index;
        mapas[16].richness = board[11].richness;
        mapas[16].cell_index = board[11].cell_index;
        mapas[17].richness = board[3].richness;
        mapas[17].cell_index = board[3].cell_index;
        mapas[18].richness = board[0].richness;
        mapas[18].cell_index = board[0].cell_index;
        mapas[19].richness = board[6].richness;
        mapas[19].cell_index = board[6].cell_index;
        mapas[20].richness = board[17].richness;
        mapas[20].cell_index = board[17].cell_index;
        mapas[21].richness = board[34].richness;
        mapas[21].cell_index = board[34].cell_index;
        mapas[22].richness = board[24].richness;
        mapas[22].cell_index = board[24].cell_index;
        mapas[23].richness = board[10].richness;
        mapas[23].cell_index = board[10].cell_index;
        mapas[24].richness = board[2].richness;
        mapas[24].cell_index = board[2].cell_index;
        mapas[25].richness = board[1].richness;
        mapas[25].cell_index = board[1].cell_index;
        mapas[26].richness = board[18].richness;
        mapas[26].cell_index = board[18].cell_index;
        mapas[27].richness = board[35].richness;
        mapas[27].cell_index = board[35].cell_index;
        mapas[28].richness = board[23].richness;
        mapas[28].cell_index = board[23].cell_index;
        mapas[29].richness = board[9].richness;
        mapas[29].cell_index = board[9].cell_index;
        mapas[30].richness = board[8].richness;
        mapas[30].cell_index = board[8].cell_index;
        mapas[31].richness = board[7].richness;
        mapas[31].cell_index = board[7].cell_index;
        mapas[32].richness = board[36].richness;
        mapas[32].cell_index = board[36].cell_index;
        mapas[33].richness = board[22].richness;
        mapas[33].cell_index = board[22].cell_index;
        mapas[34].richness = board[21].richness;
        mapas[34].cell_index = board[21].cell_index;
        mapas[35].richness = board[20].richness;
        mapas[35].cell_index = board[20].cell_index;
        mapas[36].richness = board[19].richness;
        mapas[36].cell_index = board[19].cell_index;
    }




    //TODO: Please implement the algorithm in this function
    string compute_next_action() {
        string action = "WAIT"; // default

        // your algorithm !!

        int N = 3; // size of map 36
        vector<Hex> mapas;
        for (int q = -N; q <= N; q++) {
            for (int r = -N; r <= N; r++){
                Hex *hex = new Hex(q, r, -q-r);
                if (hex_length(*hex) <= N) {
                    mapas.push_back(*hex);
                }
            }
        }


        //iterpia kaimynus
        inserNeighbours(mapas);
        inserData(mapas, board);
        //gal reikia iterpti ir kaimynu data

        //debug
//        int count =-1;
//        cerr << " size: " << mapas.size() << endl;
//        for (vector<Hex>::iterator it = mapas.begin(); it != mapas.end(); ++it) {
//            count++;
//            cerr << count << " cell_index: " << it->cell_index << "   cel Hex   " << it->q << "  " << it->r << "  "
//                                 << it->s << endl;
//            for (vector<Hex>::iterator its = it->neighbors.begin(); its != it->neighbors.end(); ++its){
//                cerr << " neighbour cell id " << its->cell_index << " richnes " << its->richness;
//            }
//            cerr << endl;
//        }

        // copy only Rich cells for PathFinder:
        vector<Hex> mapOfRichCells;
        copy_if (mapas.begin(), mapas.end(), back_inserter(mapOfRichCells), [](Hex i){return i.richness > 0;} );

//        int count =-1;
//        cerr << " size: " << mapOfRichCells.size() << endl;
//        for (vector<Hex>::iterator it = mapOfRichCells.begin(); it != mapOfRichCells.end(); ++it) {
//            count++;
//            cerr << count << " cell_index: " << it->cell_index << "   cel Hex   " << it->q << "  " << it->r << "  "
//                 << it->s << endl;
//            for (vector<Hex>::iterator its = it->neighbors.begin(); its != it->neighbors.end(); ++its){
//                cerr << " neighbour cell id " << its->cell_index << " richnes " << its->richness;
//            }
//            cerr << endl;
//        }
        int medis0 =0,medis1 =0,medis2 =0, medis3 =0;
        string gameBalance ="";





        if (day < 8){
            medziuKiekis(medis0,medis1,medis2, medis3, gameBalance);
            // paziurim su kiek medziu yra ir auginam kad butu visu po viena
            cerr<<"gameBalance: "<<gameBalance << endl;
//            cerr << "Balance TEST0 gameBalance.substr(0,4) " << gameBalance.substr(0,4)<< " " << (gameBalance.substr(0,4) == "GROW") << endl;
            if (gameBalance == "" && medis1 >0){

            }
            if (gameBalance.substr(0,4) == "GROW"){
                cerr << "Balance TEST1" << endl;
                bool foundDecision = false;
                for (const auto& i : possible_actions){
                    if ("GROW" == get<0>(i) && getTreeSize(get<1>(i)) == gameBalance[4] - '0'  ){ // jai medis
                        gameBalance = get<0>(i) + " " + to_string(get<1>(i));
                        cerr << "Balance day < 8 decision : " << gameBalance << endl;
                        foundDecision = true;
                        break;
                        //return action;
                    }
                }
                if (!foundDecision) {
                    if (mySun < 3 || gameBalance != ""){ //probably dormant
                        gameBalance ="WAIT";
                    }
                    else{
                        gameBalance ="";
                    }
                    cerr << "Balance decision else: " <<gameBalance<< endl;
                    //return action;
                }
            }
            else if (gameBalance.substr(0,8) == "COMPLETE"){
                // too early to cut
                gameBalance ="";
            }
        }
        else if (day >= 8 && day <= 16){
            medziuKiekis1(medis0,medis1,medis2, medis3, gameBalance);
            if (gameBalance.substr(0,8) == "COMPLETE"){
                //cerr << "Balance TEST1" << endl;
                bool foundDecision = false;
                string outerRing = "";
                string midleRing = "";
                string innerRing = "";
                for (int i=0; i< possible_actions.size(); i++) {
                    string a = get<0>(possible_actions[i]);
                    int b = get<1>(possible_actions[i]);
                    int c = get<2>(possible_actions[i]);
//                    cerr << "Balance possible_action: " << a << "   getTreeSize(get<1>(i)): "<< getTreeSize(b) << "   gameBalance[4] - '0': " << (gameBalance[8] - '0')<< ":"<<gameBalance << endl;
//                    cerr << "possible_action: " << a << " position: " << b << " splited if statement: "<< ("COMPLETE" == a ) <<  (getTreeSize(b) == gameBalance[8] - '0') << " if statement: " << ("COMPLETE" == a && getTreeSize(b) == gameBalance[8] - '0') << endl;

                    if ("COMPLETE" == a && getTreeSize(b) == 3) { // jai medis
 //Iskirstu centra
                        if(b < 7){
                            gameBalance = a + " " + to_string(b);
                            cerr << "Balance day 8-18 decision < 7 : " << gameBalance << endl;
                            foundDecision = true;
                            innerRing = gameBalance;
                            //return action;
                        }
                        else if(b < 19){
                            gameBalance = a + " " + to_string(b);
                            cerr << "Balance day 8-18 decision < 19: " << gameBalance << endl;
                            foundDecision = true;
                            midleRing = gameBalance;
                            //return action;
                        }
                        else if(b >= 19){
                            gameBalance = a + " " + to_string(b);
                            cerr << "Balance day 8-18 decision >= 19: " << gameBalance << endl;
                            foundDecision = true;
                            outerRing = gameBalance;
                            //return action;
                        }

                    }
                }
                cerr << "Balance decision innerRing: "<<innerRing<<" "<< midleRing<<" "<< outerRing<< endl;
                if (innerRing != "" ){ return innerRing;}
                else if (midleRing != "" ){ return midleRing;}
                else if (outerRing != "" ){ return outerRing;}

                if (!foundDecision && gameBalance != "WAIT") {
                    gameBalance ="";
                    cerr << "Balance decision else: empty in COMPLETE" << endl;
                    //return action;
                }

            }
            else if (gameBalance.substr(0,4) == "GROW"){
                bool foundDecision = false;
                for (const auto& i : possible_actions) {
//                    cerr << "Balance get<0>(i): " << get<0>(i) << "   getTreeSize(get<1>(i)): "
//                         << getTreeSize(get<1>(i)) << "   gameBalance[4] - '0': " << (gameBalance[4] - '0') << ":"
//                         << gameBalance << endl;

                    if ("GROW" == get<0>(i) && getTreeSize(get<1>(i)) == gameBalance[4] - '0') { // jai medis
                        gameBalance = get<0>(i) + " " + to_string(get<1>(i));
                        cerr << "Balance day > 20 decision:  " << gameBalance << endl;
                        foundDecision = true;
                        break;
                        //return action;
                    }
                }
                if (!foundDecision && gameBalance != "WAIT" ) {
                    if (gameBalance != "" ){ //probably dormant
                        gameBalance ="WAIT";
                    }
                    else{
                        gameBalance ="";
                    }
                    cerr << "Balance decision else: empty in GROW" << endl;
                    //return action;
                }
            }
            else if ( gameBalance != "WAIT") {
                gameBalance ="";
            }
        }
        else if ( day > 16){
            medziuKiekis2( medis0, medis1, medis2, medis3, gameBalance);
            cerr << "Suggested gameBalance: " << gameBalance << endl;

            // arba kertam arba auginam tik medzius size 2 jai ner mySun WAIT
//            cerr << "Force action WAIT CIA1" << endl;
//            cerr << gameBalance.substr(0,4) << "game balance: " <<gameBalance <<" "<<  gameBalance[4] - '0' << " "<< medis2 << endl;
            if (gameBalance.substr(0,4) == "GROW" && 2 == gameBalance[4] - '0' && medis2 > 0){
                cerr << "Force action CIA 12" << endl;
                if (mySun < 7 && 2 == gameBalance[4] - '0'){ // not enough points
                    if (medis2 >0){
                        gameBalance = "GROW1";
                    }
                }
                if (mySun < 4) {
                    cerr << "Force action WAIT (not enought sunPoints)" << endl;
                    action = "WAIT";
                    return action;
                }
            }


            if (gameBalance.substr(0,8) == "COMPLETE"){
                bool foundDecision = false;
                string outerRing = "";
                string midleRing = "";
                string innerRing = "";
                for (int i=0; i< possible_actions.size(); i++) {
                    string a = get<0>(possible_actions[i]);
                    int b = get<1>(possible_actions[i]);
                    int c = get<2>(possible_actions[i]);
//                    cerr << "possible_action: " << a << " position: " << b << " splited if statement: "<< ("COMPLETE" == a ) <<  (getTreeSize(b == gameBalance[8] - '0')) << " if statement: " << ("COMPLETE" == a && getTreeSize(b == gameBalance[8] - '0')) << endl;
                    if ("COMPLETE" == a && getTreeSize(b) == 3) { // jai medis  gameBalance[8] - '0'
                        if(b < 7){
                            gameBalance = a + " " + to_string(b);
                            cerr << "Balance day > 20 /COMPLETE1 decision < 7: " << gameBalance << endl;
                            foundDecision = true;
                            innerRing = gameBalance;
                            //return action;
                        }
                        else if(b < 19){
                            gameBalance = a + " " + to_string(b);
                            cerr << "Balance day > 20 /COMPLETE2 decision < 19: " << gameBalance << endl;
                            foundDecision = true;
                            midleRing = gameBalance;
                            //return action;
                        }
                        else if(b >= 19){
                            gameBalance = a + " " + to_string(b);
                            cerr << "Balance day > 20 /COMPLETE3 decision >= 19: " << gameBalance << endl;
                            foundDecision = true;
                            outerRing = gameBalance;
                            //return action;
                        }
                    }
                }
                if (innerRing != "" ){ return innerRing;}
                else if (midleRing != "" ){ return midleRing;}
                else if (outerRing != "" ){ return outerRing;}
                if (!foundDecision) {
                    gameBalance ="WAIT";
                    cerr << "Balance decision else: WAIT 002" << endl;
                    //return action;
                }
            }
            else if (gameBalance.substr(0,4) == "GROW"){
                bool foundDecision = false;
                for (const auto& i : possible_actions) {
//                    cerr << "Balance get<0>(i): " << get<0>(i) << "   getTreeSize(get<1>(i)): "
//                         << getTreeSize(get<1>(i)) << "   gameBalance[4] - '0': " << (gameBalance[4] - '0') << ":"
//                         << gameBalance << endl;
                    if ("GROW" == get<0>(i) && getTreeSize(get<1>(i)) == gameBalance[4] - '0') {
                        gameBalance = get<0>(i) + " " + to_string(get<1>(i));
                        cerr << "Balance day > 20 / GROW decision: " << gameBalance << endl;
                        foundDecision = true;
                        break;
                        //return action;
                    }
                }
                if (!foundDecision){
                    for (const auto& i : possible_actions) {
//                    cerr << "Balance get<0>(i): " << get<0>(i) << "   getTreeSize(get<1>(i)): "
//                         << getTreeSize(get<1>(i)) << "   gameBalance[4] - '0': " << (gameBalance[4] - '0') << ":"
//                         << gameBalance << endl;
                        if (gameBalance[4] - '0' == 1){
                            cerr << "Balance decision else: WAIT 03" << endl;
                            return action;
                        }
                        if ("GROW" == get<0>(i) && getTreeSize(get<1>(i)) == (gameBalance[4] - '0')-1) {
                            gameBalance = get<0>(i) + " " + to_string(get<1>(i));
                            cerr << "Balance day > 20 / GROW decision: " << gameBalance << endl;
                            foundDecision = true;
                            break;
                            //return action;
                        }
                    }
                }
                if (!foundDecision) {
                    gameBalance ="WAIT";
                    cerr << "Balance decision else: WAIT 003" << endl;
                }
            }
        }

        vector<vector<int>> boardsSplitedToFour { {25,24,26,11,10,27,12,3,4,9},//1
                                                {28,13,29,14,5,30,15,31,32},//2
                                                {19,6,18,36,16,17,35,33,34},//3
                                                {23,22,9,21,2,8,20,1,7}};//4
        int a=0;
        int b=0;
        int c=0;
        int d=0;

        for (vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
            if (it->is_mine){
                for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < boardsSplitedToFour[i].size(); ++j) {
                        if (it->cell_index == boardsSplitedToFour[i][j]){
                            if (i==0){ a++; break; }
                            else if (i==1){ b++; break; }
                            else if (i==2){ c++; break; }
                            else if (i==3){ d++; break; }
                        }
                    }
                }
            }
        }
        cerr << "gameBalance: " << gameBalance<< endl;
        cerr << " a: " << a << "   b: " << b << "   c: " << c << "   d: "
             << d << endl;

        if (gameBalance.substr(0,4) == "SEED" || gameBalance == "" && mySun > 1) {
            bool foundDecision = false;
            if (a < b && a < c && a < d || a + b < c + d) { //min = a;
                for (const auto &i : possible_actions) {
                    if ("SEED" == get<0>(i)) {
                        if (find(boardsSplitedToFour[0].begin(), boardsSplitedToFour[0].end(), get<2>(i)) !=boardsSplitedToFour[0].end()) {
                            gameBalance = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i));
                            foundDecision = true;
                            break;
                        }
                    }
                }
            } else if (b < a && b < c && b < d || a + b < c + d) {//min = b;
                for (const auto &i : possible_actions) {
                    if ("SEED" == get<0>(i)) {
                        if (find(boardsSplitedToFour[1].begin(), boardsSplitedToFour[1].end(), get<2>(i)) !=boardsSplitedToFour[1].end()) {
                            gameBalance = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i));
                            foundDecision = true;
                            break;
                        }
                    }
                }
            } else if (c < a && c < b && c < d || a + d < c + b) {//min = c;
                for (const auto &i : possible_actions) {
                    if ("SEED" == get<0>(i)) {
                        if (find(boardsSplitedToFour[2].begin(), boardsSplitedToFour[2].end(), get<2>(i)) !=boardsSplitedToFour[2].end()) {
                            gameBalance = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i));
                            foundDecision = true;
                            break;
                        }
                    }
                }
            } else if (d < a && d < b && d < c || a + d < c + b) {//min = d;
                for (const auto &i : possible_actions) {
                    if ("SEED" == get<0>(i)) {
                        if (find(boardsSplitedToFour[3].begin(), boardsSplitedToFour[3].end(), get<2>(i)) !=boardsSplitedToFour[3].end()) {
                            gameBalance = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i));
                            foundDecision = true;
                            break;
                        }
                    }
                }
            }
            if(foundDecision){
                cerr << "Priority found:  " << gameBalance << endl;
            }
        }
        cerr << "gameBalance before planting algorithm: " << gameBalance<< endl;
        if (gameBalance == "WAIT"){ return gameBalance; }


        /// Tactic 1. Get to center and mike across if possible then grow all trees
        vector<Hex> tacticPlan;
        int planLenght = 10;
        int planRichest =0;
        int seedPlanRichest =0;
        string possibleAction;
        vector<Hex> tempTacticPlan;
        ///get to center
        if (day < 10) {
            if (previous_actions.size() > 0){
                // if yes we already have a plan

            } else {
                /// first move - chose tactics

                for (const auto& i : possible_actions){
                    /// chek is it way to center
                    if ("COMPLETE" != get<0>(i) && "WAIT" != get<0>(i) && possible_actions.size() > 1 ){
                        //cerr << "TEST1" << endl;
                        int myPlace = get<1>(i);
                        int destination = get<2>(i);
                        Hex destinationHex = findById(mapOfRichCells,destination);

                        tempTacticPlan.clear(); //nezinau ar reikia
                        tempTacticPlan = hex_linedraw(findById(mapOfRichCells,myPlace),findById(mapOfRichCells, 0)); //returns just hex way - without other values
                        inserNeighbours(tempTacticPlan);
                        updateDataByHex(tempTacticPlan,mapOfRichCells);
                        tempTacticPlan.erase(remove_if(tempTacticPlan.begin(), tempTacticPlan.end(), [](Hex i) { return i.richness > 2; }), tempTacticPlan.end()); // removes those what is richer than 2+ to avoid duplicates in next step
                        copy_if (mapOfRichCells.begin(), mapOfRichCells.end(), back_inserter(tempTacticPlan), [](Hex i){return i.richness > 2;} ); // add all cells of richness 3
                        // we have tempTacticPlan with data and neighbours

//                        //debug
//                        cerr << "tempTacticPlan:  size " << tempTacticPlan.size() << endl;
//                        cerr << "find by id: " << findById(mapOfRichCells,myPlace).q << " " << findById(mapOfRichCells,myPlace).r << " " << findById(mapOfRichCells,myPlace).s << "    my place: " << myPlace << endl;
//                        int tmp = tempTacticPlan[0].cell_index;
//                        cerr << " first index: " << tmp << endl;
//
//                        for (vector<Hex>::iterator it = tempTacticPlan.begin(); it != tempTacticPlan.end(); ++it) {
//                            //cerr << " count " << tempTacticPlan.size() <<  << endl;
//                            cerr << " cell_index: " << it->cell_index << "   cel Hex " << it->q << "  " << it->r << "  "
//                                 << it->s << " it->richness " << it->richness << endl;
//                        }
                        //cerr << "TEST1 GROW0 " << get<0>(i) <<endl;
                        if ("GROW" == get<0>(i)){
                            //cerr << "TEST1 GROW1,1 myPlace:size " << myPlace << ":"<< getTreeSize(myPlace) << endl;
                            if ( getTreeSize(myPlace) == 0){

                                possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
                                //cerr << "TEST1 GROW1" << endl;
                            }
                        }
                        else if ("SEED" == get<0>(i)){
                            /// check is destination in my plan
                            /// (if in line) is (possible action destination) this is our possible action
                            for (const auto& i2 : tempTacticPlan) {
                                if (equal_hex(i2, destinationHex) ){
                                    if (i2.richness > planRichest){
                                        tacticPlan = tempTacticPlan;
                                        planRichest = i2.richness ;
                                        possibleAction = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i)) ;
                                        cerr << " possibile action1: " << possibleAction  << endl;
                                    }
                                }/// else if check other possible cells of richness 3
                                //else if ()
                            }
                        }

                    }else { // just one action left it is to WAIT
                        //cerr << "TEST WAIT 1" << endl;
                        possibleAction = "WAIT";
                    }
                }
            }
        }
        else if (day >= 10 && day <= 22 ){ /// grow time
            for (const auto& i : possible_actions){
                /// chek is it way to center
                if ( "WAIT" != get<0>(i) && possible_actions.size() > 1 ){
                    possibleAction = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i)) ;
                    if ("GROW" == get<0>(i) && getTreeSize(get<1>(i)) == 0) {
                        possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
                        break;
                    }
                    else if ("COMPLETE"== get<0>(i) && getTreeSize(get<1>(i)) == 0) {
                        possibleAction = get<0>(i) + " " + to_string(get<1>(i));
                        break;
                    }
                    int myPlace = get<1>(i);
                    int destination = get<2>(i);
                    Hex destinationHex = findById(mapOfRichCells,destination); // find Hex of destination

                    tempTacticPlan.clear(); //nezinau ar reikia
                    tempTacticPlan = hex_linedraw(findById(mapOfRichCells,myPlace),findById(mapOfRichCells, 0)); //returns just hex way - without other values
                    inserNeighbours(tempTacticPlan);
                    updateDataByHex(tempTacticPlan,mapOfRichCells);
                    tempTacticPlan.erase(remove_if(tempTacticPlan.begin(), tempTacticPlan.end(), [](Hex i) { return i.richness > 2; }), tempTacticPlan.end()); // removes those what is richer than 2+ to avoid duplicates in next step
                    copy_if (mapOfRichCells.begin(), mapOfRichCells.end(), back_inserter(tempTacticPlan), [](Hex i){return i.richness > 2;} ); // add all cells of richness 3
//                        //debug
//                        cerr << "tempTacticPlan:  size " << tempTacticPlan.size() << endl;
//                        cerr << "find by id: " << findById(mapOfRichCells,myPlace).q << " " << findById(mapOfRichCells,myPlace).r << " " << findById(mapOfRichCells,myPlace).s << "    my place: " << myPlace << endl;
//                        int tmp = tempTacticPlan[0].cell_index;
//                        cerr << " first index: " << tmp << endl;
//
//                        for (vector<Hex>::iterator it = tempTacticPlan.begin(); it != tempTacticPlan.end(); ++it) {
//                            //cerr << " count " << tempTacticPlan.size() <<  << endl;
//                            cerr << " cell_index: " << it->cell_index << "   cel Hex " << it->q << "  " << it->r << "  "
//                                 << it->s << endl;
//                        }
                    if ("GROW" == get<0>(i)){
                        //cerr << "TEST1 GROW1,1 myPlace:size " << myPlace << ":"<< getTreeSize(myPlace) << endl;
                        if ( getTreeSize(myPlace) == 0){
                            planLenght = 1;
                            possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
                            break;
                            //cerr << "TEST1 GROW1" << endl;
                        } else {
                            planLenght = 1;
                            possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
                            break;
                        }
                    }
                    else if ("SEED" == get<0>(i)){
                        /// check is destination in my plan
                        /// (if in line) is (possible action destination) this is our possible action
                        for (int i2=0; i2 < tempTacticPlan.size(); i2++) {
                            //cerr << " myPlace: "<< myPlace<< "    destinatio: "<< destination <<"    tempTacticPlan_index: " << tempTacticPlan[i2].cell_index  << "    tempTacticPlan richness: " << tempTacticPlan[i2].richness << "    planRichest: " << planRichest<< "   if(): "<<(tempTacticPlan[i2].richness > planRichest) <<  endl;
                            if (equal_hex(tempTacticPlan[i2], destinationHex) ){

                                if (tempTacticPlan[i2].richness > seedPlanRichest){
                                    tacticPlan = tempTacticPlan;
                                    seedPlanRichest = tempTacticPlan[i2].richness ;
                                    //cerr<< "get<0>(i): " << get<0>(i)  << "    to_string(get<1>(i)) : " << to_string(get<1>(i))<< "    to_string(get<2>(i)) " << to_string(get<2>(i));
                                    possibleAction = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i)) ;
                                    cerr << " possibile action212: " << possibleAction  << endl;
                                    return possibleAction;
                                }
                            }/// else if check other possible cells of richness 3
                            //else if ()
                        }
                    }
                }
                else { // just one action left it is to WAIT
                    possibleAction = "WAIT";
                    //cerr << " TEST 3 " << endl;
                }
            }
        }else if (day > 22){ /// Harvest time
            for (const auto& i : possible_actions){
                /// chek is it way to center
                if ("COMPLETE" == get<0>(i)){
                    possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
                    //cerr << " TEST 2 " << endl;
                    break;
                }
                else { // just one action left it is to WAIT
                    //cerr << " TEST 1 " << endl;
                    possibleAction = "WAIT";
                }
            }
        }
//        if (day < 10) {
//            if (previous_actions.size() > 0){
//                // if yes we already have a plan
//
//            } else {
//                /// first move - chose tactics
//
//                for (const auto& i : possible_actions){
//                    /// chek is it way to center
//                    if ("COMPLETE" != get<0>(i) && "WAIT" != get<0>(i) && possible_actions.size() > 1 ){
//                        //cerr << "TEST1" << endl;
//                        int myPlace = get<1>(i);
//                        int destination = get<2>(i);
//                        Hex destinationHex = findById(mapOfRichCells,destination);
//
//                        tempTacticPlan.clear(); //nezinau ar reikia
//                        tempTacticPlan = hex_linedraw(findById(mapOfRichCells,myPlace),findById(mapOfRichCells, 0)); //returns just hex way - without other values
//                        inserNeighbours(tempTacticPlan);
//                        updateDataByHex(tempTacticPlan,mapOfRichCells);
//                        tempTacticPlan.erase(remove_if(tempTacticPlan.begin(), tempTacticPlan.end(), [](Hex i) { return i.richness > 2; }), tempTacticPlan.end()); // removes those what is richer than 2+ to avoid duplicates in next step
//                        copy_if (mapOfRichCells.begin(), mapOfRichCells.end(), back_inserter(tempTacticPlan), [](Hex i){return i.richness > 2;} ); // add all cells of richness 3
//                        // we have tempTacticPlan with data and neighbours
//
////                        //debug
////                        cerr << "tempTacticPlan:  size " << tempTacticPlan.size() << endl;
////                        cerr << "find by id: " << findById(mapOfRichCells,myPlace).q << " " << findById(mapOfRichCells,myPlace).r << " " << findById(mapOfRichCells,myPlace).s << "    my place: " << myPlace << endl;
////                        int tmp = tempTacticPlan[0].cell_index;
////                        cerr << " first index: " << tmp << endl;
////
////                        for (vector<Hex>::iterator it = tempTacticPlan.begin(); it != tempTacticPlan.end(); ++it) {
////                            //cerr << " count " << tempTacticPlan.size() <<  << endl;
////                            cerr << " cell_index: " << it->cell_index << "   cel Hex " << it->q << "  " << it->r << "  "
////                                 << it->s << " it->richness " << it->richness << endl;
////                        }
//                        //cerr << "TEST1 GROW0 " << get<0>(i) <<endl;
//                        if ("GROW" == get<0>(i)){
//                            //cerr << "TEST1 GROW1,1 myPlace:size " << myPlace << ":"<< getTreeSize(myPlace) << endl;
//                            if ( getTreeSize(myPlace) == 0){
//
//                                possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
//                                //cerr << "TEST1 GROW1" << endl;
//                            }
//                        }
//                        else if ("SEED" == get<0>(i)){
//                            /// check is destination in my plan
//                            /// (if in line) is (possible action destination) this is our possible action
//                            for (const auto& i2 : tempTacticPlan) {
//                                if (equal_hex(i2, destinationHex) ){
//                                    if (i2.richness > planRichest){
//                                        tacticPlan = tempTacticPlan;
//                                        planRichest = i2.richness ;
//                                        possibleAction = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i)) ;
//                                        cerr << " possibile action1: " << possibleAction  << endl;
//                                    }
//                                }/// else if check other possible cells of richness 3
//                                //else if ()
//                            }
//                        }
//
//                    }else { // just one action left it is to WAIT
//                        //cerr << "TEST WAIT 1" << endl;
//                        possibleAction = "WAIT";
//                    }
//                }
//            }
//        } else if (day >= 10 && day <= 22 ){ /// grow time
//            for (const auto& i : possible_actions){
//                /// chek is it way to center
//                if ( "WAIT" != get<0>(i) && possible_actions.size() > 1 ){
//                    possibleAction = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i)) ;
//                    if ("GROW" == get<0>(i) && getTreeSize(get<1>(i)) == 0) {
//                        possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
//                        break;
//                    }
//                    else if ("COMPLETE"== get<0>(i) && getTreeSize(get<1>(i)) == 0) {
//                        possibleAction = get<0>(i) + " " + to_string(get<1>(i));
//                        break;
//                    }
//                    int myPlace = get<1>(i);
//                    int destination = get<2>(i);
//                    Hex destinationHex = findById(mapOfRichCells,destination); // find Hex of destination
//
//                    tempTacticPlan.clear(); //nezinau ar reikia
//                    tempTacticPlan = hex_linedraw(findById(mapOfRichCells,myPlace),findById(mapOfRichCells, 0)); //returns just hex way - without other values
//                    inserNeighbours(tempTacticPlan);
//                    updateDataByHex(tempTacticPlan,mapOfRichCells);
//                    tempTacticPlan.erase(remove_if(tempTacticPlan.begin(), tempTacticPlan.end(), [](Hex i) { return i.richness > 2; }), tempTacticPlan.end()); // removes those what is richer than 2+ to avoid duplicates in next step
//                    copy_if (mapOfRichCells.begin(), mapOfRichCells.end(), back_inserter(tempTacticPlan), [](Hex i){return i.richness > 2;} ); // add all cells of richness 3
////                        //debug
////                        cerr << "tempTacticPlan:  size " << tempTacticPlan.size() << endl;
////                        cerr << "find by id: " << findById(mapOfRichCells,myPlace).q << " " << findById(mapOfRichCells,myPlace).r << " " << findById(mapOfRichCells,myPlace).s << "    my place: " << myPlace << endl;
////                        int tmp = tempTacticPlan[0].cell_index;
////                        cerr << " first index: " << tmp << endl;
////
////                        for (vector<Hex>::iterator it = tempTacticPlan.begin(); it != tempTacticPlan.end(); ++it) {
////                            //cerr << " count " << tempTacticPlan.size() <<  << endl;
////                            cerr << " cell_index: " << it->cell_index << "   cel Hex " << it->q << "  " << it->r << "  "
////                                 << it->s << endl;
////                        }
//                    if ("GROW" == get<0>(i)){
//                        //cerr << "TEST1 GROW1,1 myPlace:size " << myPlace << ":"<< getTreeSize(myPlace) << endl;
//                        if ( getTreeSize(myPlace) == 0){
//                            planLenght = 1;
//                            possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
//                            break;
//                            //cerr << "TEST1 GROW1" << endl;
//                        } else {
//                            planLenght = 1;
//                            possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
//                            break;
//                        }
//                    }
//                    else if ("SEED" == get<0>(i)){
//                        /// check is destination in my plan
//                        /// (if in line) is (possible action destination) this is our possible action
//                        for (int i2=0; i2 < tempTacticPlan.size(); i2++) {
//                            //cerr << " myPlace: "<< myPlace<< "    destinatio: "<< destination <<"    tempTacticPlan_index: " << tempTacticPlan[i2].cell_index  << "    tempTacticPlan richness: " << tempTacticPlan[i2].richness << "    planRichest: " << planRichest<< "   if(): "<<(tempTacticPlan[i2].richness > planRichest) <<  endl;
//                            if (equal_hex(tempTacticPlan[i2], destinationHex) ){
//
//                                if (tempTacticPlan[i2].richness > planRichest){
//                                    tacticPlan = tempTacticPlan;
//                                    planRichest = tempTacticPlan[i2].richness ;
//                                    //cerr<< "get<0>(i): " << get<0>(i)  << "    to_string(get<1>(i)) : " << to_string(get<1>(i))<< "    to_string(get<2>(i)) " << to_string(get<2>(i));
//                                    possibleAction = get<0>(i) + " " + to_string(get<1>(i)) + " " + to_string(get<2>(i)) ;
//                                    cerr << " possibile action212: " << possibleAction  << endl;
//                                }
//                            }/// else if check other possible cells of richness 3
//                            //else if ()
//                        }
//                    }
//                }
//                else { // just one action left it is to WAIT
//                    possibleAction = "WAIT";
//                    //cerr << " TEST 3 " << endl;
//                }
//            }
//        }else if (day > 22){ /// Harvest time
//            for (const auto& i : possible_actions){
//                /// chek is it way to center
//                if ("COMPLETE" == get<0>(i)){
//                    possibleAction = get<0>(i) + " " + to_string(get<1>(i)) ;
//                    //cerr << " TEST 2 " << endl;
//                    break;
//                }
//                else { // just one action left it is to WAIT
//                    //cerr << " TEST 1 " << endl;
//                    possibleAction = "WAIT";
//                }
//            }
//        }


    //-----Debug----------------------------------------------------------------------

//        for (vector<Cell>::iterator it = board.begin() ; it != board.end(); ++it) {
//            cerr << " Cell index: " << it->cell_index << " richness: " << it->richness << endl;
//            cerr << " Neighbours = index : index of neighbour " << endl;
//            for (int i=0; i< it->neighbors.size(); i++) {
//            cerr <<  i << " : " << it->neighbors[i] << endl;
//            }
//        }
//        cerr << endl;
//        cerr << '\n';
//------------

    cerr << right << setw(12) << "nutrients: " << nutrients << endl;
    cerr << right << setw(12) << "day: " << day << endl;
    cerr << right << setw(12) << "sun: " << mySun << endl;
    cerr << right << setw(12) << "score: " << score << endl;
    cerr << right << setw(12) << "oppSun: " << oppSun << " oppScore: " << oppScore << " oppIsWaiting: " << oppIsWaiting << endl;

//------------
//    cerr << " number of trees: " << trees.size() <<   endl;
//    for (vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
//        //cerr << " count " << trees.size() <<  << endl;
//        cerr << " cell_index: " << it->cell_index << "   size: " << it->size << "   Is_mine: " << it->is_mine << "   is_dormant: "
//             << it->is_dormant << endl;
//    }

//-------------
//    cerr << possible_actions.size() << endl;
//    for (const auto& i : possible_actions) {
//        cerr << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;
//    }




    // Asign final decision
        if (gameBalance != ""){
            action = gameBalance;
        }
        else {
            action = possibleAction;
        }

   // action = "WAIT";

    return action;
}
};

int main()
{
    Game game;
    game.inputInitData();

    //------------
    //


    //------------

    while (true) {
        game.inputInfo();

        cout << game.compute_next_action() << endl;
    }
}