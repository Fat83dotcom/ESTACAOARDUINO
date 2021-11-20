#ifndef CLASSESFILTRO_H
#define CLASSEFILTRO_H

#include <Arduino.h>
#include <math.h>

class Temporarios;
class FiltraNaN;
double getTemp(int sensor);


class Temporarios{
  private:
  float t_Umi;
  float t_Temp;
  float t_Press;
  double t_10k;

  public:
  
    float *pt_U = &t_Umi;
    float *pt_T = &t_Temp;
    float *pt_P = &t_Temp;
    double *pt_10 = &t_10k;

};

class FiltraNaN {

  private:
  float _Umi;
  float _Temp;
  float _Press;
  double _10k;
  int cont;

  public:
  float umi_NaN(float umi, float *u) {
    _Umi = umi;
    if (!isnan(_Umi)) {
      *u = _Umi;
    }
    cont = 0;
    while (isnan(_Umi) && cont < 1000){
      _Umi = umi;
      cont ++;
    }
    if (cont == 1000) {
      return *u;
    }
    else {
      return _Umi;
    }
  }

  float temp_Nan(float temp, float *t) {
    _Temp = temp;
    if (!isnan(_Temp)) {
      *t = _Temp;
    }
    cont = 0;
    while (isnan(_Temp) && cont < 1000) {
      _Temp = temp;
      cont++;
    }
    if (cont == 1000) {
      return *t;
    }
    else {
      return _Temp;
    }
  }

  float press_Nan(float press, float *p) {
    _Press = press;
    if (!isnan(_Press)) {
      *p = _Press;
    }
    
    cont = 0;
    while (isnan(_Press) && cont < 1000) {
      _Press = press;
      cont++;
    }
    if (cont == 1000) {
      return *p;
    }
    else {
      return _Press;
    }
  }

  double t10k_Nan(double t10k, double *t10) {
    _10k = t10k;
    if (!isnan(_10k)) {
      *t10 = _10k;
    }
    cont = 0;
    while (isnan(_10k) && cont < 1000) {
      _10k = t10k;
      cont++;
    }
    if (cont == 1000) {
      return *t10;
    }
    else {
      return _10k;
    }
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
