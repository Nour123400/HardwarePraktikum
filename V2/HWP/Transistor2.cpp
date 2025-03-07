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
	const uint8_t curve2 = 2;
	const uint8_t curve3 = 3;
	const uint8_t curve4= 4;
	const uint8_t curve5 = 5;
	const uint8_t curve6 = 6;
	pf.setUnitX("V");
	pf.setUnitY("mA");
	pf.setUnitPara("mA");
	pf.setDescX("V_in");
	pf.setDescY("I_D__I_B");
	pf.setDescPara("");
	pf.setRefX(5);
	pf.setRefY(5);
	pf.setParaFirstCurve(0);
	pf.setParaStepWidth(0);
    
    
    double Vlot_Rd;
	double Volt_D;
	double stromD;
	
	double Volt_Rg;
	double Volt_G;
	double stromG;
    
	
	drv.analogSequence(0, &buf[0], 0, 1, nullptr, 0, start, delta, count);
	
	
	
	 drv.analogWrite0(0);
	for (uint16_t x =208; x <=1023; x+=1)
	{
        drv.analogWrite1(x);
        std::cout<<(double)x*5/1023.0<<"V"<<std::endl;
        
        
        Vlot_Rd = ((double)drv.analogRead(0)*5/1023.0)-((double)drv.analogRead(1)*5/1023.0);
        Volt_D= ((double)drv.analogRead(0)*5/1023.0)- Vlot_Rd;
        stromD= (Vlot_Rd/150)*1000;
        
       Volt_Rg= ((double)x*5/1023.0) -((double)drv.analogRead(2)*5/1023.0);
       Volt_G= ((double)drv.analogRead(3)*5/1023.0);
  
        std::cout<<"ID    "<<stromD<<"mA"<<std::endl<<std::endl;
     
    
       pf.addDot(Dot(Volt_G/10*204,stromD/10*204,curve0));
	
	}
	
	pf.writeToFile(PLOT_FILE);
	pf.startPlotty(PLOT_FILE);
	
	
}
