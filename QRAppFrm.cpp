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
	WxButton1 = new wxButton(this, ID_WXBUTTON1, _("Choose Image"), wxPoint(225, 35), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton1"));
	WxButton2 = new wxButton(this, ID_WXBUTTON2, _("Pre-Process QR"), wxPoint(225, 65), wxSize(88, 25), 0, wxDefaultValidator, _("WxButton2"));
    WxFileDialog1 =  new wxFileDialog(this, _("Choose Image"), _(""), _(""), _("*.*"), wxFD_OPEN);	
	WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(9, 33), wxSize(300, 300) );
	
	// GUI Window Settings
	SetTitle(_("QRApp"));
	SetIcon(wxNullIcon);
	SetSize(8,8,342,334);
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
    
    imgView.Create(imgHeight, imgWidth);
    
    for (int i=0; i<imgHeight; i++)
	{
		for (int j=0; j<imgWidth; j++)
		{
            imgView.SetRGB(i, j, 240, 240, 240);
		}
	}
	
    if ((imgHeight*200/imgWidth) <= 200)
    {    
        WxStaticBitmap1->SetBitmap(imgView.Scale(200,imgHeight*200/imgWidth));
        WxStaticBitmap1->SetBitmap(imgInput.Scale(200,imgHeight*200/imgWidth));
    }
     
    else
    {
        WxStaticBitmap1->SetBitmap(imgView.Scale(imgWidth*200/imgHeight,200));
        WxStaticBitmap1->SetBitmap(imgInput.Scale(imgWidth*200/imgHeight,200));
    }
}

/*
 * Pre-Process QR
 */
void QRAppFrm::WxButton2Click(wxCommandEvent& event)
{
    //imgInput.GetRed(20,20) == 20;
}
