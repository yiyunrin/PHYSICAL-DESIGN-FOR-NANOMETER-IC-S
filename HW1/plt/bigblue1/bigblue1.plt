set nokey
set term pngcairo font 'qt'
set output 'bigblue1.png'

set size ratio -1
set xrange [0:11589]
set yrange [0:11589]
plot[:][:] '../dat/bigblue1/bigblue1_chip.dat' w l lc -1, '../dat/bigblue1/bigblue1_cell.dat' w l lc 6, '../dat/bigblue1/bigblue1_pad.dat' w l lc 4, '../dat/bigblue1/bigblue1_pad_pin.dat' w l lc -1


set title 'bigblue1, MODULE= 210904, NET= 221142, HPWL= 71226344.000, ' font 'Times, 22'

set term qt
set key

set size ratio -1
plot[:][:] '../dat/bigblue1/bigblue1_chip.dat' w l lc -1, '../dat/bigblue1/bigblue1_cell.dat' w l lc 6, '../dat/bigblue1/bigblue1_pad.dat' w l lc 4, '../dat/bigblue1/bigblue1_pad_pin.dat' w l lc -1

pause -1 'Press any key'
