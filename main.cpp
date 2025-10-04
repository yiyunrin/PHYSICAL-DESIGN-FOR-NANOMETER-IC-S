// g++ -std=c++11 -o HW1 main.cpp parser.cpp writer.cpp
// ./HW1 ../benchmarks/benchmarks/adaptec1/adaptec1.aux

#include <iostream>

#include "parser.h"
#include "writer.h"
using namespace std;

int main(int argc, char *argv[]) {
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
    parser.parseNets(path + parser.netsFile);

    cout << "=== SUMMARY ===" << endl;

    // --- 總數 ---
    cout << "Total Nodes: " << parser.nodes.size() << endl;
    cout << "Total Pads : " << parser.pads.size() << endl;
    cout << "Total Nets : " << parser.nets.size() << endl;
    cout << "Total Rows : " << parser.rows.size() << endl;
    cout << endl;

    // // --- NODE ---
    // if (!parser.nodes.empty()) {
    //     auto first = parser.nodes.begin();

    //     cout << "[NODE] First: " << first->second.name
    //          << " pos=(" << first->second.pos.x << "," << first->second.pos.y << ")"
    //          << " size=" << first->second.width << "x" << first->second.height
    //          << " ori=" << first->second.ori << endl;
    // }

    // // --- PAD ---
    // if (!parser.pads.empty()) {
    //     auto first = parser.pads.begin();

    //     cout << "[PAD] First: " << first->second.name
    //          << " pos=(" << first->second.pos.x << "," << first->second.pos.y << ")"
    //          << " size=" << first->second.width << "x" << first->second.height
    //          << " ori=" << first->second.ori
    //          << " NI=" << (first->second.NI ? "true" : "false") << endl;
    // }

    // // --- NET ---
    // if (!parser.nets.empty()) {
    //     cout << "[NET] First: " << parser.nets.front().name
    //          << " degree=" << parser.nets.front().degree << endl;
    //     for (auto &p : parser.nets.front().pins) {
    //         cout << "   pin " << p.node
    //              << " dir=" << p.dir
    //              << " offset=(" << p.x_offset << "," << p.y_offset << ")" << endl;
    //     }

    //     cout << "[NET] Last : " << parser.nets.back().name
    //          << " degree=" << parser.nets.back().degree << endl;
    //     for (auto &p : parser.nets.back().pins) {
    //         cout << "   pin " << p.node
    //              << " dir=" << p.dir
    //              << " offset=(" << p.x_offset << "," << p.y_offset << ")" << endl;
    //     }
    // }

    // // --- ROW ---
    // if (!parser.rows.empty()) {
    //     cout << "[ROW] First: y=" << parser.rows.front().pos.y
    //          << " x=" << parser.rows.front().pos.x
    //          << " height=" << parser.rows.front().height
    //          << " spacing=" << parser.rows.front().spacing
    //          << " site=" << parser.rows.front().site << endl;

    //     cout << "[ROW] Last : y=" << parser.rows.back().pos.y
    //          << " x=" << parser.rows.back().pos.x
    //          << " height=" << parser.rows.back().height
    //          << " spacing=" << parser.rows.back().spacing
    //          << " site=" << parser.rows.back().site << endl;
    // }

    Writer::writeChip("dat/adaptec1_chip.dat", parser.rows);
    Writer::writeCells("dat/adaptec1_cell.dat", parser.nodes, parser.node_names);
    Writer::writePads("dat/adaptec1_pad.dat", parser.pads, parser.pad_names);
    Writer::writePadPins("dat/adaptec1_pad_pin.dat", parser.nodes, parser.nets, parser.pads);

    return 0;
}
