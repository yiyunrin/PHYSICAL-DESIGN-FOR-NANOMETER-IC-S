# HW1 README
## 請先確認目前終端機位置位於 "HW1_P76144265資料夾" 中
若尚未進入資料夾，請執行下面指令
```bash
cd HW1_P76144265
```
## 環境
- 作業系統：Linux
- 編譯器：g++ 6.3.0 或以上
- 工具：gnuplot（繪圖）

## Makefile使用方法
有下面三條指令可以使用，功能分別是
```bash
make #編譯專案，產生執行檔
make run #執行程式
make clean #清除產生的檔案
```

輸入"make"即可編譯專案，編譯後輸入"make run"即可執行程式。

```bash
# 編譯並執行
make
make run
```
執行結束後，會在 **"dat資料夾"** 中產生 adaptec1~4 和 bigblue1~4 的所有".dat檔"。

## gnuplot
在 **"plt資料夾"** 中，有寫好的 ".plt檔"。進入此資料夾後，依序輸入下面指令，即可畫出需要的圖片。
```bash
cd plt/adaptec1
gnuplot adaptec1.plt
cd ../adaptec2
gnuplot adaptec2.plt
cd ../adaptec3
gnuplot adaptec3.plt
cd ../adaptec4
gnuplot adaptec4.plt
cd ../bigblue1
gnuplot bigblue1.plt
cd ../bigblue2
gnuplot bigblue2.plt
cd ../bigblue3
gnuplot bigblue3.plt
cd ../bigblue4
gnuplot bigblue4.plt
```
也可以直接使用Makefile來畫全部的圖，指令如下
```bash
cd plt
make plot
```
執行後會在對應資料夾下（如 plt/adaptec1/）生成 .png 圖檔。

## 輸出結果
執行完成後，整體資料夾結構如下
```bash
HW1_P76144265/
├── src/              # 所有 .cpp 和 .h 程式碼
├── dat/              # 輸出的 .dat 檔案
├── plt/              # gnuplot 腳本與圖片
├── Makefile          # 編譯用
├── HW1_P76144265     # 執行檔
└── README.md         # 本文件
```
