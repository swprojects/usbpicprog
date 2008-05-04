#include "uppmainwindow_callback.h"
#include "uppmainwindow.h"




void UppMainWindowCallBack::printHexFile()
{
	int lineSize;
	char txt[256];
	uppHexEdit->Clear();
	uppHexEdit->Freeze();
	
	uppHexEdit->AppendText(wxT("Code Memory\n"));
	for(int i=0;i<readHexFile->getCodeMemory().size();i+=16)
	{
		sprintf(txt,"%00000008X::",i);
		uppHexEdit->AppendText(wxString::FromAscii(txt));
		if(i+16<readHexFile->getCodeMemory().size())
		{
			lineSize=16;
		}
		else
		{
			lineSize=readHexFile->getCodeMemory().size()-i;
		}
		for(int j=0;j<lineSize;j++)
		{
			sprintf(txt,"%02X",readHexFile->getCodeMemory()[i+j]);
			uppHexEdit->AppendText(wxString::FromAscii(txt));
		}
		uppHexEdit->AppendText(wxT("\n"));	
	}
	uppHexEdit->AppendText(wxT("\nConfig Memory\n"));
	for(int i=0;i<readHexFile->getConfigMemory().size();i+=16)
	{
		sprintf(txt,"%00000008X::",i+picType->getCurrentPic().ConfigAddress);
		uppHexEdit->AppendText(wxString::FromAscii(txt));
		if(i+16<readHexFile->getConfigMemory().size())
		{
			lineSize=16;
		}
		else
		{
			lineSize=readHexFile->getConfigMemory().size()-i;
		}
		for(int j=0;j<lineSize;j++)
		{
			sprintf(txt,"%02X",readHexFile->getConfigMemory()[i+j]);
			uppHexEdit->AppendText(wxString::FromAscii(txt));
		}
		uppHexEdit->AppendText(wxT("\n"));
	}
	uppHexEdit->AppendText(wxT("\nData Memory\n"));
	for(int i=0;i<readHexFile->getDataMemory().size();i+=16)
	{
		sprintf(txt,"%00000008X::",i);
		uppHexEdit->AppendText(wxString::FromAscii(txt));
		if(i+16<readHexFile->getDataMemory().size())
		{
			lineSize=16;
		}
		else
		{
			lineSize=readHexFile->getDataMemory().size()-i;
		}
		for(int j=0;j<lineSize;j++)
		{
			sprintf(txt,"%02X",readHexFile->getDataMemory()[i+j]);
			uppHexEdit->AppendText(wxString::FromAscii(txt));
		}
		uppHexEdit->AppendText(wxT("\n"));
	}
	
    uppHexEdit->Thaw();
}


void UppMainWindowCallBack::upp_new()
{
	uppHexEdit->Clear();
	delete readHexFile;
	readHexFile=new ReadHexFile();
	fileOpened=false;
}

void UppMainWindowCallBack::upp_open()
{
	wxFileDialog* openFileDialog =
		new wxFileDialog( this, wxT("Open hexfile"), wxT(""), wxT(""), FILETYPES,
		                  wxOPEN, wxDefaultPosition);
 
	if ( openFileDialog->ShowModal() == wxID_OK )
	{
		upp_open_file(openFileDialog->GetPath());
	}
}


void UppMainWindowCallBack::upp_open_file(wxString path)
{
	
 	if(readHexFile->open(picType,path.mb_str(wxConvUTF8))<0)SetStatusText(wxT("Unable to open file"));
	printHexFile();
	fileOpened=true;
}


void UppMainWindowCallBack::upp_refresh()
{
	if(readHexFile->reload(picType)<0)SetStatusText(wxT("Unable to open file"));
	printHexFile();
}

void UppMainWindowCallBack::upp_save()
{
	if(fileOpened)
    {
        if(readHexFile->save(picType)<0)SetStatusText(wxT("Unable to save file"));
    }
	else upp_save_as();
}

void UppMainWindowCallBack::upp_save_as()
{
	wxFileDialog* openFileDialog =
		new wxFileDialog( this, wxT("Save hexfile"), wxT(""), wxT(""), FILETYPES,
		                  wxSAVE, wxDefaultPosition);
 
	if ( openFileDialog->ShowModal() == wxID_OK )
	{
		if(readHexFile->saveAs(picType,openFileDialog->GetPath().mb_str(wxConvUTF8))<0)
            SetStatusText(wxT("Unable to save file"));
	}
	
	
}

void UppMainWindowCallBack::upp_exit()
{
	Close();
}

void UppMainWindowCallBack::upp_program()
{
	hardware->writeCode(readHexFile,picType);
	hardware->writeData(readHexFile,picType);
	hardware->writeConfig(readHexFile,picType);
}

void UppMainWindowCallBack::upp_read()
{
	hardware->readCode(readHexFile,picType);
	hardware->readData(readHexFile,picType);
	hardware->readConfig(readHexFile,picType);
	printHexFile();
	
}

void UppMainWindowCallBack::upp_verify()
{
	cout<<"Not implemented yet"<<endl;
}

void UppMainWindowCallBack::upp_erase()
{
	hardware->bulkErase();
}

void UppMainWindowCallBack::upp_blankcheck()
{
	cout<<"Not implemented yet"<<endl;
}

void UppMainWindowCallBack::upp_autodetect()
{
	picType=new PicType(hardware->autoDetectDevice());
	hardware->setPicType(picType);
	m_comboBox1->SetValue(wxString::FromAscii(picType->getCurrentPic().Name.c_str()));
	
}

void UppMainWindowCallBack::upp_connect()
{
	hardware=new Hardware();
	if(hardware->connected())
		
		SetStatusText(wxT("Usbpicprog found"));
	else
		SetStatusText(wxT("Usbpicprog not found"));
	upp_autodetect();
}

void UppMainWindowCallBack::upp_disconnect()
{
	if(hardware->connected())
	   {
		   delete hardware;
		   SetStatusText(wxT("Disconnected usbpicprog"));
		   
	   }
	   else
	   {
		   SetStatusText(wxT("Already disconnected"));
	   }
}

void UppMainWindowCallBack::upp_help()
{
	wxLaunchDefaultBrowser(wxT("http://usbpicprog.sourceforge.net/"));
}

void UppMainWindowCallBack::upp_about()
{
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName(wxT("Usbpicprog"));
	aboutInfo.SetVersion(wxT("0.1"));
	aboutInfo.SetDescription(wxT("An open source USB pic programmer"));
	aboutInfo.SetCopyright(wxT("(C) 2008 http://usbpicprog.sourceforge.net/"));
	wxAboutBox(aboutInfo);
}


void UppMainWindowCallBack::upp_combo_changed()
{

	
		picType=new PicType(string(m_comboBox1->GetValue().mb_str(wxConvUTF8)));
		hardware->setPicType(picType);
      	cout << "Name=" << picType->getCurrentPic().Name << std::endl;
    	upp_new();
}



UppMainWindowCallBack::UppMainWindowCallBack(wxWindow* parent, wxWindowID id , const wxString& title , const wxPoint& pos , const wxSize& size , long style ) : UppMainWindow( parent, id, title, pos, size, style )
{
	upp_connect();
	readHexFile=new ReadHexFile();
	for(int i=0;i<picType->getPicNames().size();i++)
	{
		m_comboBox1->Append(wxString::FromAscii(picType->getPicNames()[i].c_str()));
		
	}
	upp_autodetect();
	cout<<"Detected: "<<picType->getCurrentPic().Name<<endl;
	fileOpened=false;
}
