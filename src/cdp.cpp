#include "cdp.hpp"
#include <iostream>


void CDP::inicializarDatos(){

    //tasa de interes en colones y dolares dependieno de los dias
    //sacado de ref 10 bcr

    informacionIntereses[30] = {0.0301, 0.0161};
    informacionIntereses[59] = {0.0301, 0.0161};
    informacionIntereses[60] = {0.0345, 0.0166};
    informacionIntereses[89] = {0.0345, 0.0166};
    informacionIntereses[90] = {0.0388, 0.0196}; 
    informacionIntereses[119] = {0.0388, 0.0196}; 
    informacionIntereses[120] = {0.0453, 0.0221}; 
    informacionIntereses[149] = {0.0453, 0.0221}; 
    informacionIntereses[150] = {0.0501, 0.0251};
    informacionIntereses[179] = {0.0501, 0.0251};
    informacionIntereses[180] = {0.0519, 0.0281};
    informacionIntereses[239] = {0.0519, 0.0281};
    informacionIntereses[240] = {0.0544, 0.0302};
    informacionIntereses[269] = {0.0544, 0.0302};
    informacionIntereses[270] = {0.0545, 0.0336};
    informacionIntereses[299] = {0.0545, 0.0336};
    informacionIntereses[300] = {0.0549, 0.0361};
    informacionIntereses[329] = {0.0549, 0.0361};
    informacionIntereses[330] = {0.0554, 0.0361};
    informacionIntereses[359] = {0.0554, 0.0361};
    informacionIntereses[360] = {0.0559, 0.0378};
    informacionIntereses[539] = {0.0559, 0.0378};
    informacionIntereses[540] = {0.0569, 0.0441};
    informacionIntereses[719] = {0.0569, 0.0441};
    informacionIntereses[720] = {0.0604, 0.0448};
    informacionIntereses[1079] = {0.0604, 0.0448};
    informacionIntereses[1080] = {0.0624, 0.0456};
    informacionIntereses[1439] = {0.0624, 0.0456};
    informacionIntereses[1440] = {0.0638, 0.0471};
    informacionIntereses[1799] = {0.0638, 0.0471};
    informacionIntereses[1800] = {0.0652, 0.0489};

}
