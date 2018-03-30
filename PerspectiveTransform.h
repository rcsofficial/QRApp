//---------------------------------------------------------------------------
//
// Name:        PerspectiveTransform.h
// Author:      John Benedict Du
// Created:     14/02/2018 2:30:24 PM
// Description: 
//
//---------------------------------------------------------------------------

#include <vector>

class PerspectiveTransform{
  private:
    double a11, a12, a13;
    double a21, a22, a23;
    double a31, a32, a33;
	
  public:
    PerspectiveTransform(
      double a11Given, double a21Given, double a31Given,
      double a12Given, double a22Given, double a32Given,
      double a13Given, double a23Given, double a33Given
    );

    PerspectiveTransform quadrilateralToQuadrilateral(
      double x0, double y0,
      double x1, double y1,
      double x2, double y2,
      double x3, double y3,
      double x0prime, double y0prime,
      double x1prime, double y1prime,
      double x2prime, double y2prime,
      double x3prime, double y3prime
    );
    
    PerspectiveTransform squareToQuadrilateral(
      double x0, double y0,
      double x1, double y1,
      double x2, double y2,
      double x3, double y3
    );
    
    PerspectiveTransform quadrilateralToSquare(
      double x0, double y0,
      double x1, double y1,
      double x2, double y2,
      double x3, double y3
    );
    
    PerspectiveTransform reverseWarp(
      double x0, double y0,
      double x1, double y1,
      double x3, double y3
    );
    
    PerspectiveTransform times(PerspectiveTransform givenMatrix);
      void transformPoints(double *points, int length);
      void transformPoints(double *xValues, double *yValues, int length);
      int** transformPoints(int** givenPixelArray, int givenWidth, int givenHeight);
      
      void transformPoints(
        std::vector<double> xValues, 
        std::vector<double> yValues, 
        int length
      );
      
    PerspectiveTransform buildAdjoint();
};
