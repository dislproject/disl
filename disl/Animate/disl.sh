#/bin/bash
# mencoder command for outputting
# movie from bmp files

#mencoder "mf://images/bmp*.bmp" -mf fps=10 -o "disl.mpg" -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=800

#mencoder "mf://images/graph*.bmp" -mf fps=10 -o "dislgraph.mpg" -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=800


#convert -delay 10 -loop 0 images/bmp*.bmp disl.gif
#convert -delay 10 -loop 0 images/graph*.bmp graph.gif


mencoder "mf://images/image*.bmp" -mf fps=10 -o "animation.mpg" -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=800


convert -delay 10 -loop 0 images/image*.bmp animation.gif