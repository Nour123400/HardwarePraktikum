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
    
	PlottyFile pf;
	
	uint16_t buf[1023];
	const uint16_t delta = 1;
	const uint16_t count = 1023;
	const uint16_t start =0;
	const uint8_t curve0 = 0;
	const uint8_t curve1 = 1;
	pf.setUnitX("V");
	pf.setUnitY("V");
	pf.setUnitPara("V_out");
	pf.setDescX("V_in");
	pf.setDescY("V");
	pf.setDescPara("");
	pf.setRefX(5);
	pf.setRefY(5);
	pf.setParaFirstCurve(0);
	pf.setParaStepWidth(0);
    
    
    
    drv.analogWrite0(1020);
	
	
	double Vlot_Rd;
	double Volt_D;
	double stromD;
	
	double Volt_Rg;
	double Volt_G;
	double stromG;
	drv.analogSequence(0, &buf[0], 0, 1, nullptr, 0, start, delta, count);
	
	for (uint16_t x =0; x <=1023; x+=1)
	{
        drv.analogWrite1(x);
        std::cout<<Spannung an AA1= <<(double)x*5/1023.0<<"V"<<std::endl;
        
        
        Vlot_Rd = ((double)drv.analogRead(0)*5/1023.0)-((double)drv.analogRead(1)*5/1023.0);
        Volt_D= ((double)drv.analogRead(0)*5/1023.0)- Vlot_Rd;
        stromD= (Vlot_Rd/150)*1000;
        
   //     Volt_Rg= ((double)drv.analogRead(3)*5/1023.0) -((double)drv.analogRead(2)*5/1023.0);
        Volt_G= ((double)drv.analogRead(3)*5/1023.0);
       // stromG= (Volt_Rg/1000)*1000;
        
        
        
        std::cout<<"D     "<<stromD<<"V"<<std::endl;
        std::cout<<"G     "<<Volt_G<<"V"<<std::endl<<std::endl;
     

        
       //drv.delay_ms();
       pf.addDot(Dot(x,Volt_D*204,curve0));
	 pf.addDot(Dot(x,Volt_G*204,curve1));
		
		
	}
	
	pf.writeToFile(PLOT_FILE);
	pf.startPlotty(PLOT_FILE);
	
	
}
