#include <iostream>

#include "parser.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./HW1 <circuit.aux>" << endl;
        return 1;
    }

    string path = "../benchmarks/benchmarks/adaptec1/";

    Parser parser(argv[1]);
    parser.parseAux();
    parser.parseNodes(path + parser.nodesFile);
    parser.parsePl(path + parser.plFile);
    parser.parseScl(path + parser.sclFile);

    // 測試印出一個 node 和 row
    if (!parser.nodes.empty()) {
        auto it = parser.nodes.begin();
        cout << "First node: " << it->second.name
             << " pos=(" << it->second.pos.x << "," << it->second.pos.y << ")"
             << " size=" << it->second.width << "x" << it->second.height
             << " ori=" << it->second.ori << endl;
    }

    if (!parser.rows.empty()) {
        cout << "First row y=" << parser.rows[0].pos.y
             << " height=" << parser.rows[0].height
             << " spacing=" << parser.rows[0].spacing
             << " site=" << parser.rows[0].site << endl;
    }

    return 0;
}
