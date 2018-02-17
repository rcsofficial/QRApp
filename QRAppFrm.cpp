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
        vector<int> pixelVector;
        vector<int> pixelCmprsVector;
        int pixelTemp = 1;
        
        for (int heightIndex=0; heightIndex<imgHeight; heightIndex++)
        {
            for (int widthIndex=0; widthIndex<imgWidth; widthIndex++)
            {
                // Storing RGB Value
                int red = (imgInput.GetRed(widthIndex,heightIndex));
                int green =(imgInput.GetGreen(widthIndex,heightIndex));
                int blue = (imgInput.GetBlue(widthIndex,heightIndex));

                // Grayscaling (Formula)
                int gray = (red * 0.2126) + (green * 0.7152) + (blue * 0.0722);
                
                // Binarization (Formula)
                if (gray < 128)
                {
                    // Turn Pixel to Black, Store Pixel Value in Vector as 0
                    imgOutput.SetRGB(widthIndex, heightIndex, 0, 0, 0);
                    pixelVector.push_back(0);
                }
                else
                {
                    // Turn Pixel to White, Store Pixel Value in Vector as 1
                    imgOutput.SetRGB(widthIndex, heightIndex, 255, 255, 255);
                    pixelVector.push_back(1);
                }
            }            
        }
        
        // Pixel Data Compression Vector
        for (unsigned int pixelIndex = 0; pixelIndex < pixelVector.size()-1; pixelIndex++)
        {
            if (pixelVector.at(pixelIndex) == pixelVector.at(pixelIndex+1))
            {
                pixelTemp++;
            }
            else if (pixelVector.at(pixelIndex) != pixelVector.at(pixelIndex+1))
            {
                pixelCmprsVector.push_back(pixelTemp);
                pixelTemp = 1;
            }
            
            if (pixelIndex == pixelVector.size()-2)
            {
                pixelCmprsVector.push_back(pixelTemp);
                pixelTemp = 1;
            }
        }

        // Finding Pattern
        if (pixelCmprsVector.size() <= 6)
        {
            WxMessageDialog1 =  new wxMessageDialog(this, _("Image has less than 6 pixel variations, impossible to contain QR Code."), _("Error"));
	        WxMessageDialog1->ShowModal();
        }
        else
        {
            for (unsigned int pixelCmprsIndex = 0; pixelCmprsIndex < pixelCmprsVector.size()-6; pixelCmprsIndex++)
            {
                int pixelCmprsA = pixelCmprsVector.at(pixelCmprsIndex);
                int pixelCmprsB = pixelCmprsVector.at(pixelCmprsIndex+1);
                int pixelCmprsC = pixelCmprsVector.at(pixelCmprsIndex+2);
                int pixelCmprsD = pixelCmprsVector.at(pixelCmprsIndex+3);
                int pixelCmprsE = pixelCmprsVector.at(pixelCmprsIndex+4);

                // Finding Ratio 1:1:3:1:1
                if(pixelCmprsA*0.90 <= pixelCmprsB && pixelCmprsB <= pixelCmprsA*1.1)
                {
                    if(pixelCmprsA*1.90 <= pixelCmprsC && pixelCmprsC <= pixelCmprsA*4.1)
                    {
                        if(pixelCmprsA*0.90 <= pixelCmprsD && pixelCmprsD <= pixelCmprsA*1.1)
                        {
                            if(pixelCmprsA*0.90 <= pixelCmprsE && pixelCmprsE <= pixelCmprsA*1.1)
                            {
                                // Finding Color Combination
                                if ((pixelVector.at(0)==0 && pixelCmprsIndex % 2 == 0) ||
                                    (pixelVector.at(0)==1 && pixelCmprsIndex % 2 == 1))
                                {
                                    // TODO: Store these data points
                                    // TODO: Verify recurrence on succeeding rows (outside this loop)
                                    // TODO: Find centroids (outside this loop)
                                }
                                else
                                {
                                    WxMessageDialog2 =  new wxMessageDialog(this, _("No QR Pattern Finder Detected."), _("Alert"));
	                                WxMessageDialog2->ShowModal();
                                }
	                            return;
                            }
                        }
                    }
                }
            }
        }

        
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




    //#include<fstream>
    /*ofstream output_file("./datapoints.txt");
    ostream_iterator<int> output_iterator(output_file, " ");
    copy(pixelCmprsVector.begin(), pixelCmprsVector.end(), output_iterator);*/
    
    /*
    if (Insert Error Here) {
        WxMessageDialog1 =  new wxMessageDialog(this, _("Unrecognized file type. Please select supported image formats."), _("Error"));
	    WxMessageDialog1->ShowModal();
    }
    */
