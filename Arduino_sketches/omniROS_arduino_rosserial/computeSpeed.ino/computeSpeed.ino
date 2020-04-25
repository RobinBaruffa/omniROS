double computeSpeed(int motorNb){
  
  float newpos,newtime,w;
  newpos=pos[motorNb];
  newtime=millis();
  w=(((float)newpos-(float)oldpos[motorNb])*2*pi/(float)nbsteprevolution)/(((float)newtime-(float)oldtime[motorNb])/1000.0);
  oldtime[motorNb]=newtime;
  oldpos[motorNb]=pos[motorNb];
  return w;
}
