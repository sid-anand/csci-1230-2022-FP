# CSCI 1230 Final Project (Team Name: Just Phong-Blinn Around)

## How to Use
After running the project in QT Creator, you will be placed in the middle of the procedural city. 

You can use the following controls to move around (same as the Realtime projects):
* W: Translates the camera in the direction of the look vector
* S: Translates the camera in the opposite direction of the look vector
* A: Translates the camera in the left direction, perpendicular to the look and up vectors
* D: Translates the camera in the right direction, also perpendicular to the look and up vectors. This movement should be opposite to that of pressing A
* Space: Translates the camera along the world space vector `(0, 1, 0)`
* Ctrl: Translates the camera along the world space vector `(0, -1, 0)`
* Mouse Dragging: Rotates the camera

We also added some new controls specific to this project:
* G: Holding it down causes the camera to follow a smooth tour of the city based on Bezier curves
* Slider labeled "Day to Night": Sliding it from left to right smoothly transitions the time of day from day to night
* Slider labeled "Max Building Height": Sliding it from left to right increases the **maximum** building height and regenerates the floor plan

## Known Bugs
Increasing the maximum building height increases the probability that the Bezier camera path will collide with a building upon turning and proceed to go through it, rather than navigating around it.

## Third-Party Libraries
None.
