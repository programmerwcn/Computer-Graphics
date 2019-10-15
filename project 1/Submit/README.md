## Compile

- enter make-build-debug dictory:  `cd cmake-build-debug/`
- Compile:  `make`
- Run:  `./Project1`

## Operations

Users should use ***keyboard input*** to interact with this system.

**Please make sure** that your mouse is focused on the screen rather than command line to enable keyboard  interaction.

The initial page rasterizes the 5 polygons previously stored in *test_scene* file.

#### Step 1 -- Choose polygon

- Press 1-5 to choose which polygon to manipulate.  Press 0 to manipulate all polygons

#### Step 2 -- Manipulation

- Press D to show the outline of your polygon with DDA algorithm
- Press B to show the outline of your polygon with Bres algorithm
- Press T to translate the polygon, input translate size in ***x*** and ***y*** with command line
- Press R to rotate the polygon with respect to its centroid, input rotation angle ***theta*** with command line
- Press S to scale the polygon with respect to its centroid, input scale size in ***x*** and ***y*** with command line
- Press C to clip, input coordinates of ***min-point*** and ***max-point*** with command line
- Press Q to quit

