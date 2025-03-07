//Transistor

#include <iostream>
#include <cmath>
#include <iomanip>
#include <b15f/b15f.h>
#include <b15f/plottyfile.h>

const char PLOT_FILE[] = "plot.bin";

B15F& drv = B15F::getInstance();



int main(){
	
	B15F& drv = B15F::getInstance();
    
	    
    
    drv.analogWrite0(1020);
	
	
	double Volt_Rg;
	double Volt_G;
	double stromG;
	
	while (true)
	{
        drv.analogWrite1(0);
        drv.delay_ms(1);

        Volt_Rg= ((double)drv.analogRead(3)*5/1023.0)-((double)drv.analogRead(2)*5/1023.0);
        Volt_G= ((double)drv.analogRead(3)*5/1023.0)- Volt_Rg;
        stromG= (Volt_Rg/1000)*1000;

        std::cout<<"IG    "<<stromG<<"mA"<<std::endl<<std::endl;


	  drv.analogWrite1(1023);
        drv.delay_ms(1);
              
        Volt_Rg= ((double)drv.analogRead(3)*5/1023.0)-((double)drv.analogRead(2)*5/1023.0);
        Volt_G= ((double)drv.analogRead(3)*5/1023.0)- Volt_Rg;
        stromG= (Volt_Rg/1000)*1000;
        
               
        
        
        
        std::cout<<"IG    "<<stromG<<"mA"<<std::endl<<std::endl;
     
		
		
	}
	
	
	
}
