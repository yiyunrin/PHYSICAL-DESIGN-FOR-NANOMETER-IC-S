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
        iss >> keyword;
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

        string name, type, tmp;
        prec w, h, num;
        stringstream ss(line);
        ss >> name;
        if (name == "NumNodes") {
            ss >> tmp >> num;
            NumNodes = num;
            continue;
        } else if (name == "NumTerminals") {
            ss >> tmp >> num;
            NumTerminals = num;
            continue;
        } else if (name == "UCLA")
            continue;

        ss >> w >> h >> type;

        NODE node;
        node.name = name;
        node.width = w;
        node.height = h;
        node.ori = "N";
        node.pos = POS(0, 0);  // ?

        if (!type.empty()) {
            PAD pad = PAD(node, type == "terminal_NI");
            pads[name] = pad;
            pad_names.push_back(name);
            // cout << "pad: " << name << (type == "terminal_NI" ? " terminal_NI" : " terminal") << endl;
        } else {
            nodes[name] = node;
            node_names.push_back(name);
            // cout << "node: " << name << endl;
        }
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
        string movetype;

        stringstream ss(line);
        ss >> name >> x >> y >> colon >> orient >> movetype;
        if (name == "UCLA") continue;

        if (movetype.empty() && nodes.find(name) != nodes.end()) {
            nodes[name].pos = POS(x, y);
            if (!orient.empty()) nodes[name].ori = orient;
        } else if (pads.find(name) != pads.end()) {
            pads[name].pos = POS(x, y);
            if (!orient.empty()) pads[name].ori = orient;
            if ((movetype == "/FIXED_NI" && !pads[name].NI) || (movetype == "/FIXED" && pads[name].NI))
                cout << name << "fixed error" << endl;
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
    int y = 0, height = 0, spacing = 0, x = 0, numSites = 0;
    string tmp;
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
                rows.push_back(ROW(POS(x, y), height, spacing, numSites));
                inRow = false;
            }
        } else if (key == "Coordinate") {
            ss >> tmp >> y;
        } else if (key == "Height") {
            ss >> tmp >> height;
        } else if (key == "Sitespacing") {
            ss >> tmp >> spacing;
        } else if (key == "SubrowOrigin") {
            ss >> tmp >> x >> tmp >> tmp >> numSites;
        }
    }

    cout << "[SCL] Loaded " << rows.size() << " rows." << endl;
}

void Parser::parseNets(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line, tmp;
    NET currentNet;
    bool inNet = false;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string key;
        ss >> key;

        if (key == "UCLA") {
            continue;
        } else if (key == "NumNets") {
            ss >> tmp >> NumNets;
            continue;
        } else if (key == "NumPins") {
            ss >> tmp >> NumPins;
            continue;
        } else if (key == "NetDegree") {
            if (inNet) {
                nets.push_back(currentNet);
                currentNet = NET();
            }

            int deg;
            string netName;
            ss >> tmp >> deg >> netName;

            currentNet = NET(deg, netName);
            inNet = true;
        } else {
            string dir;
            prec xOff = 0, yOff = 0;
            ss >> dir >> tmp;
            if (!(ss >> xOff >> yOff)) {
                xOff = 0;
                yOff = 0;
            }

            PIN pin(key, dir, xOff, yOff);
            currentNet.pins.push_back(pin);
        }
    }

    if (inNet) {
        nets.push_back(currentNet);
    }

    cout << "[NETS] Loaded " << nets.size() << " nets." << endl;
}
