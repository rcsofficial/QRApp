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

bool imgState = false;
wxImage imgInput;
wxImage imgView;
wxImage imgOutput;
int imgHeight;
int imgWidth;

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
	WxFileDialog1->ShowModal();
    if (WxFileDialog1->GetPath().IsEmpty())
    {
        return;
    }
    
    imgState = imgInput.LoadFile(WxFileDialog1->GetPath(), wxBITMAP_TYPE_ANY);
    imgHeight = imgInput.GetHeight();
    imgWidth = imgInput.GetWidth();
    	
    if ((imgHeight*200/imgWidth) <= 200)
    {    
        WxStaticBitmap1->SetBitmap(imgInput.Scale(200,imgHeight*200/imgWidth));
    }
     
    else
    {
        WxStaticBitmap1->SetBitmap(imgInput.Scale(imgWidth*200/imgHeight,200));
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
        
        for (int widthIndex=0; widthIndex<imgWidth; widthIndex++)
        {
            for (int heightIndex=0; heightIndex<imgHeight; heightIndex++)
            {
                int red = (imgInput.GetRed(widthIndex,heightIndex));
                int green =(imgInput.GetGreen(widthIndex,heightIndex));
                int blue = (imgInput.GetBlue(widthIndex,heightIndex));

                // Grayscaling (Formula)
                int gray = (red * 0.2126) + (green * 0.7152) + (blue * 0.0722);
                
                // Binarization (Formula)
                if (gray < 128)
                {
                    imgOutput.SetRGB(widthIndex, heightIndex, 0, 0, 0);
                }
                else
                {
                    imgOutput.SetRGB(widthIndex, heightIndex, 255, 255, 255);
                }
                
                // Finder Pattern
                
                // Perspective Transformation
                
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


    /*
    if (Insert Error Here) {
       WxMessageDialog1 =  new wxMessageDialog(this, _("Insert Error Code"), _("Error"));
	   WxMessageDialog1->ShowModal();
    }
    */
    
}
