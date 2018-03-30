///-----------------------------------------------------------------
///
/// @file      QRAppFrm.cpp
/// @author    Richmond Sim
/// Created:   13/02/2018 10:52:28 AM
/// @section   DESCRIPTION
///            QRAppFrm class implementation
///
///------------------------------------------------------------------

#include "QRAppFrm.h"
#include "PerspectiveTransform.h"
#include <numeric>
#include <fstream>

bool imgState = false;
wxImage imgInput;
wxImage imgView;
wxImage imgOutput;
int imgHeight;
int imgWidth;

using namespace std;

/*
 * Event Table
 */
BEGIN_EVENT_TABLE(QRAppFrm,wxFrame)	
  EVT_CLOSE(QRAppFrm::OnClose)
  EVT_BUTTON(ID_WXBUTTON1,QRAppFrm::WxButton1Click)
  EVT_BUTTON(ID_WXBUTTON2,QRAppFrm::WxButton2Click)
  EVT_BUTTON(ID_WXBUTTON3,QRAppFrm::WxButton3Click)
  END_EVENT_TABLE()

/*
 * GUI Class Constructor
 */
QRAppFrm::QRAppFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
  CreateGUIControls();
}

/*
 * GUI Class Deconstructor
 */
QRAppFrm::~QRAppFrm()
{
  // Do nothing
}

/*
 * GUI Items Creation
 */
