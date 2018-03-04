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
#include <vector>
#include <numeric>

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
	WxButton1 = new wxButton(this, ID_WXBUTTON1, _("Choose Image"), wxPoint(460, 35), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton1"));
	WxButton2 = new wxButton(this, ID_WXBUTTON2, _("Pre-Process QR"), wxPoint(460, 65), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton2"));
    WxFileDialog1 =  new wxFileDialog(this, _("Choose Image"), _(""), _(""), _("*.*"), wxFD_OPEN);	
	WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(9, 33), wxSize(200, 200) );
	WxStaticBitmap2 = new wxStaticBitmap(this, ID_WXSTATICBITMAP2, wxNullBitmap, wxPoint(224, 33), wxSize(200, 200) );
	WxMessageDialog1 =  new wxMessageDialog(this, _(""), _("Message box"));
	
	// GUI Window Settings
	SetTitle(_("QRApp"));
	SetIcon(wxNullIcon);
	SetSize(8,8,590,330);
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
                        if (weight*0.5 <= pixelCount && pixelCount <= weight*1.5)
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
                        if (weight*2.5 <= pixelCount && pixelCount <= weight*3.5)
                        {
                            patternState = 3;
                        }
                        else {
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
                        if (weight*0.5 <= pixelCount && pixelCount <= weight*1.5)
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
                        if (weight*0.5 <= pixelCount && pixelCount <= weight*1.5)
                        {   
                            int patternStateChk = 0;
                            int weightChk = 0;
                            int pixelChkCount = 0;
                            
                            // Store Center of Detected Horizontal Finder Pattern
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
                                        if (weightChk*1.5 <= pixelChkCount && pixelChkCount <= weightChk*2.5)
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
                                        // Add 0.1 more precision to get exact centers
                                        if (weightChk*1.6 <= pixelChkCount && pixelChkCount <= weightChk*2.4)
                                        {
                                            // Store the centroid in (X,Y) alternating intervals format
                                            center.push_back(centerX);
                                            center.push_back(centerY);
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
        Use this code to check if centers are correctly positioned:
            
        #include <fstream>
        
        ofstream output_file("./datapoints.txt");
        ostream_iterator<int> output_iterator(output_file, " ");
        copy(center.begin(), center.end(), output_iterator);
        */
        
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
