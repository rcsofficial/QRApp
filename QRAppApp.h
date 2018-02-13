//---------------------------------------------------------------------------
//
// Name:        QRAppApp.h
// Author:      Richmond Sim
// Created:     13/02/2018 10:52:28 AM
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __QRAPPFRMApp_h__
#define __QRAPPFRMApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class QRAppFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
