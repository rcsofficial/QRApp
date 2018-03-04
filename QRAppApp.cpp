//---------------------------------------------------------------------------
//
// Name:        QRAppApp.cpp
// Author:      Richmond Sim
// Created:     04/03/2018 6:11:42 PM
// Description: 
//
//---------------------------------------------------------------------------

#include "QRAppApp.h"
#include "QRAppFrm.h"

IMPLEMENT_APP(QRAppFrmApp)

bool QRAppFrmApp::OnInit()
{
    QRAppFrm* frame = new QRAppFrm(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int QRAppFrmApp::OnExit()
{
	return 0;
}
