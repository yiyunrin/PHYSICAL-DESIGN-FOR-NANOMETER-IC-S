set nokey
set term pngcairo font 'qt'
set output 'bigblue2.png'

set size ratio -1
set xrange [0:18721]
set yrange [0:18796]
plot[:][:] '../dat/bigblue2/bigblue2_chip.dat' w l lc -1, '../dat/bigblue2/bigblue2_cell.dat' w l lc 6, '../dat/bigblue2/bigblue2_pad.dat' w l lc 4, '../dat/bigblue2/bigblue2_pad_pin.dat' w l lc -1


set title 'bigblue2, MODULE= 210904, NET= 221142, HPWL= 71226344.000, ' font 'Times, 22'

set term qt
set key

set size ratio -1
plot[:][:] '../dat/bigblue2/bigblue2_chip.dat' w l lc -1, '../dat/bigblue2/bigblue2_cell.dat' w l lc 6, '../dat/bigblue2/bigblue2_pad.dat' w l lc 4, '../dat/bigblue2/bigblue2_pad_pin.dat' w l lc -1

pause -1 'Press any key'
