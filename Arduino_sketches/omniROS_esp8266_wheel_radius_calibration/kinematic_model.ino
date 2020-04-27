/*Here is located the forward and inverse kinematic model
 *Source : Introduction to Autonomous Mobile Robots, Roland Siegwart and Illah R. Nourbakhsh
 *Assumptions : rolling without slipping, dynamic effects are not taken into account, angular spacing between the wheel is 2*pi/3  or 120°
 *Takes into account : different radius for each wheel, different distance between the wheels and the center of the robot
 *For more information, read "Maxima_holonomic_differentRadius.wxmx" (must be opened with the free symbolic mathematic software)
*/

#define sqrt3  1.73205

void ForwardKinematic(float ** worldSpeedP, float ** measuredSpeedP){
  /*
  Old kinematic model, assumes same radius and same length 
  
  worldSpeed[0] = (r / sqrt3) * (measuredSpeed[2] - measuredSpeed[1]);
  worldSpeed[1] = -(r / 3) * (2 * measuredSpeed[0] - measuredSpeed[1] - measuredSpeed[2]);
  worldSpeed[2] = (r / (3 * R)) * (measuredSpeed[0] + measuredSpeed[1] + measuredSpeed[2]);
*/

  worldSpeedP[0] = ((2*motorSpeedP[0]*r1+motorSpeedP[1]*r2)*L3+(motorSpeedP[0]*r1-motorSpeedP[2]*r3)*L2+((-motorSpeedP[1]*r2)-2*motorSpeedP[2]*r3)*L1)/(sqrt3*L1+sqrt3*L2+sqrt3*L3);
  worldSpeedP[1] = (motorSpeedP[1]*r2*L3+((-motorSpeedP[0]*r1)-motorSpeedP[2]*r3)*L2+motorSpeedP[1]*r2*L1)/(L1+L2+L3);
  worldSpeedP[2] = -(motorSpeedP[2]*r3+motorSpeedP[1]*r2+motorSpeedP[0]*r1)/(L1+L2+L3);
  
  return;
}

void InverseKinematic(float ** motorSpeedP, float ** worldSpeedCommandP){
  
  motorSpeedP[0] = -(2*worldSpeedCommandP[2]*L1-worldSpeedCommandP[0]*sqrt3+worldSpeedCommandP[1])/(2*r1);
  motorSpeedP[1] = -(worldSpeedCommandP[2]*L2-worldSpeedCommandP[1])/r2;
  motorSpeedP[2] = -(2*worldSpeedCommandP[2]*L3+worldSpeedCommandP[0]*sqrt3+worldSpeedCommandP[1])/(2*r3);
  
  return;
}
