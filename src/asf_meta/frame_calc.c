#include <math.h>
#include <string.h>

float ers_frame[901] = 
  {-99,0.40,0.80,1.20,1.60,2.00,2.40,2.80,3.20,3.60,4.00,4.40,4.80,5.20,5.60,6.00,
   6.40,6.80,7.20,7.60,8.00,8.40,8.80,9.20,9.60,10.00,10.40,10.80,11.20,11.60,12.00,
   12.40,12.80,13.20,13.60,14.00,14.40,14.80,15.20,15.60,16.00,16.40,16.80,17.20,17.60,18.00,
   18.40,18.80,19.20,19.60,20.00,20.40,20.80,21.20,21.60,22.00,22.40,22.80,23.20,23.60,24.00,
   24.40,24.80,25.20,25.60,26.00,26.40,26.80,27.20,27.60,28.00,28.40,28.80,29.20,29.60,30.00,
   30.40,30.80,31.20,31.60,32.00,32.40,32.80,33.20,33.60,34.00,34.40,34.80,35.20,35.60,36.00,
   36.40,36.80,37.20,37.60,38.00,38.40,38.80,39.20,39.60,40.00,40.40,40.80,41.20,41.60,42.00,
   42.40,42.80,43.20,43.60,44.00,44.40,44.80,45.20,45.60,46.00,46.40,46.80,47.20,47.60,48.00,
   48.40,48.80,49.20,49.60,50.00,50.40,50.80,51.20,51.60,52.00,52.40,52.80,53.20,53.60,54.00,
   54.40,54.80,55.20,55.60,56.00,56.40,56.80,57.20,57.60,58.00,58.40,58.80,59.20,59.60,60.00,
   60.39,60.77,61.16,61.54,61.92,62.31,62.69,63.08,63.47,63.84,64.23,64.61,64.99,65.38,65.76,
   66.14,66.52,66.90,67.29,67.67,68.04,68.42,68.80,69.18,69.55,69.93,70.31,70.68,71.06,71.43,
   71.80,72.17,72.55,73.91,70.31,70.68,71.06,71.43,71.80,75.17,72.55,72.91,76.19,76.54,76.90,
   77.25,77.59,77.94,78.28,78.62,78.96,79.29,79.62,79.94,80.26,80.57,80.88,81.18,81.47,81.75,
   82.03,82.29,82.54,82.79,83.01,83.22,83.42,83.60,83.76,83.89,84.01,84.10,84.16,84.20,84.21,
   84.20,84.16,84.10,84.01,83.89,83.76,83.60,83.42,83.22,83.01,82.79,82.54,82.29,82.03,81.75,
   81.47,81.18,80.88,80.57,80.26,79.94,79.62,79.29,78.96,78.62,78.28,77.94,77.59,77.25,76.90,
   76.54,76.19,72.91,72.55,75.17,71.80,71.43,71.06,70.68,70.31,73.91,72.55,72.17,71.80,71.43,
   71.06,70.68,70.31,69.93,69.55,69.18,68.80,68.42,68.04,67.67,67.29,66.90,66.52,66.14,65.76,
   65.38,64.99,64.61,64.23,63.84,63.47,63.08,62.69,62.31,61.92,61.54,61.16,60.77,60.39,60.00,
   59.60,59.20,58.80,58.40,58.00,57.60,57.20,56.80,56.40,56.00,55.60,55.20,54.80,54.40,54.00,
   53.60,53.20,52.80,52.40,52.00,51.60,51.20,50.80,50.40,50.00,49.60,49.20,48.80,48.40,48.00,
   47.60,47.20,46.80,46.40,46.00,45.60,45.20,44.80,44.40,44.00,43.60,43.20,42.80,42.40,42.00,
   41.60,41.20,40.80,40.40,40.00,39.60,39.20,38.80,38.40,38.00,37.60,37.20,36.80,36.40,36.00,
   35.60,35.20,34.80,34.40,34.00,33.60,33.20,32.80,32.40,32.00,31.60,31.20,30.80,30.40,30.00,
   29.60,29.20,28.80,28.40,28.00,27.60,27.20,26.80,26.40,26.00,25.60,25.20,24.80,24.40,24.00,
   23.60,23.20,22.80,22.40,22.00,21.60,21.20,20.80,20.40,20.00,19.60,19.20,18.80,18.40,18.00,
   17.60,17.20,16.80,16.40,16.00,15.60,15.20,14.80,14.40,14.00,13.60,13.20,12.80,12.40,12.00,
   11.60,11.20,10.80,10.40,10.00,9.60,9.20,8.80,8.40,8.00,7.60,7.20,6.80,6.40,6.00,
   5.60,5.20,4.80,4.40,4.00,3.60,3.20,2.80,2.40,2.00,1.60,1.20,0.80,0.40,0.00,
   -0.40,-0.80,-1.20,-1.60,-2.00,-2.40,-2.80,-3.20,-3.60,-4.00,-4.40,-4.80,-5.20,-5.60,-6.00,
   -6.40,-6.80,-7.20,-7.60,-8.00,-8.40,-8.80,-9.20,-9.60,-10.00,-10.40,-10.80,-11.20,-11.60,-12.00,
   -12.40,-12.80,-13.20,-13.60,-14.00,-14.40,-14.80,-15.20,-15.60,-16.00,-16.40,-16.80,-17.20,-17.60,-18.00,
   -18.40,-18.80,-19.20,-19.60,-20.00,-20.40,-20.80,-21.20,-21.60,-22.00,-22.40,-22.80,-23.20,-23.60,-24.00,
   -24.40,-24.80,-25.20,-25.60,-26.00,-26.40,-26.80,-27.20,-27.60,-28.00,-28.40,-28.80,-29.20,-29.60,-30.00,
   -30.40,-30.80,-31.20,-31.60,-32.00,-32.40,-32.80,-33.20,-33.60,-34.00,-34.40,-34.80,-35.20,-35.60,-36.00,
   -36.40,-36.80,-37.20,-37.60,-38.00,-38.40,-38.80,-39.20,-39.60,-40.00,-40.40,-40.80,-41.20,-41.60,-42.00,
   -42.40,-42.80,-43.20,-43.60,-44.00,-44.40,-44.80,-45.20,-45.60,-46.00,-46.40,-46.80,-47.20,-47.60,-48.00,
   -48.40,-48.80,-49.20,-49.60,-50.00,-50.40,-50.80,-51.20,-51.60,-52.00,-52.40,-52.80,-53.20,-53.60,-54.00,
   -54.40,-54.80,-55.20,-55.60,-56.00,-56.40,-56.80,-57.20,-57.60,-58.00,-58.40,-58.80,-59.20,-59.60,-60.00,
   -60.39,-60.77,-61.16,-61.54,-61.92,-62.31,-62.69,-63.08,-63.47,-63.84,-64.23,-64.61,-64.99,-65.38,-65.76,
   -66.14,-66.52,-66.90,-67.29,-67.67,-68.04,-68.42,-68.80,-69.18,-69.55,-69.93,-70.31,-70.68,-71.06,-71.43,
   -71.80,-72.17,-72.55,-73.91,-70.31,-70.68,-71.06,-71.43,-71.80,-75.17,-72.55,-72.91,-76.19,-76.54,-76.90,
   -77.25,-77.59,-77.94,-78.28,-78.62,-78.96,-79.29,-79.62,-79.94,-80.26,-80.57,-80.88,-81.18,-81.47,-81.75,
   -82.03,-82.29,-82.54,-82.79,-83.01,-83.22,-83.42,-83.60,-83.76,-83.89,-84.01,-84.10,-84.16,-84.20,-84.21
   -84.21,-84.20,-84.16,-84.10,-84.01,-83.89,-83.76,-83.60,-83.42,-83.22,-83.01,-82.79,-82.54,-82.29,-82.03,
   -81.75,-81.47,-81.18,-80.88,-80.57,-80.26,-79.94,-79.62,-79.29,-78.96,-78.62,-78.28,-77.94,-77.59,-77.25,
   -76.90,-76.54,-76.19,-72.91,-72.55,-75.17,-71.80,-71.43,-71.06,-70.68,-70.31,-73.91,-72.55,-72.17,-71.80,
   -71.43,-71.06,-70.68,-70.31,-69.93,-69.55,-69.18,-68.80,-68.42,-68.04,-67.67,-67.29,-66.90,-66.52,-66.14,
   -65.76,-65.38,-64.99,-64.61,-64.23,-63.84,-63.47,-63.08,-62.69,-62.31,-61.92,-61.54,-61.16,-60.77,-60.39,
   -60.00,-59.60,-59.20,-58.80,-58.40,-58.00,-57.60,-57.20,-56.80,-56.40,-56.00,-55.60,-55.20,-54.80,-54.40,
   -54.00,-53.60,-53.20,-52.80,-52.40,-52.00,-51.60,-51.20,-50.80,-50.40,-50.00,-49.60,-49.20,-48.80,-48.40,
   -48.00,-47.60,-47.20,-46.80,-46.40,-46.00,-45.60,-45.20,-44.80,-44.40,-44.00,-43.60,-43.20,-42.80,-42.40,
   -42.00,-41.60,-41.20,-40.80,-40.40,-40.00,-39.60,-39.20,-38.80,-38.40,-38.00,-37.60,-37.20,-36.80,-36.40,
   -36.00,-35.60,-35.20,-34.80,-34.40,-34.00,-33.60,-33.20,-32.80,-32.40,-32.00,-31.60,-31.20,-30.80,-30.40,
   -30.00,-29.60,-29.20,-28.80,-28.40,-28.00,-27.60,-27.20,-26.80,-26.40,-26.00,-25.60,-25.20,-24.80,-24.40,
   -24.00,-23.60,-23.20,-22.80,-22.40,-22.00,-21.60,-21.20,-20.80,-20.40,-20.00,-19.60,-19.20,-18.80,-18.40,
   -18.00,-17.60,-17.20,-16.80,-16.40,-16.00,-15.60,-15.20,-14.80,-14.40,-14.00,-13.60,-13.20,-12.80,-12.40,
   -12.00,-11.60,-11.20,-10.80,-10.40,-10.00,-9.60,-9.20,-8.80,-8.40,-8.00,-7.60,-7.20,-6.80,-6.40,-6.00,
   -5.60,-5.20,-4.80,-4.40,-4.00,-3.60,-3.20,-2.80,-2.40,-2.00,-1.60,-1.20,-0.80,-0.40,0.00};


int asf_frame_calc(char *sensor, float latitude, char orbit_direction)
{
  int i=1, frame=-1;
  float diff=99;

  if (strncmp(sensor, "ERS", 3)==0 || strncmp(sensor, "JERS", 4)==0) {
    if (orbit_direction == 'D') {
      i = 225;
      while (fabs(ers_frame[i]-latitude) < diff) {
        diff = fabs(ers_frame[i]-latitude);
        frame = i;
        i++;
      }
    }
    else {
      i = 1;
      while (fabs(ers_frame[i]-latitude) < diff) {
        diff = fabs(ers_frame[i]-latitude);
        frame = i;
        if (i < 225) i++;
        else i=675;
      }
    } 
  }

  return frame;
}
