#command to convert all bmp files to an
#animated gif and then remove bmp files.

convert -delay 10 -loop 0 images/bmp*.bmp disl.gif
convert -delay 10 -loop 0 images/graph*.bmp graph.gif

#./disl.gif

#rm *.bmp