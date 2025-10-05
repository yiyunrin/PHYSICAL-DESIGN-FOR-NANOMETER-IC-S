#include <bits/stdc++.h>
using namespace std;

typedef double prec;
class NODE;

class POS;
class PAD;
class NET;
class PIN;
class ROW;

class POS {
   public:
    prec x;
    prec y;

    POS() : x(0), y(0) {}
    POS(prec _x, prec _y) : x(_x), y(_y) {}
    POS(const POS &pos) : x(pos.x), y(pos.y) {}
    ~POS() = default;
};

class NODE {
   public:
    string name;
    prec width;
    prec height;
    POS pos;
    string ori;

    NODE() : name(""), width(0), height(0), pos(POS()), ori("N") {}
    NODE(string _name, prec _w, prec _h, POS _pos, string _ori) : name(_name), width(_w), height(_h), pos(_pos), ori(_ori) {}
    NODE(const NODE &node) : name(node.name), width(node.width), height(node.height), pos(node.pos), ori(node.ori) {}
    ~NODE() = default;
};

class PAD : public NODE {
   public:
    bool NI;

    PAD() : NODE(), NI(false) {}
    PAD(string _name, prec _w, prec _h, POS _pos, string _ori, bool _NI) : NODE(_name, _w, _h, _pos, _ori), NI(_NI) {}
    PAD(const NODE &node, bool _NI) : NODE(node), NI(_NI) {}
    PAD(const PAD &pad) : NODE(pad.name, pad.width, pad.height, pad.pos, pad.ori), NI(pad.NI) {}
    ~PAD() = default;
};

class NET {
   public:
    int degree;
    string name;
    vector<PIN> pins;

    NET() : degree(0), name(""), pins(vector<PIN>()) {}
    NET(int _deg, string _name) : degree(_deg), name(_name), pins(vector<PIN>()) {}
    NET(int _deg, string _name, vector<PIN> _pins) : degree(_deg), name(_name), pins(_pins) {}
    NET(const NET &net) : degree(net.degree), name(net.name), pins(net.pins) {}
    ~NET() = default;
};

class PIN {
   public:
    string node;
    string dir;
    prec x_offset;
    prec y_offset;

    PIN() : node(""), dir(""), x_offset(0), y_offset(0) {}
    PIN(string _node, string _dir, prec _x_offset, prec _y_offset) : node(_node), dir(_dir), x_offset(_x_offset), y_offset(_y_offset) {}
    PIN(const PIN &pin) : node(pin.node), dir(pin.dir), x_offset(pin.x_offset), y_offset(pin.y_offset) {}
    ~PIN() = default;
};

class ROW {
   public:
    POS pos;
    prec height;
    prec spacing;
    prec site;

    ROW() : pos(POS()), height(0), spacing(0), site(0) {}
    ROW(POS _pos, prec _h, prec _spacing, prec _site) : pos(_pos), height(_h), spacing(_spacing), site(_site) {}
    ROW(const ROW &row) : pos(row.pos), height(row.height), spacing(row.spacing), site(row.site) {}
    ~ROW() = default;
};
