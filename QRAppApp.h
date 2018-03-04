//---------------------------------------------------------------------------
//
// Name:        QRAppApp.h
// Author:      Richmond Sim
// Created:     04/03/2018 6:11:42 PM
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
