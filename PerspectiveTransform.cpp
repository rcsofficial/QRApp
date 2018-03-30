//---------------------------------------------------------------------------
//
// Name:        PerspectiveTransform.cpp
// Author:      John Benedict Du
// Created:     14/02/2018 2:07:33 PM
// Description: 
//
//---------------------------------------------------------------------------

#include <vector>
#include <fstream>
#include <iostream>
#include "PerspectiveTransform.h"

PerspectiveTransform::PerspectiveTransform(
                                double a11Given, double a21Given, double a31Given,
                                double a12Given, double a22Given, double a32Given,
                                double a13Given, double a23Given, double a33Given)
{
  a11 = a11Given;
  a12 = a12Given;
  a13 = a13Given;
  a21 = a21Given;
  a22 = a22Given;
  a23 = a23Given;
  a31 = a31Given;
  a32 = a32Given;
  a33 = a33Given;					 
}

PerspectiveTransform PerspectiveTransform::quadrilateralToQuadrilateral(
                                                double x0, double y0,
                                                double x1, double y1,
                                                double x2, double y2,
                                                double x3, double y3,
                                                double x0prime, double y0prime,
                                                double x1prime, double y1prime,
                                                double x2prime, double y2prime,
                                                double x3prime, double y3prime)
{
  PerspectiveTransform qToS = quadrilateralToSquare(
    x0, y0, 
    x1, y1, 
    x2, y2, 
    x3, y3
  );
  
  PerspectiveTransform sToQ = squareToQuadrilateral(
    x0prime, y0prime, 
    x1prime, y1prime, 
    x2prime, y2prime, 
    x3prime, y3prime
  );

  return sToQ.times(qToS);
}

void PerspectiveTransform::transformPoints(double *points, int length)
{
  int max = length;
  for(int i = 0; i < max; i += 2)
  {
    double x = *(points);
    double y = *(points + 1);
    double denominator = a13 * x + a23 * y + a33;
    *(points) = (a11 * x + a21 * y + a31) / denominator;
    *(points + 1) = (a12 * x + a22 * y + a32) / denominator;
  }
}

int** PerspectiveTransform::transformPoints(int** givenPixelArray, int givenWidth, int givenHeight)
{

    //Dynamic Allocation for 2D Array of Pixel Values
  int** imgPixelArray = new int*[givenHeight];
  for(int dynamicIndex = 0; dynamicIndex < givenHeight; ++dynamicIndex)
  {
    imgPixelArray[dynamicIndex] = new int[givenWidth];
  }
    
  for (int heightIndex=0; heightIndex<givenHeight; ++heightIndex)
  {
    for (int widthIndex=0; widthIndex<givenWidth; ++widthIndex)
    {
      imgPixelArray[heightIndex][widthIndex] = 1;
    }
  }
       
  for (int heightIndex=0; heightIndex<givenHeight; ++heightIndex)
  {
    for (int widthIndex=0; widthIndex<givenWidth; ++widthIndex)
    {
      double x = widthIndex;
      double y = heightIndex;
      double denominator = a13 * x + a23 * y + a33;
      double doubleNewX = ((a11 * x + a21 * y + a31) / denominator);
      double doubleNewY = ((a12 * x + a22 * y + a32) / denominator);
      int newX = doubleNewX;
      int newY = doubleNewY;
            
      // Store Binarized Pixel Value
      int value = givenPixelArray[heightIndex][widthIndex];
      if(newX < givenWidth && newY < givenHeight && newX > 0 && newY > 0)
      {
        imgPixelArray[newY][newX] = value;
      }
    }
  }    
        
    //Use this code to check if the matrix is generated correctly
    /*    
    std::ofstream myfile ("matrix.txt");
    if (myfile.is_open())
    {
        myfile << a11 <<"    " << a21 << "   " << a31 << "\n";
        myfile << a12 <<"    " << a22 << "   " << a32 << "\n";
        myfile << a13 <<"    " << a23 << "   " << a33 << "\n";
        for (int heightIndex=0; heightIndex<givenHeight; ++heightIndex)
        {
            for (int widthIndex=0; widthIndex<givenWidth; ++widthIndex)
            {
                myfile << imgPixelArray[heightIndex][widthIndex] << " ";
            }
            myfile << "\n";
        }    
        myfile.close();
    }
    else std::cout << "Unable to open file";
    */
  return imgPixelArray;
}

