#ifndef CLASSESFILTRO_H
#define CLASSEFILTRO_H

#include <Arduino.h>
#include <math.h>

class Temporarios;
class FiltraNaN;
double getTemp(int sensor);

class Temporarios {
  private:
  double t_Umi;
  double t_Temp;
  double t_Press;
  double t_10k;

  public:
  double *pt_U = &t_Umi;
  double *pt_T = &t_Temp;
  double *pt_P = &t_Temp;
  double *pt_10 = &t_10k;

};

class FiltraNaN {
  private:
  double _Umi;
  double _Temp;
  double _Press;
  double _10k;
  int cont;

  public:
  double umi_NaN (double umidade, double *pUmidade) {
    _Umi = umidade;
    if (!isnan(_Umi)) {
      *pUmidade = _Umi;
    }
    cont = 0;
    while (isnan(_Umi) && cont < 1000){
      _Umi = umidade;
      cont ++;
    }
    return (cont == 1000) ? *pUmidade : _Umi;
    
  }

  double temp_Nan (double temperatura_1, double *ptemperatura_1) {
    _Temp = temperatura_1;
    if (!isnan(_Temp)) {
      *ptemperatura_1 = _Temp;
    }
    cont = 0;
    while (isnan(_Temp) && cont < 1000) {
      _Temp = temperatura_1;
      cont++;
    }
    return (cont == 1000) ? *ptemperatura_1 : _Temp;
    
  }

  double press_Nan (double press, double *pPressao) {
    _Press = press;
    if (!isnan(_Press)) {
      *pPressao = _Press;
    }
    
    cont = 0;
    while (isnan(_Press) && cont < 1000) {
      _Press = press;
      cont++;
    }
    return (cont == 1000) ? *pPressao : _Press;
    
  }

  double t10k_Nan(double temperatura_10k, double *pTemperatura_2) {
    _10k = temperatura_10k;
    if (!isnan(_10k)) {
      *pTemperatura_2 = _10k;
    }
    cont = 0;
    while (isnan(_10k) && cont < 1000) {
      _10k = temperatura_10k;
      cont++;
    }
    return (cont == 1000) ? *pTemperatura_2 : _10k;
    
  }
};

double getTemp(int sensor) {
  
  int RawADC = analogRead(sensor);
  long Resistance;
  double Temp;
  Resistance=((10240000/RawADC) - 10000);
  Temp = log(Resistance);
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius
  return Temp;  
}

#endif
