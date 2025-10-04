#include "writer.h"

#include <fstream>
#include <iostream>
using namespace std;

#define PIN_SIZE 3.0

void Writer::writeChip(const string& filename, const vector<ROW>& rows, const unordered_map<string, PAD>& pads) {
    ofstream fout(filename);
    fout << fixed << setprecision(3);
    if (!fout) {
        cerr << "Error: Cannot write " << filename << endl;
        return;
    }

    // --- Core region ---
    prec core_x_min = 1e9, core_y_min = 1e9;
    prec core_x_max = -1e9, core_y_max = -1e9;

    for (auto& r : rows) {
        core_x_min = min(core_x_min, r.pos.x);
        core_y_min = min(core_y_min, r.pos.y);
        core_x_max = max(core_x_max, r.pos.x + r.site);
        core_y_max = max(core_y_max, r.pos.y + r.height);
    }

    fout << "# core region\n";
    fout << core_x_min << ", " << core_y_min << "\n";
    fout << core_x_max << ", " << core_y_min << "\n";
    fout << core_x_max << ", " << core_y_max << "\n";
    fout << core_x_min << ", " << core_y_max << "\n";
    fout << core_x_min << ", " << core_y_min << "\n\n";

    // --- Die area ---
    prec die_x_min = 1e9, die_y_min = 1e9;
    prec die_x_max = -1e9, die_y_max = -1e9;

    for (auto& kv : pads) {
        const PAD& p = kv.second;
        prec x0 = p.pos.x;
        prec y0 = p.pos.y;
        prec x1 = x0 + p.width;
        prec y1 = y0 + p.height;

        die_x_min = 0;  // min(die_x_min, x0);
        die_y_min = 0;  // min(die_y_min, y0);
        die_x_max = max(die_x_max, x1);
        die_y_max = max(die_y_max, y1);
    }

    fout << "# die area\n";
    fout << die_x_min << ", " << die_y_min << "\n";
    fout << die_x_max << ", " << die_y_min << "\n";
    fout << die_x_max << ", " << die_y_max << "\n";
    fout << die_x_min << ", " << die_y_max << "\n";
    fout << die_x_min << ", " << die_y_min << "\n";

    fout.close();
    cout << "[OUTPUT] chip (core + die) written to " << filename << endl;
}

void Writer::writeCells(const string& filename, const unordered_map<string, NODE>& nodes, const std::vector<std::string>& node_names) {
    ofstream fout(filename);
    fout << std::fixed << std::setprecision(3);
    fout << "#FENCE DEFAULT" << "\n";
    fout << "0, 0\n\n";
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

void rotate90(vector<POS>& pts, prec cx, prec cy) {
    for (auto& p : pts) {
        prec dx = p.x - cx;
        prec dy = p.y - cy;
        prec newX = cx - dy;
        prec newY = cy + dx;
        p.x = newX;
        p.y = newY;
    }
}

// 沿 Y 軸翻轉 (基準點 cx,cy)
void flipY(vector<POS>& pts, prec cx, prec cy) {
    for (auto& p : pts) {
        prec dx = p.x - cx;
        p.x = cx - dx;
    }
}
// 沿 X 軸翻轉 (基準點 cx, cy)
void flipX(vector<POS>& pts, prec cx, prec cy) {
    for (auto& p : pts) {
        prec dy = p.y - cy;
        p.y = cy - dy;
    }
}

void Writer::writePads(const string& filename, const unordered_map<string, PAD>& pads, const vector<string>& pad_names) {
    ofstream fout(filename);
    fout << fixed << setprecision(3);
    fout << "#FENCE DEFAULT" << "\n";
    fout << "0, 0\n\n";
    if (!fout) {
        cerr << "Error: Cannot write " << filename << endl;
        return;
    }

    for (auto& pname : pad_names) {
        auto it = pads.find(pname);
        if (it == pads.end()) continue;
        const PAD& pad = it->second;

        prec x = pad.pos.x;
        prec y = pad.pos.y;
        prec w = pad.width;
        prec h = pad.height;

        // 1. pad 矩形點
        vector<POS> pts = {
            POS(x, y),
            POS(x + w, y),
            POS(x + w, y + h),
            POS(x, y + h),
            POS(x, y)  // close loop
        };

        // 2. 箭頭點 (初始為 N 方向)
        prec arr_w = w / 8.0, arr_h = h / 3.0;
        prec arr_x = x + arr_w * 6.0, arr_y = y + arr_h * 2.0;
        vector<POS> arrow = {
            POS(arr_x, arr_y + arr_h),
            POS(arr_x, arr_y),
            POS(arr_x - arr_w, arr_y + arr_h / 2.0),
            POS(arr_x + arr_w, arr_y + arr_h / 2.0)};

        // 3. 根據方向旋轉/翻轉
        if (pad.ori == "N") {
            // 不動
        } else if (pad.ori == "W") {
            rotate90(pts, x, y);
            rotate90(arrow, x, y);
        } else if (pad.ori == "S") {
            rotate90(pts, x, y);
            rotate90(pts, x, y);
            rotate90(arrow, x, y);
            rotate90(arrow, x, y);
        } else if (pad.ori == "E") {
            rotate90(pts, x, y);
            rotate90(pts, x, y);
            rotate90(pts, x, y);
            rotate90(arrow, x, y);
            rotate90(arrow, x, y);
            rotate90(arrow, x, y);
        } else if (pad.ori == "FN") {
            flipY(pts, x, y);
            flipY(arrow, x, y);
        } else if (pad.ori == "FW") {
            flipX(pts, x, y);
            flipX(arrow, x, y);
            rotate90(pts, x, y);
            rotate90(arrow, x, y);
        } else if (pad.ori == "FS") {
            flipX(pts, x, y);
            flipX(arrow, x, y);
        } else if (pad.ori == "FE") {
            flipY(pts, x, y);
            flipY(arrow, x, y);
            rotate90(pts, x, y);
            rotate90(arrow, x, y);
        }

        // 4. 輸出矩形
        fout << "#" << pad.name << " ori=" << pad.ori << "\n";
        for (auto& p : pts) {
            fout << p.x << ", " << p.y << "\n";
        }
        fout << "\n";

        // 5. 輸出箭頭
        for (size_t i = 1; i < arrow.size(); i++) {
            fout << arrow[0].x << ", " << arrow[0].y << "\n";
            fout << arrow[i].x << ", " << arrow[i].y << "\n\n";
        }
        // close loop
    }

    fout.close();
    cout << "[OUTPUT] pads written to " << filename << endl;
}

void Writer::writePadPins(const string& filename, const unordered_map<string, NODE>& nodes, const vector<NET>& nets, const unordered_map<string, PAD>& pads) {
    ofstream fout(filename);
    fout << std::fixed << std::setprecision(3);
    if (!fout) {
        cerr << "Error: Cannot write " << filename << endl;
        return;
    }
    fout << "#FENCE DEFAULT" << "\n";
    fout << "0, 0\n\n";
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
