#Script to create animation of graph and material side by side
#edit i and j to represent the number of iterations required.

bash

for i in {100..130}; 
do for j in {0..5};
echo $i;
echo $j; 
montage -tile 2x1 -geometry +0+0 images/bmp$i\_$j.bmp images/graph$i.bmp images/image$i\_$j.bmp;
done
done

mencoder "mf://images/image*.bmp" -mf fps=10 -o "animation1.mpg" -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=800

convert -delay 10 -loop 0 images/image*.bmp animation1.gif