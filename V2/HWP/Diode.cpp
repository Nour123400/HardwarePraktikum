//Diode Programm
#include <iostream>
#include <cmath>
#include <iomanip>
#include <b15f/b15f.h>
#include <b15f/plottyfile.h>

const char PLOT_FILE[] = "plot.bin";
int main() {



B15F& drv = B15F::getInstance();




		PlottyFile pf;
		
		
			
				
					uint16_t buf[1023];
					const uint16_t delta = 1;
					const uint16_t count = 1023;
					const uint16_t start =0;
				
					pf.setUnitX("b_mA_L_V ");
					pf.setUnitY("V");
					pf.setUnitPara("I");
					pf.setDescX("AA0");
					pf.setDescY("U_I");
					pf.setDescPara("U");
					pf.setRefX(5);
					pf.setRefY(5);
					pf.setParaFirstCurve(0);
					pf.setParaStepWidth(0);
			
					const uint8_t curve0 = 0;	
					const uint8_t curve1 = 1;
					
					drv.analogSequence(0, &buf[0], 0, 1, nullptr, 0, start, delta, count);
					for(uint16_t x = start; x < count; x+=20)
					{
					
						drv.analogWrite0(x);
						double u1 = (double) x*5/1023.0;
						double u2 = (double)drv.analogRead(0)*5/1023.0 ;

						
						std::cout <<"AA0 "<< u1<< "V - " << u1-u2 <<"V \"lila\""<<std::endl;
					
						
						double  i_D= u2 /1 ;     
						std::cout <<"AA0 "<< u1<< "V - "  << i_D << "mA \"Blau\""<<std::endl;//u2<< "V - "  << i_D << "mA"<<std::endl; 
						
						pf.addDot(Dot(x,x-drv.analogRead(0),curve0));
						pf.addDot(Dot(x,i_D*200,curve1));
						drv.delay_ms(1);
					}
			
					// speichern und plotty starten
			pf.writeToFile(PLOT_FILE);
			pf.startPlotty(PLOT_FILE);
			

				
	
		


}





