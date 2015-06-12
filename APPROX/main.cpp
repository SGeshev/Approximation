#include <iostream>
#include "msw.h"


double DEFAULT_XMIN = -30;
double DEFAULT_YMIN = 30;
double DEFAULT_XMAX = 30;
double DEFAULT_YMAX = -30;




int win_main(){
	string yn;  
	cwin<<RGB(0,0,0);
	cwin.axes();
	cwin<<SNAP;
	return 0;
}