void QRAppFrm::CreateGUIControls()
{
  // Allows different image input types
  wxInitAllImageHandlers();
    
  // Initialization of GUI Items
  WxButton1 = new wxButton(this, ID_WXBUTTON1, _("Choose Image"), wxPoint(455, 30), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton1"));
  WxButton2 = new wxButton(this, ID_WXBUTTON2, _("Pre-Process QR"), wxPoint(455, 60), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton2"));
  WxButton3 = new wxButton(this, ID_WXBUTTON3, _("Save Output"), wxPoint(455, 90), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton3"));
  WxButton2->Enable(false);
  WxButton3->Enable(false);
  WxFileDialog1 =  new wxFileDialog(this, _("Choose Image"), _(""), _(""), _("*.*"), wxFD_OPEN);
  WxSaveFileDialog1 =  new wxFileDialog(this, _("Save Processed Image"), _(""), _(""), _("JPG files (*.jpg)|*.jpg|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp"), wxFD_SAVE);
  WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(15, 33), wxSize(200, 200) );
  WxStaticBitmap2 = new wxStaticBitmap(this, ID_WXSTATICBITMAP2, wxNullBitmap, wxPoint(240, 33), wxSize(200, 200) );
  WxMessageDialog1 =  new wxMessageDialog(this, _(""), _("Message box"));
  WxStaticBox1 = new wxStaticBox(this, ID_WXSTATICBOX1, _("Input Image"), wxPoint(6, 15), wxSize(216, 226));
  WxStaticBox2 = new wxStaticBox(this, ID_WXSTATICBOX2, _("Processed Output"), wxPoint(231, 15), wxSize(216, 226));
  	
  // GUI Window Settings
  SetTitle(_("QRApp"));
  SetIcon(wxNullIcon);
  SetSize(8,8,560,290);
  Center();
}

/*
 * Destroys GUI Window when User Closes
 */
void QRAppFrm::OnClose(wxCloseEvent& event)
{
  Destroy();
}

/*
 * Choose Image from File (WxButton1 Click)
 */
void QRAppFrm::WxButton1Click(wxCommandEvent& event)
{   
  // Open File Dialog 
  WxFileDialog1->ShowModal();
    
  if (WxFileDialog1->GetPath().IsEmpty())
  {
    // Invoke wxWidgets Built-in Error Handler
    return;
  }
    
  // Load Image
  imgState = imgInput.LoadFile(WxFileDialog1->GetPath(), wxBITMAP_TYPE_ANY);

  if (imgState)
  {
    // Get Loaded Image Dimensions
    imgHeight = imgInput.GetHeight();
    imgWidth = imgInput.GetWidth();
        
    // Display Loaded Image to Screen
    if ((imgHeight*200/imgWidth) <= 200)
    {    
      WxStaticBitmap1->SetBitmap(imgInput.Scale(200,imgHeight*200/imgWidth));
    }
     
    else
    {
      WxStaticBitmap1->SetBitmap(imgInput.Scale(imgWidth*200/imgHeight,200));
    }
  }
  
  WxButton2->Enable(true);
}

/*
 * Filters Output Pattern Finder
 */
bool isExistent(vector<int> points, int incomingX, int incomingY)
{
  for (int x = 0, y = 1; x <= points.size()-2 && y <= points.size(); x+=2, y+=2)
  {
    if(points.at(x)*0.5 <= incomingX && points.at(x)*1.5 >= incomingX &&
       points.at(y)*0.5 <= incomingY && points.at(y)*1.5 >= incomingY)
    {
      return true;
    }
  }
  
  return false;
}

/*
 * Arranges the Points accordingly starting form the top-left corner followed by
 * the top-right corner and finally the bottom-left corner
 */
std::vector<int> arrangePoints(vector<int> center)
{
  vector<int> xValues;
  vector<int> yValues;
  for(int index = 0; index < center.size(); index += 2)
  {
    xValues.push_back(center.at(index));
    yValues.push_back(center.at(index + 1));
  }
  
  center.clear();
  int nearestIndex = 0;
  int nearestDistance = xValues.at(0)*xValues.at(0) + yValues.at(0)*yValues.at(0);
  for(int index = 0; index < xValues.size(); index++)
  {
    if(nearestDistance > xValues.at(index)*xValues.at(index) + yValues.at(index)*yValues.at(index))
    {
      nearestIndex = index;
    }
  }
  center.push_back(xValues.at(nearestIndex));
  center.push_back(yValues.at(nearestIndex));
  xValues.erase(xValues.begin() + nearestIndex);
  yValues.erase(yValues.begin() + nearestIndex);
    
  if(xValues.at(0) > xValues.at(1))
  {
    center.push_back(xValues.at(0));
    center.push_back(yValues.at(0));
    center.push_back(xValues.at(1));
    center.push_back(yValues.at(1));  
  }
  
  else
  {
    center.push_back(xValues.at(1));
    center.push_back(yValues.at(1));     
    center.push_back(xValues.at(0));
    center.push_back(yValues.at(0));   
  }

  return center;
}

std::vector<int> finderPattern(
                            int patternState, 
                            int** imgPixelArray, 
                            int pixelCount, 
                            int weight, 
                            int centerX, 
                            int centerY, 
                            vector<int> center, 
                            double tolerance,
                            int iteration)
{
  // Checking if 1:1:3:1:1 pattern exists
  for (int aryHeight=0; aryHeight<imgHeight; ++aryHeight)
  {
    for (int aryWidth=0; aryWidth<imgWidth; ++aryWidth)
    {
      // Checks for continuous black patterns and set it as ratio 1
      if (patternState == 0)
      {
        if(imgPixelArray[aryHeight][aryWidth] == 0)
        {
          pixelCount++;
        }
        else
        {
          weight = pixelCount;
          patternState = 1;
          pixelCount = 0;
        }
      }
            
      // Checks for continuous white patterns and check if within ratio 1 range
      else if (patternState == 1)
      {
        if(imgPixelArray[aryHeight][aryWidth] == 1)
        {
          pixelCount++;
        }
        else
        {
          if (weight*(1 - tolerance) <= pixelCount && pixelCount <= weight*(1 + tolerance))
          {
            patternState = 2;
          }
          else
          {
            patternState = 0;
          }
                    
          pixelCount = 0;
        }
      }
            
      // Checks for continuous black patterns and check if within ratio 3 range
      else if (patternState == 2)
      {
        if (imgPixelArray[aryHeight][aryWidth] == 0)
        {
          pixelCount++;
        }
        else
        {
          if (weight*(3 - tolerance) <= pixelCount && pixelCount <= weight*(3 + tolerance))
          {
            patternState = 3;
          }
          else
          {
            patternState = 0;
          }
                    
          pixelCount = 0;
        }
      }
            
      // Checks for continuous white patterns and check if within ratio 1 range
      else if (patternState == 3)
      {
        if (imgPixelArray[aryHeight][aryWidth] == 1)
        {
          pixelCount++;
        }
        else
        {
          if (weight*(1 - tolerance) <= pixelCount && pixelCount <= weight*(1 + tolerance))
          {
            patternState = 4;
          }
          else
          {
            patternState = 0;
          }
                    
          pixelCount = 0;
        }
      }
            
      // Checks for continuous black patterns and check if within ratio 1 range
      else if (patternState == 4)
      {
        if (imgPixelArray[aryHeight][aryWidth] == 0)
        {
          pixelCount++;
        }
        else
        {
          if (weight*(1 - tolerance) <= pixelCount && pixelCount <= weight*(1 + tolerance))
          {   
            int patternStateChk = 0;
            int weightChk = 0;
            int pixelChkCount = 0;
                        
            // Temporarily Store Center of Detected Horizontal Finder Pattern
            centerX = aryWidth - weight*3.5;
            centerY = aryHeight;
                       
            // Check if Horizontal Center is Center for Vertical Finder Pattern
            for (int verticalChkIndex = centerY; verticalChkIndex < imgHeight; ++verticalChkIndex)
            {
              // Check for continuous black pattern below center to get vertical weight
              if (patternStateChk == 0)
              {
                if (imgPixelArray[verticalChkIndex][centerX]==0)
                {
                  pixelChkCount++;
                }
                else
                {
                  weightChk = pixelChkCount;
                  patternStateChk = 1;
                  pixelChkCount = 0;
                }
              }
                            
              // Check for continuous white pattern and compare weights
              else if (patternStateChk == 1)
              {
                if(imgPixelArray[verticalChkIndex][centerX]==1)
                {
                  pixelChkCount++;
                }
                else
                {
                  if (weightChk*(2 - tolerance) <= pixelChkCount && pixelChkCount <= weightChk*(2 + tolerance))
                  {
                    patternStateChk = 2;
                  }
                  else
                  {
                    patternStateChk = 0;
                  }
                                    
                  pixelChkCount = 0;
                }
              }
                            
              // Check for continuous black pattern and compare weight
              else if (patternStateChk == 2)
              {
                if(imgPixelArray[verticalChkIndex][centerX]==0)
                {
                  pixelChkCount++;
                }
                else
                {                   
                  if (weightChk*(2 - tolerance) <= pixelChkCount && pixelChkCount <= weightChk*(2 + tolerance))
                  {
                    // Store the centroid in (X,Y) alternating intervals format
                    if (center.size() < 1)
                    {
                      center.push_back(centerX);
                      center.push_back(centerY);
                    }
                    else if (center.size() >= 1)
                    {
                      if(isExistent(center, centerX, centerY) == false)
                      {
                        center.push_back(centerX);
                        center.push_back(centerY);
                      }                        
                    }
                  }
                                    
                  patternStateChk = 0;
                  pixelChkCount = 0;
                  weightChk = 0;
                }
              }
            }
          }
                    
          patternState = 0;
          pixelCount = 0;
          weight = 0;
        }
      }
    }
  }
   
  /* 
  * Recursive function kills itself if points could not be narrowed down
  * after 100 iterations
  */
  if(iteration > 100)
  {
    return center;
  }
  
  if(center.size() < 6)
  {
    patternState = 0;
    pixelCount = 0;
    weight = 0;
    centerX = 0;
    centerY = 0;
    center.clear();
    
    return finderPattern(
            patternState, 
            imgPixelArray, 
            pixelCount, 
            weight, 
            centerX, 
            centerY, 
            center, 
            tolerance - 0.1, 
            iteration + 1
    );
  }
  
  else if(center.size() > 6)
  {
    patternState = 0;
    pixelCount = 0;
    weight = 0;
    centerX = 0;
    centerY = 0;
    center.clear();
    return finderPattern(
            patternState, 
            imgPixelArray, 
            pixelCount, 
            weight, 
            centerX, 
            centerY, 
            center, 
            tolerance + 0.1, 
            iteration + 1
    );
  }
  else
  {
    return center;
  }
}



/*
 * Pre-Process QR
 */
void QRAppFrm::WxButton2Click(wxCommandEvent& event)
{
  if (imgState)
  {
    imgOutput.Create(imgWidth,imgHeight);
    int currentPixelVal;
    int patternState = 0;
    int pixelCount = 0;
    int weight = 0;
    int centerX = 0;
    int centerY = 0;
    vector<int> center;
             
    //Dynamic Allocation for 2D Array of Pixel Values
    int** imgPixelArray = new int*[imgHeight];
    for(int dynamicIndex = 0; dynamicIndex < imgHeight; ++dynamicIndex)
    {
      imgPixelArray[dynamicIndex] = new int[imgWidth];
    }
        

    for (int heightIndex=0; heightIndex<imgHeight; ++heightIndex)
    {
      for (int widthIndex=0; widthIndex<imgWidth; ++widthIndex)
      {
        // Getting RGB Values per Pixel
        int red = (imgInput.GetRed(widthIndex,heightIndex));
        int green = (imgInput.GetGreen(widthIndex,heightIndex));
        int blue = (imgInput.GetBlue(widthIndex,heightIndex));

        // Grayscaling (using Formula)
        int gray = (red * 0.2126) + (green * 0.7152) + (blue * 0.0722);
                
        // Binarization Thresholding
        if (gray < 128)
        {
          // Turn Pixel to Black, Set Pixel Value as 0
          imgOutput.SetRGB(widthIndex, heightIndex, 0, 0, 0);
          currentPixelVal = 0;
        }
                
        else
        {
          // Turn Pixel to White, Set Pixel Value as 1
          imgOutput.SetRGB(widthIndex, heightIndex, 255, 255, 255);
          currentPixelVal = 1;
        }
                
        // Store Binarized Pixel Value
        imgPixelArray[heightIndex][widthIndex] = currentPixelVal;
      }
    }

    center = finderPattern(
            patternState, 
            imgPixelArray, 
            pixelCount, 
            weight, 
            centerX, 
            centerY, 
            center, 
            0.6, 
            0
    );
        
    /*    
    // Use this code to check if centers are correctly positioned
    ofstream output_file("./datapoints.txt");
    ostream_iterator<int> output_iterator(output_file, " ");
    copy(center.begin(), center.end(), output_iterator);
    */
    
    if(center.size() > 6)
    {
      WxMessageDialog1 =  new wxMessageDialog(this, _("Error G6: Program detected no QR Code"), _("Error"));
      WxMessageDialog1->ShowModal();
      return;
    }
    
    else if(center.size() < 6)
    {
      WxMessageDialog1 =  new wxMessageDialog(this, _("Error L6: Program detected no QR Code"), _("Error"));
      WxMessageDialog1->ShowModal();
      return;
    }
    
    else if(center.size() == 6)
    {
      center = arrangePoints(center);
            
      // START OF PERSPECTIVE TRANSFORMATION
      // Perspective Transform initialization
      PerspectiveTransform testGivenMatrix(
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
      );
                        
      double x0 = center[0]*1.0;
      double x1 = center[2]*1.0;         
      double x3 = center[4]*1.0;
      double x2 = (x3 + (x1 - x0))*1.0;
                
      double y0 = center[1]*1.0;
      double y1 = center[3]*1.0;
      double y3 = center[5]*1.0;
      double y2 = (y3 + (y1 - y0))*1.0;
            
      //Dynamic Allocation for 2D Array of Pixel Values
      imgPixelArray = testGivenMatrix.reverseWarp(
                x0, y0,
                x1, y1,
                x3, y3
      ).transformPoints(imgPixelArray, imgWidth, imgHeight);
            
      for (int heightIndex=0; heightIndex<imgHeight; ++heightIndex)
      {
        for (int widthIndex=0; widthIndex<imgWidth; ++widthIndex)
        {
          // Binarization Thresholding
          if (imgPixelArray[heightIndex][widthIndex] == 0)
          {
            // Turn Pixel to Black, Set Pixel Value as 0
            imgOutput.SetRGB(widthIndex, heightIndex, 0, 0, 0);
          }
                    
          else
          {
            // Turn Pixel to White, Set Pixel Value as 1
            imgOutput.SetRGB(widthIndex, heightIndex, 255, 255, 255);
          }
        }
      }
      // END OF PERSPECTIVE TRANSFORM
      
      // Output Final Image
      if (200>=(imgHeight*200/imgWidth))
      {
        WxStaticBitmap2->SetBitmap(imgOutput.Scale(200,imgHeight*200/imgWidth));
      }
        
      else
      {
        WxStaticBitmap2->SetBitmap(imgOutput.Scale(imgWidth*200/imgHeight,200));
      }
    }
  }
  
  WxButton3->Enable(true);
}


/*
 * Save Processed Image
 */
void QRAppFrm::WxButton3Click(wxCommandEvent& event)
{
  if (WxSaveFileDialog1->ShowModal() == wxID_OK) {
    imgOutput.SaveFile(WxSaveFileDialog1->GetPath());
  }
  else
  {
    WxSaveFileDialog1->Close();
  }
}
