float computeSpeed(int motorNb){
  float w;
  short newpos = pos[motorNb]; 
  unsigned long now = millis(); 
  float deltaT = (now - (float)oldtime[motorNb]) / 1000.0;
  w = ((float)newpos*2*pi / (float)nbsteprevolution)/ deltaT;
  pos[motorNb]=0;
  oldtime[motorNb] = now;
  return w;
}




/*
 * 
 * float computeSpeed(int motorNb){
  short newpos;
  float newtime,w;
  newpos=pos[motorNb];
  newtime=millis();
  w=(((float)newpos-(float)oldpos[motorNb])*2*pi/(float)nbsteprevolution)/(((float)newtime-(float)oldtime[motorNb])/1000.0);
  oldtime[motorNb]=newtime;
  oldpos[motorNb]=pos[motorNb];
  return w;
}
 * 
 */
