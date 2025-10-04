#include "writer.h"

#include <fstream>
#include <iostream>
using namespace std;

#define PIN_SIZE 3.0

void Writer::writeChip(const string& filename, const vector<ROW>& rows) {
    if (rows.empty()) return;

    prec ymin = rows.front().pos.y;
    prec ymax = rows.back().pos.y + rows.back().height;

    prec xmin = rows.front().pos.x;
    prec xmax = rows.front().pos.x + rows.front().site * rows.front().spacing;

    ofstream fout(filename);
    fout << std::fixed << std::setprecision(3);
    fout << xmin << " " << ymin << "\n";
    fout << xmax << " " << ymin << "\n";
    fout << xmax << " " << ymax << "\n";
    fout << xmin << " " << ymax << "\n";
    fout << xmin << " " << ymin << "\n";
    fout.close();

    cout << "[EXPORT] chip -> " << filename << endl;
}

void Writer::writeCells(const string& filename, const unordered_map<string, NODE>& nodes, const std::vector<std::string>& node_names) {
    ofstream fout(filename);
    fout << std::fixed << std::setprecision(3);
    fout << "#FENCE DEFAULT" << "\n";
    fout << "0, 0\n";
    for (const string& node_name : node_names) {
        const NODE& n = nodes.at(node_name);
        prec x1 = n.pos.x;
        prec y1 = n.pos.y;
        prec x2 = x1 + n.width;
        prec y2 = y1 + n.height;

        fout << "# " << n.name << "\n";
        fout << x1 << ", " << y1 << "\n";
        fout << x2 << ", " << y1 << "\n";
        fout << x2 << ", " << y2 << "\n";
        fout << x1 << ", " << y2 << "\n";
        fout << x1 << ", " << y1 << "\n\n";
    }
    fout.close();
    cout << "[EXPORT] cells -> " << filename << endl;
}

void Writer::writePads(const string& filename, const unordered_map<string, PAD>& pads, const std::vector<std::string>& pad_names) {
    ofstream fout(filename);
    fout << std::fixed << std::setprecision(3);
    fout << "#FENCE DEFAULT" << "\n";
    fout << "0, 0\n";
    for (const string& pad_name : pad_names) {
        const PAD& pad = pads.at(pad_name);
        prec x1 = pad.pos.x;
        prec y1 = pad.pos.y;
        prec x2 = x1 + pad.width;
        prec y2 = y1 + pad.height;

        fout << "# " << pad.name << "\n";
        fout << x1 << ", " << y1 << "\n";
        fout << x2 << ", " << y1 << "\n";
        fout << x2 << ", " << y2 << "\n";
        fout << x1 << ", " << y2 << "\n";
        fout << x1 << ", " << y1 << "\n\n";

        // --- 畫 orientation arrow ---
        prec cx = (x1 + x2) / 2.0;
        prec cy = (y1 + y2) / 2.0;
        prec arrow_len = (y2 - y1) / 3.0;  // 箭頭長度，可調

        if (pad.ori == "N") {
            fout << cx - 50 << ", " << cy + arrow_len * 0.7 << "\n";
            fout << cx << ", " << y2 << "\n\n";

            fout << cx << ", " << cy << "\n";
            fout << cx << ", " << y2 << "\n\n";

            fout << cx << ", " << y2 << "\n";
            fout << cx + 50 << ", " << cy + arrow_len * 0.7 << "\n\n";
        } else if (pad.ori == "S") {
            fout << cx - 50 << ", " << cy - arrow_len * 0.7 << "\n";
            fout << cx << ", " << y1 << "\n\n";

            fout << cx << ", " << cy << "\n";
            fout << cx << ", " << y1 << "\n\n";

            fout << cx << ", " << y1 << "\n";
            fout << cx + 50 << ", " << cy - arrow_len * 0.7 << "\n\n";
        } else if (pad.ori == "E") {
            fout << cx + arrow_len * 0.7 << ", " << cy - 50 << "\n";
            fout << x2 << ", " << cy << "\n\n";

            fout << cx << ", " << cy << "\n";
            fout << x2 << ", " << cy << "\n\n";

            fout << x2 << ", " << cy << "\n";
            fout << cx + arrow_len * 0.7 << ", " << cy + 50 << "\n\n";
        } else if (pad.ori == "W") {
            fout << cx - arrow_len * 0.7 << ", " << cy - 50 << "\n";
            fout << x1 << ", " << cy << "\n\n";

            fout << cx << ", " << cy << "\n";
            fout << x1 << ", " << cy << "\n\n";

            fout << x1 << ", " << cy << "\n";
            fout << cx - arrow_len * 0.7 << ", " << cy + 50 << "\n\n";
        }
    }
    fout.close();
    cout << "[EXPORT] pads -> " << filename << endl;
}

void Writer::writePadPins(const string& filename, const unordered_map<string, NODE>& nodes, const vector<NET>& nets, const unordered_map<string, PAD>& pads) {
    ofstream fout(filename);
    fout << std::fixed << std::setprecision(3);
    if (!fout) {
        cerr << "Error: Cannot write " << filename << endl;
        return;
    }

    for (auto& net : nets) {
        for (auto& pin : net.pins) {
            auto it = pads.find(pin.node);
            if (it == pads.end()) continue;

            const PAD& p = it->second;
            prec x_center = p.pos.x + p.width / 2.0, y_center = p.pos.y + p.height / 2.0;
            prec x = x_center + pin.x_offset;
            prec y = y_center + pin.y_offset;

            fout << "#" << p.name << " " << pin.dir << "\n";
            fout << x << ", " << y << "\n";
            fout << x + PIN_SIZE << ", " << y << "\n";
            fout << x + PIN_SIZE << ", " << y + PIN_SIZE << "\n";
            fout << x << ", " << y + PIN_SIZE << "\n";
            fout << x << ", " << y << "\n\n";
        }
    }

    fout.close();
    cout << "[OUTPUT] pad pins written to " << filename << endl;
}
