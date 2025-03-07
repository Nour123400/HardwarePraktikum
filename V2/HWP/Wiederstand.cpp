//Widerstand Programm
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
				
					pf.setUnitX("V");
					pf.setUnitY("V");
					pf.setUnitPara("V");
					pf.setDescX("AA0");
					pf.setDescY("AE0");
					pf.setDescPara("");
					pf.setRefX(5);
					pf.setRefY(5);
					pf.setParaFirstCurve(0);
					pf.setParaStepWidth(0);
			
					const uint8_t curve0 = 0;	
					
					drv.analogSequence(0, &buf[0], 0, 1, nullptr, 0, start, delta, count);
					for(uint16_t x = start; x < count; x+=20)
					{
					
						drv.analogWrite0(x);
						double u1 = (double) x*5/1023.0;
						double u2 = (double)drv.analogRead(0)*5/1023.0 ;

						std::cout <<"Spannung an AA0 ="<< u1<< "V       Spanung an AE0 =" << u2 <<"V"<<std::endl;
						
						pf.addDot(Dot(x,drv.analogRead(0),curve0));
						drv.delay_ms(10);
					}
			
					// speichern und plotty starten
			pf.writeToFile(PLOT_FILE);
			pf.startPlotty(PLOT_FILE);
			

				
	
		


}
