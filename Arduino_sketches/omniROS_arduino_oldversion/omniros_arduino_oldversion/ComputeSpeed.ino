double computeSpeed(int motorNb){
  const int nbsteprevolution = 1398;
  const float pi=3.141592653;
  double newpos,newtime,w;
  newpos=pos[motorNb];
  newtime=millis();
  w=(((float)newpos-(float)oldpos[motorNb])*2*pi/(float)nbsteprevolution)/(((float)newtime-(float)oldtime[motorNb])/1000.0);
  oldtime[motorNb]=newtime;
  oldpos[motorNb]=newpos;
  return w;
}
