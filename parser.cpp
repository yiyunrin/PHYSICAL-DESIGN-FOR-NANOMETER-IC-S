#include "parser.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Parser::Parser(const string& auxFile) : auxFile(auxFile) {}

void Parser::parseAux() {
    ifstream fin(auxFile);
    if (!fin) {
        cerr << "Error: Cannot open " << auxFile << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        istringstream iss(line);
        string keyword;
        iss >> keyword;  // "RowBasedPlacement"
        string fname;
        while (iss >> fname) {
            if (fname.find(".nodes") != string::npos)
                nodesFile = fname;
            else if (fname.find(".nets") != string::npos)
                netsFile = fname;
            else if (fname.find(".pl") != string::npos)
                plFile = fname;
            else if (fname.find(".scl") != string::npos)
                sclFile = fname;
        }
    }

    cout << "[AUX] nodes=" << nodesFile
         << " pl=" << plFile
         << " scl=" << sclFile << endl;
}

void Parser::parseNodes(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        string name, type;
        prec w, h;
        stringstream ss(line);
        ss >> name;
        if (name == "UCLA" || name == "NumNodes" || name == "NumTerminals") continue;

        ss >> w >> h >> type;

        NODE node;
        node.name = name;
        node.width = w;
        node.height = h;
        node.ori = "N";
        node.pos = POS(0, 0);

        if (type.empty()) node.ori = "N";
        nodes[name] = node;
    }
    cout << "[Nodes] Loaded " << nodes.size() << " nodes." << endl;
}

void Parser::parsePl(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        string name, colon, orient;
        prec x, y;
        string fixedFlag;

        stringstream ss(line);
        ss >> name >> x >> y >> colon >> orient >> fixedFlag;

        if (nodes.find(name) != nodes.end()) {
            nodes[name].pos = POS(x, y);
            if (!orient.empty()) nodes[name].ori = orient;
        }
    }
    cout << "[PL] Positions updated." << endl;
}

void Parser::parseScl(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    int y = 0, height = 0, spacing = 0, site = 0, originX = 0, numSites = 0;
    bool inRow = false;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        string key;
        stringstream ss(line);
        ss >> key;

        if (key == "CoreRow") {
            inRow = true;
        } else if (key == "End") {
            if (inRow) {
                rows.push_back(ROW(POS(originX, y), height, spacing, site));
                inRow = false;
            }
        } else if (key == "Coordinate") {
            ss >> y;
        } else if (key == "Height") {
            ss >> height;
        } else if (key == "Sitewidth") {
            ss >> site;
        } else if (key == "Sitespacing") {
            ss >> spacing;
        } else if (key == "SubrowOrigin") {
            ss >> originX >> numSites;
        }
    }

    cout << "[SCL] Loaded " << rows.size() << " rows." << endl;
}
