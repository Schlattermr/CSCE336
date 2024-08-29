# Motors Library README
## Matthew Schlatter

### danceRobot
The danceRobot function has the obot move forward, backwards, turn left 45 degrees, 
turn right 45 degrees, turn left 90 degrees, and then turn right 90 degrees.

### robotForward
The robotForward function moves the robot forward by setting IN1 and IN3 to HIGH 
and IN2 and IN4 to LOW.

### robotBackwards
The robotBackwards function moves the robot backwards by flipping these bits.

### turnLeft
The turnLeft function turns the robot left by setting IN2 and IN3 to HIGH and IN1 and 
IN4 to LOW so the left wheel goes backwards and the right wheel goes forward. The length 
of the turn can be changed by how long of a delay is set after calling this function 
before stopping the robot or calling another movement function.

### turnRight
The turnRight function turns the robot right by flipping the turnLeft bits so the left 
wheel goes forward and the right wheel goes backwards. The length of the turn can be 
changed by how long of a delay is set after calling this function before stopping the 
robot or calling another movement function.

### robotStop
The robotStop function stops the robot bu during the enable bits off and setting the 
output compare registers to zero. 

### wallFollowing
The wallFollowing function is an algorithm to follow the wall and adjust depending on 
the distance from the wall, which is determined from the ultrasonic sensor. If the robot 
is getting too far away, the speed of the right motor is increased, and if the robot is 
getting too close, the speed of the left wheel is increased. This algorithm assumes the 
wall is to the left of the robot, and there is various error control to deal with rogue 
ultrasonic sensor values.
