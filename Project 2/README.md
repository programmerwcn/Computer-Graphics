## Compile

- enter make-build-debug dictory:  `cd cmake-build-debug/`
- Get makefile: `cmake ..`
- Compile:  `make`
- Run:  `./Project2`

## Operations

Users should use ***keyboard input*** to interact with this system.

**Please make sure** that your mouse is focused on the screen rather than command line to enable keyboard  interaction.

The initial page rasterizes the 5 polygons previously stored in *test_scene* file.

#### Step 1 -- Choose polygon

- Press 1-3 to choose which polygon to manipulate.  Press 0 to manipulate all polygons

#### Step 2 -- Manipulation

- Press T to translate the polygon, input translate size in ***x*** and ***y*** and ***z*** with command line

- Press R to rotate the polygon with respect to its centroid, input rotation angle ***theta*** and the **coordinates of p1 and p2** to define the rotation vector with command line

- Press S to scale the polygon with respect to its centroid, input scale size in ***x*** and ***y*** and ***z*** with command line

- Press F to change the input file, input file name with command line.

  **Please pay attention that all files are put under the /Project2 dictionary!** 

- Press Q to quit

- **ATTENTION**: You will be asked whether to apply BOUNDING BOX **when manipulating the polygon**. By default, the BOUNDING BOX is set to be applied.

