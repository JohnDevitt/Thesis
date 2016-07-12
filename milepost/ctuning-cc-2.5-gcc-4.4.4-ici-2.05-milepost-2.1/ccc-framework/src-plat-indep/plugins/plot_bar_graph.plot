set output 'fig_bar_graph.eps'
set terminal postscript eps color size 6in,3.5in "Helvetica" 16

set datafile missing '-'

set style data histogram
set style histogram cluster gap 2
#set style histogram cluster gap 2

set grid noxtics ytics

set rmargin 5

set ylabel 'Execution time speedup'

set xtic rotate by -60

plot 'fig_bar_graph.data' using 2:xticlabels(1) title 'AMD' fs solid 0 ls -1, \
     '' using 3 title 'Intel' fs solid 0.3 ls 2, \
     '' using 4 title 'ARC' fs solid 0.6 ls 3

