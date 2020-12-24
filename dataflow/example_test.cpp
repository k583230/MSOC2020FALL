// 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
#include <iostream>
#include <stdlib.h>
#include "example.h"

void example(strm_t &in1, strm_t &in2, strm_t &out);

int main()
{
  // Declare streams
  strm_t in1, in2, out;
  value_t valDataCtrl1, valDataCtrl2;

  valDataCtrl1.data = 0;
  valDataCtrl1.keep = 0xF;
  valDataCtrl1.strb = 0;
  valDataCtrl1.user = 0;
  valDataCtrl1.last = 0;
  valDataCtrl1.id = 0;
  valDataCtrl1.dest = 0;

  valDataCtrl2.data = 0;
  valDataCtrl2.keep = 0xF;
  valDataCtrl2.strb = 0;
  valDataCtrl2.user = 0;
  valDataCtrl2.last = 0;
  valDataCtrl2.id = 0;
  valDataCtrl2.dest = 0;
  // Write data into a and b
  for (int i = 0; i < 10; i++)
  {
    int a = rand() % 10;
    int b = rand() % 10;
    valDataCtrl1.data =a;
    valDataCtrl2.data =b;
    in1.write(valDataCtrl1);
    in2.write(valDataCtrl2);

  }

  example(in1, in2, out);
  
  value_t sum = out.read();
  
  std::cout << "sum = " << sum.data << std::endl;
  
  return 0;
}