void PerspectiveTransform::transformPoints(
                                        double *xValues, 
                                        double *yValues, 
                                        int length)
{
  int n = length;
  for(int i = 0; i < n; i++)
  {
    double x = *(xValues + i);
    double y = *(yValues + i);
    double denominator = a13 * x + a23 * y + a33;
    *(xValues + i) = (a11 * x + a21 * y + a31) / denominator;
    *(yValues + i) = (a12 * x + a22 * y + a32) / denominator;
  }
}

void PerspectiveTransform::transformPoints(
                                        std::vector<double> xValues, 
                                        std::vector<double> yValues, 
                                        int length){
  int n = length;
  for(int i = 0; i < n; i++)
  {
    double x = (xValues.at(i));
    double y = (yValues.at(i));
    double denominator = a13 * x + a23 * y + a33;
    (xValues.at(i)) = (a11 * x + a21 * y + a31) / denominator;
    (yValues.at(i)) = (a12 * x + a22 * y + a32) / denominator;
  }
}

PerspectiveTransform PerspectiveTransform::squareToQuadrilateral(
                                                            double x0, double y0,
                                                            double x1, double y1,
                                                            double x2, double y2,
                                                            double x3, double y3)
{
  double dx3 = x0 - x1 + x2 - x3;
  double dy3 = y0 - y1 + y2 - y3;
        
  if(dx3 == 0.0 && dy3 == 0.0)
  {            
    a11 = x1 - x0;
    a21 = x2 - x1;
    a31 = x0;
    a12 = y1 - y0;
    a22 = y2 - y1;
    a32 = y0;
    a13 = 0.0;
    a23 = 0.0;
    a33 = 1.0;
  }
  
  else
  {
    double dx1 = x1 - x2;
    double dx2 = x3 - x2;
    double dy1 = y1 - y2;
    double dy2 = y3 - y2;
    double denominator = dx1 * dy2 - dx2 * dy1;
    a13 = (dx3 * dy2 - dx2 * dy3) / denominator;
    a23 = (dx1 * dy3 - dx3 * dy1) / denominator;
            
    a11 = x1 - x0 + a13 * x1;
    a21 = x3 - x0 + a23 * x3;
    a31 = x0;
    a12 = y1 - y0 + a13 * y1;
    a22 = y3 - y0 + a23 * y3;
    a32 = y0;
    a33 = 1.0;
  }
        
  PerspectiveTransform result(
    a11, a21, a31,
    a12, a22, a32,
    a13, a23, 1.0
  );
        
        //Use this code to check if the matrix is generated correctly
        /*
        std::ofstream myfile ("result.txt");
        if (myfile.is_open())
        {
            myfile << a11 <<"    " << a21 << "   " << a31 << "\n";
            myfile << a12 <<"    " << a22 << "   " << a32 << "\n";
            myfile << a13 <<"    " << a23 << "   " << a33 << "\n";
            myfile.close();
        }
        else std::cout << "Unable to open file";
        */
        
  return result;
}

