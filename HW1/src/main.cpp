#include <sys/stat.h>
#include <sys/types.h>

#include <cstdlib>
#include <iostream>

#include "parser.h"
#include "writer.h"
using namespace std;

bool dirExists(const string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return (info.st_mode & S_IFDIR);
}

void makeDir(const string &path) {
    if (!dirExists(path)) {
#ifdef _WIN32
        _mkdir(path.c_str());
#else
        mkdir(path.c_str(), 0755);
#endif
    }
}

int main(int argc, char *argv[]) {
    string basePath = "../benchmarks/benchmarks/";
    vector<string> circuits = {
        "adaptec1", "adaptec2", "adaptec3", "adaptec4",
        "bigblue1", "bigblue2", "bigblue3", "bigblue4"};

    makeDir("dat");

    for (auto &ckt : circuits) {
        cout << "==============================" << endl;
        cout << "Processing " << ckt << " ..." << endl;

        string cktPath = basePath + ckt + "/";
        string auxPath = cktPath + ckt + ".aux";
        string outDir = "dat/" + ckt;

        makeDir(outDir);

        FILE *f = fopen(auxPath.c_str(), "r");
        if (!f) {
            cerr << "Warning: " << auxPath << " not found, skip." << endl;
            continue;
        }
        fclose(f);

        Parser parser(auxPath);
        parser.parseAux();
        parser.parseNodes(cktPath + parser.nodesFile);
        parser.parsePl(cktPath + parser.plFile);
        parser.parseScl(cktPath + parser.sclFile);
        parser.parseNets(cktPath + parser.netsFile);

        cout << "=== SUMMARY ===" << endl;
        cout << "Total Nodes: " << parser.nodes.size() << endl;
        cout << "Total Pads : " << parser.pads.size() << endl;
        cout << "Total Nets : " << parser.nets.size() << endl;
        cout << "Total Rows : " << parser.rows.size() << endl;
        cout << endl;

        Writer::writeChip(outDir + "/" + ckt + "_chip.dat", parser.rows, parser.pads);
        Writer::writeCells(outDir + "/" + ckt + "_cell.dat", parser.nodes, parser.node_names);
        Writer::writePads(outDir + "/" + ckt + "_pad.dat", parser.pads, parser.pad_names);
        Writer::writePadPins(outDir + "/" + ckt + "_pad_pin.dat", parser.nodes, parser.nets, parser.pads);

        cout << "Finished " << ckt << endl;
        cout << "Output written to " << outDir << "/" << endl;
        cout << "==============================" << endl
             << endl;
    }

    return 0;
}
