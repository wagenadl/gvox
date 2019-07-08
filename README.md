GVox - Visualization of and tracing in gigavoxel volumes
========================================================

Introduction
------------

GVox is a tool for rapid visualization of isotropic CT or EM volumes
up to a few gigavoxels. GVox allows rapid panning and rotation through
a stack of up to a few thousand slices GVox is fast because it loads
the entire stack into memory. To enable a sense of depth, GVox renders
not just one thin slice through the data, but also 5 slices just in
front of the current viewing plane and 5 slices just behind it. It
tints these “barely out of focus” slices green and purple so that
users can intuitively visualize the 3D structure of a volume.

Beyond visualization, GVox can also be used to annotate volumes,
particularly to trace the processes of neurons through the volume.

GVox can load stacks of jpegs or (8-bit) tiffs and can save text files
with the coordinates of annotations.

Installation
------------

GVox was developed on Ubuntu Linux using the Qt libraries
(https://www.qt.io). It should compile on Windows and Mac OS as well,
provided the Qt libraries are available. On Ubuntu Linux, it is
easiest to install Qt using “apt”:

	sudo apt install qt5-qmake qtbase5-dev

On other systems, the downloads at https://www.qt.io/downloads may be
more convenient.

Once Qt is installed, you can compile GVox either from within the Qt
environment (open “gvox.pro” and click “Build” and “Run”), or, on
Linux or Mac OS, by opening a terminal in the “src” folder and typing
“make”. You can then copy the “gvox” executable to any convenient
location, e.g., $HOME/bin.

Running GVox
------------

GVox is started from a terminal (shell) window like this:

    gvox /path/to/data/images

where */path/to/data/images* is the name of a folder that contains a
stack of jpeg or tiff files corresponding to the raw data. GVox
imports the raw data and produces a .json file with metadata, a .data
file with the image data as a single file, and a .id-rle file with
annotation data. These files are located in the parent folder of the
source data folder (i.e., in */path/to/data* in the example
above). The next time around, start gvox like this:

    gvox /path/to/data/images.json

This will load the previously generated metadata and startup will be
much faster.

The metadata is user editable, and you may, in particular, want to
change the axis labeling that is specified in items named
“label-xpositive”, “label-ynegative”, etc. (The items named
“label-xp”, etc., are meant to be single-letter abbreviations of the
axis labels.)

Navigation
----------

Panning through the volume is done by shift + dragging the mouse for
movement in the plane parallel to the screen and by shift + scrolling
the mouse for movement in depth. Rotation in 3D is done by control +
dragging the mouse. Rotation in the current 2D plane is done by
control + shift + dragging the mouse.
Zooming in and out is done by control + scrolling the mouse.

Annotation
----------

Hit “Enter” to start a new annotation object. Draw with the left mouse
button. Double click to conveniently create a larger sphere.  Hit
“Backspace” or “Delete” to switch to erase mode. In erase mode,
drawing with the left mouse button erases previous annotations. Right
click on an existing annotation object to draw more of that
object. Alternatively, type the ID number of an existing object and
“G" to center the view on that object and continue drawing it. For
instance, typing the three keys “1”, “7”, “G” would select object
#17. In a similar vein, “1”, “7”, “D” erases object #17 entirely.

Alternative visualization
-------------------------

Type “T” to view all traced objects in a separate window. “1”, “T”
through “6”, “T” specifies which aspect to visualize this
way. Similarly, type “O” to view traced objects overlaid on the image
data, and again, “1”, “O” through “6”, “O” to specify an aspect.

Exporting data
--------------

Type “E” to export the annotation data to a text file. The file will
be called */path/to/data/images.txt*. Each line corresponds to one
pixel that you drew. There are four numbers on each line: the X, Y,
and Z coordinates of the pixel, and the ID number of the object the
pixel belongs to.

Conclusion
----------

If you use this software to analyze your data, please mention it in
resulting publications. Please contact me (daw@caltech.edu) for
appropriate citation information.

If you find bugs, or have suggestions for improvement, please also
contact me.