PerspectiveTransform PerspectiveTransform::reverseWarp(
                                                        double x0, double y0,
                                                        double x1, double y1,
                                                        double x3, double y3)
{
  x0 *= 1.0;
  x1 *= 1.0;         
  x3 *= 1.0;
  double x2 = (x3 + (x1 - x0))*1.0;
        
  y0 *= 1.0;
  y1 *= 1.0;
  y3 *= 1.0;
  double y2 = (y3 + (y1 - y0))*1.0;

  double scaleY = 0;
  double scaleX = 0;

  double x0p = 0; 
  double x1p = 0;
  double x2p = 0;
  double x3p = 0;
    
  double y0p = 0; 
  double y1p = 0;
  double y2p = 0;
  double y3p = 0;
    
  if(y2 > y3)
  {
    y0p = y0 + (y2 - y3);
    y1p = y0;
    y2p = y2 - (y1 - y0);
    y3p = y2;
    scaleY = y3p - y1p;
  }
  
  else
  {
    y0p = y1;
    y1p = y1 + (y3 - y2);
    y2p = y3;
    y3p = y3 - (y0 - y1);
    scaleY = y2p - y0p;
  }
    
  if(x1 > x2)
  {
    x0p = x3; 
    x1p = x1 - (x0 - x3);
    x2p = x1;
    x3p = x3 + (x1 - x2);
    scaleX = x1 - x3;
  }
  
  else
  {
    x0p = x0 + (x2 - x1); 
    x1p = x2;
    x2p = x2 - (x3 - x0);
    x3p = x0;
    scaleX = x2 - x0;
  }
    
    x0p /= scaleX;
    x1p /= scaleX;
    x2p /= scaleX;
    x3p /= scaleX;
    
    y0p /= scaleY;
    y1p /= scaleY;
    y2p /= scaleY;
    y3p /= scaleY;        

    //Use this code to check if the coordinates were generated correctly
    /*
    std::ofstream myfile ("center.txt");
    if (myfile.is_open())
    {
        myfile << x0p << "\t" << y0p << "\n";
        myfile << x1p << "\t" << y1p << "\n";
        myfile << x2p << "\t" << y2p << "\n";
        myfile << x3p << "\t" << y3p << "\n";

        myfile.close();
    }
    else std::cout << "Unable to open file";
    */
        
  PerspectiveTransform result = squareToQuadrilateral(
    x0p, y0p,
    x1p, y1p,
    x2p, y2p,
    x3p, y3p
  );                            
  
  return result;                                                                          
}

PerspectiveTransform PerspectiveTransform::quadrilateralToSquare(
                                                            double x0, double y0,
                                                            double x1, double y1,
                                                            double x2, double y2,
                                                            double x3, double y3)
{
  // Here, the adjoint serves as the inverse:
  return squareToQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3).buildAdjoint();
}

PerspectiveTransform PerspectiveTransform::buildAdjoint()
{
    // Adjoint is the transpose of the cofactor matrix:
        
  a11 = a22 * a33 - a23 * a32;
  a21 = a23 * a31 - a21 * a33;
  a31 = a21 * a32 - a22 * a31;
  a12 = a13 * a32 - a12 * a33;
  a22 = a11 * a33 - a13 * a31;
  a32 = a12 * a31 - a11 * a32;
  a13 = a12 * a23 - a13 * a22;
  a23 = a13 * a21 - a11 * a23;
  a33 = a11 * a22 - a12 * a21;
        
  PerspectiveTransform result(
    a22 * a33 - a23 * a32,
    a23 * a31 - a21 * a33,
    a21 * a32 - a22 * a31,
    a13 * a32 - a12 * a33,
    a11 * a33 - a13 * a31,
    a12 * a31 - a11 * a32,
    a12 * a23 - a13 * a22,
    a13 * a21 - a11 * a23,
    a11 * a22 - a12 * a21
  );

  return result;
}

PerspectiveTransform PerspectiveTransform::times(PerspectiveTransform other)
{
  PerspectiveTransform result(
    a11 * other.a11 + a21 * other.a12 + a31 * other.a13,
    a11 * other.a21 + a21 * other.a22 + a31 * other.a23,
    a11 * other.a31 + a21 * other.a32 + a31 * other.a33,
    a12 * other.a11 + a22 * other.a12 + a32 * other.a13,
    a12 * other.a21 + a22 * other.a22 + a32 * other.a23,
    a12 * other.a31 + a22 * other.a32 + a32 * other.a33,
    a13 * other.a11 + a23 * other.a12 + a33 * other.a13,
    a13 * other.a21 + a23 * other.a22 + a33 * other.a23,
    a13 * other.a31 + a23 * other.a32 + a33 * other.a33
  );
  
  return result;
}
