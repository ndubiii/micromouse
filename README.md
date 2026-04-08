# micromouse
A micromouse competition repository

The repository has program files divided into the following modules; control, drivers, main and navigation. Detailed information about the modules is provided below;

1. Control
-Motion; Template for the motion control of the micromouse.
-pid; Template for the pid control logic to minimize errors
-wall follow; Template for the wall following code to enable the micromouse to navigate the maze without bumping into walls.

2. Drivers
- encoder; Template for encoder action
- imu; Template for for the imu sensor action
- ir sensors; Template for the 2 ir sensors to be used
- motor driver; Template for the motor control action
- ultrasonic; Template for the ultrasonic sensor to be used in conjunction with the ir sensors

3. Main
- main; The main algorithm to run all the files

4. Navigation
- Flood fill; Template for the flood fill algorithm 
- Maze; Template for the maze structure

Participants are advised to look at both the .h and .cpp files. Detailed information about the code to be added to the templates have been provided in the specific files. Participants should choose the modules they are going to work on.