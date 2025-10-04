#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "parser.h"

class Writer {
   public:
    static void writeChip(const std::string& filename, const std::vector<ROW>& rows);
    static void writeCells(const std::string& filename, const std::unordered_map<std::string, NODE>& nodes, const std::vector<std::string>& node_names);
    static void writePads(const std::string& filename, const std::unordered_map<std::string, PAD>& pads, const std::vector<std::string>& pad_names);
    static void writePadPins(const string& filename, const unordered_map<string, NODE>& nodes, const vector<NET>& nets, const std::unordered_map<std::string, PAD>& pads);
};
#endif
