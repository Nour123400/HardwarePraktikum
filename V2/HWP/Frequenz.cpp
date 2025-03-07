//Freq

#include <iostream>
#include <cmath>
#include <iomanip>
#include <b15f/b15f.h>






int main(){
	
	B15F& drv = B15F::getInstance();
    
	    
    
    //drv.analogWrite1();
	
	
	
	while (true)
	{

	//delay 1000ns for 1 Perode
        drv.analogWrite0(0);
        drv.delay_ns(500);


	  drv.analogWrite0(1023);
        drv.delay_ns(500);
                     
		
		
	}
	
	
	
}
