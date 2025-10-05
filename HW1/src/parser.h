#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "datatype.h"

class Parser {
   public:
    Parser(const std::string& auxFile);

    void parseAux();
    void parseNodes(const std::string& filename);
    void parsePl(const std::string& filename);
    void parseScl(const std::string& filename);
    void parseNets(const std::string& filename);

    int NumNodes, NumTerminals, NumRows, NumNets, NumPins;
    std::vector<std::string> node_names, pad_names;
    std::unordered_map<std::string, NODE> nodes;
    std::unordered_map<std::string, PAD> pads;
    std::vector<ROW> rows;
    std::vector<NET> nets;

    std::string nodesFile, netsFile, plFile, sclFile;

   private:
    std::string auxFile;
};

#endif
