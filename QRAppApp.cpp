//---------------------------------------------------------------------------
//
// Name:        QRAppApp.cpp
// Author:      Richmond Sim
// Created:     13/02/2018 10:52:28 AM
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
