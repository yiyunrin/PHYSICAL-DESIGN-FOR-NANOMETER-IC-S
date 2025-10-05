set nokey
set term pngcairo font 'qt'
set output 'bigblue4.png'

set size ratio -1
set xrange [0:32223]
set yrange [0:32386]
plot[:][:] '../dat/bigblue4/bigblue4_chip.dat' w l lc -1, '../dat/bigblue4/bigblue4_cell.dat' w l lc 6, '../dat/bigblue4/bigblue4_pad.dat' w l lc 4, '../dat/bigblue4/bigblue4_pad_pin.dat' w l lc -1


set title 'bigblue4, MODULE= 210904, NET= 221142, HPWL= 71226344.000, ' font 'Times, 22'

set term qt
set key

set size ratio -1
plot[:][:] '../dat/bigblue4/bigblue4_chip.dat' w l lc -1, '../dat/bigblue4/bigblue4_cell.dat' w l lc 6, '../dat/bigblue4/bigblue4_pad.dat' w l lc 4, '../dat/bigblue4/bigblue4_pad_pin.dat' w l lc -1

pause -1 'Press any key'
