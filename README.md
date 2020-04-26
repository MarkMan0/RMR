# RMR

Application for control and SLAM of KOBUKI robot. The application was primarily developed using a simulator, but some of the functionalities have also been tested on the real robot. 

## Core features
+ Absolute and relative move commands - tested and verified on the physical robot
+ Processing and filtration of data coming from RPLIDAR
+ Dynamic mapping of an unknown world, with no hardcoded size limits
+ Implementation if Dijkstra's path finding algorithm, with route simplification
+ GUI in which you can control the robot in multipe ways
+ Representation of the mapped area, map used for path finding and the path itself

## Feature description

### Odometry

We are utilising the MEMS gyroscope on the robot to keep track of the absolute orientation in the world. While testing on the physical robot, we've used a combination of classic odometry for linear movements, and odometry for arc movements. Using this we've achieved only minor errors when moving along a square with side length of 2m.

### Motion control

After receiving the target points, the robot turns towards the target - using a PD rotation controller. After this it moves along an arc, which is controlled by a pair of PD controllers for linear and angular speed. The motion is considered done, when the error is smaller than a predified value, for a given amount of time.

### LIDAR mapping

The incoming data are transformed into the global(x,y) frame, using linear transformations. The data is stored in a std::map, where the key is of type Point, and value is integer. A filtration process is executed periodically: When a point on the map has a lower value than a certain threshold, and all it's neighbors are non-blocked, it's value is set to 0.

The mapping is done by issuing a series of motion commands by the user. The mapping path for the default area is included in the application(button *Discover*)

### Path finding

Path finding is done using Dijkstra's algorithm. Before connecting the nodes, the map is dilated. In this process, each node, which has a blocked neighbor, is also set blocked. This is done to ensure that the robot doesn't crash into any of the walls. After this, each node is connected to a maximum of 8, non-blocked neighbors. The result of the algorithm is a dense set of points. The path is simplified, firstly by removing every second node on the path, then by removing the nodes, where there is no change in direction. The result is a greatly simplified set of points, and by connecting these points, we can reconstruct the path. 

## Conclusion
