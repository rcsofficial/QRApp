///-----------------------------------------------------------------
///
/// @file      QRAppFrm.h
/// @author    Richmond Sim
/// Created:   13/02/2018 10:52:28 AM
/// @section   DESCRIPTION
///            QRAppFrm class declaration
///
///------------------------------------------------------------------

#ifndef __QRAPPFRM_H__
#define __QRAPPFRM_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

////Header Include
#include <wx/button.h>

//Dialog Style
#undef QRAppFrm_STYLE
#define QRAppFrm_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX

class QRAppFrm : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		QRAppFrm(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("QRApp"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = QRAppFrm_STYLE);
		virtual ~QRAppFrm();
		void WxButton1Click(wxCommandEvent& event);
		
	private:
		//GUI Control Declaration
		wxButton *WxButton1;
		wxFileDialog *WxFileDialog1;
		wxStaticBitmap *WxStaticBitmap1;
		
	private:
		enum
		{
			////GUI Enum Control ID
			ID_WXBUTTON1 = 1001,
			ID_WXSTATICBITMAP1 = 1002,
			ID_DUMMY_VALUE_
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
