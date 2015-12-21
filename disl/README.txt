//////////////////////////////////////////////////////////////////////////
////////////////Dislocation Program - GUI README//////////////////////////
//////////////////////////////////////////////////////////////////////////
##############################Version 3.0#################################

Mathematical classes based on work by by G. Sheehan and further developed
by P. Young.
Image rendering, animation and GUI implementation by P. Young.
With supervision from M.I. Heggie

University of Surrey

Date: 03/10/2012

/////////////////////////////////////////////////////////////////////////


1. Purpose
The dislocation program calculates the elastic strains of dislocations in
graphite and creates an animation of the expansion of the material as the 
number of dislocations is increased.


/////////////////////////////////////////////////////////////////////////


2. Requirements
The program is written using C++ with a GUI designed using the Qt package
and libraries. Several of the image functions use Image Magick libraries 
to annotate images and create the animated gif.
Both of these packages can be downloaded for free.
Qt creator is found in the Ubuntu software centre.
Imagemagick libraries can be installed using apt-get in the command line.


//////////////////////////////////////////////////////////////////////////


3. Interface
The graphical interface allows the user to select the strain calculation 
they would like to perform; isotropic or anisotropic. The mathematical
calculation of this can be found in Hirth and Lothe, Theory of Dislocations.
The initial number of dislocations and their Burgers vector must be input,
along with the number of iterations. For each iteration another pair of
dislocations is added to the structure.
The dislocation pairs are added to the structure in any x positions but 
the y position must lie on the y position of one of the layers of graphite.
i.e there can only be set y-axis co-ordinates which will be chosen by the
program.
For the case of isotropic strains the energy of the dislocation system 
will also be calculated and displayed.


//////////////////////////////////////////////////////////////////////////


4. Input
The magnitude of the Burgers vector can be altered. Initial number of 
dislocation dipoles is required along with the number of iterations and
how many extra pairs to add each iteration. The number of iterations can
be zero.

If the randomise button is checked then the program will generate positions
with a random x co-ordinate and a quantised random y co-ordinate. 
The calculate energy checkbox will output a graph of the energy along with
the dimensional change however this will increase computational time.

The result is not related to increment size so a quick run with several
hundred or thousand pairs added per iteration will be much faster than 
serveral hundred smaller steps and gives an equivalent result.

   

/////////////////////////////////////////////////////////////////////////


5. Output
The program displays realtime rendering of the image and also has the 
capability to create an animated gif by selecting the "replay" button.
The animated gif is created by stitching together of the images of each
iteration of the material which were written to the /images file as the
program runs.


/////////////////////////////////////////////////////////////////////////


Version 1.1 updates:
Program now looks for dislocations within a radius Rc of each dislocation
and calculates the interaction of the two dislocations. Opposite sign 
dislocations with the same magnitude Burgers vector will annihillate. If 
the Burgers vectors are not equal then they will add linearly and one of 
the dislocations is removed. For same sign dislocations the Burgers 
vectors add linearly and create a single dislocation with a larger Burgers
vector.
Dislocations of burgers vector 3.32 are plotted in black, 6.64 in red and 
9.96 or higher in blue. If a dislocation gets very large the magnitude of
its Burgers vector is plotted on the map.


Version 1.2 updates:
program now contains basal dislocations as well as prismatic. The basal 
dislocations are allowed to move by calculating the force on the dislocation
due to the other dislocations and moving the dislocation in the x direction
accordingly. 

Version 1.3 updates:
When basal dislocations reach the edge of the material they are removed and 
the extra material deposited onto the edge leaving a slip step. 
Large pileups of dislocations are flagged as being a crack.
Large pileups of opposite sign dislocations on adjacent planes are now 
represented as a prismatic loops i.e "ruck and tuck".

Version 1.4 updates:
Some known bugs fixed. Previously it was noted that some prismatic dislocations
were somehow being left on their own instead of in a loop so this has now
been corrected in order to prevent excessive distorition of the material
when dislocations are left without their opposite pair.

Version 1.5 updates:
Added image dislocations to the left and right of the material which are
equal and opposite the existing dislocations and help to flatten out the 
expansion of the material so that the top is not so distorted.

Version 1.6 updates:
Prismatic dislocations are now linked in groups such that it is possible
to calculate the force on the group and move it accordingly. When interactions
occur between groups and dislocations are removed the remaining dislocations
in that group are then assigned to the other group of dislocations.
Prismatic dislocation groups are now allowed to move by conservative climb 
in such a way that if the stress on the group is greater than a thershold
value the loop can move. If the stress is not great enough then the group
will remain stationary.

Version 1.7 updates:
Images provided as repeat cells rather than simple images. i.e there are
periodic boundaries to the left and right of the origin cell and the top 
and bottom sides are free to expand as necessary.

Version 2.0 updates:
Basal dislocations are not allowed to cross over. Energy scale is converted
to J/m3. Basal and prismatic models both working. Force amendment is not
implemented as it was causing rigid kink bands to form.
Can be included by uncommenting out line 1322 in interaction.cpp and recompiling.

Version 2.1 updates:
Basals can be represented with ruck and tucks with an x tolerance (x_tol) in 
the check_for_ruck_tucks() function which represents the maximum x separation
between pileups to be considered. the variable min_pileup is used to define
how many dislocations are required to be classed as a pileup.

Version 3.0 updates:
Dislocations are allowed to be generated anywhere in the material, removed 
x_position limits - remaining from finite size version. Energy no longer
levels out. Updated interaction energies so that only half of the image-disl 
interaction is added as half of the interaction energy is attributed to the
image cell.



