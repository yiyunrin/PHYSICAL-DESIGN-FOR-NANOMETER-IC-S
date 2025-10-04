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

    // 存資料
    int NumNodes, NumTerminals, NumRows, NumNets, NumPins;
    std::map<std::string, NODE> nodes;
    std::map<std::string, PAD> pads;
    std::vector<ROW> rows;
    std::vector<NET> nets;

    // 檔名
    std::string nodesFile, netsFile, plFile, sclFile;

   private:
    std::string auxFile;
};

#endif
