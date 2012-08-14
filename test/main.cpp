/*
 * main.cpp
 *
 * GUIのテスト用プログラム
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#include <wx/wx.h>
#include "wx/wxnkf.h"

class MyApp: public wxApp {
public:
	virtual bool OnInit();
};

class MyFrame: public wxFrame {
public:
	MyFrame(const wxString& title);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnConvertFTF(wxCommandEvent& event);
	void OnConvertFTM(wxCommandEvent& event);

private:
	wxTextCtrl* m_tc;DECLARE_EVENT_TABLE()
};

enum {
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT,
	wxID_CONVERTFILETOFILE,
	wxID_CONVERTFILETOMEM
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
EVT_BUTTON(wxID_CONVERTFILETOFILE, MyFrame::OnConvertFTF)
EVT_BUTTON(wxID_CONVERTFILETOMEM, MyFrame::OnConvertFTM)
END_EVENT_TABLE()
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
	return false;

	MyFrame *frame = new MyFrame(wxT("libnkfをwxWidgetsから使用するテスト"));
	frame->Show(true);
	return true;
}

MyFrame::MyFrame(const wxString& title) :
		wxFrame(NULL, wxID_ANY, title) {

	wxMenu *fileMenu = new wxMenu;
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, wxT("LibNKFをスタートさせる..."),
			wxT("LibNKFをスタートさせる"));
	fileMenu->Append(Minimal_Quit, wxT("終了"), wxT("プログラムを終了させる"));
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");
	SetMenuBar(menuBar);

	// とりあえずテキストで画面を埋めておく
	wxPanel* panel = new wxPanel(this, -1);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_tc = new wxTextCtrl(panel, wxID_ANY);
	sizer->Add(m_tc, 1, wxEXPAND);

	// bbox　ボタン配置部分
	wxBoxSizer *bbox = new wxBoxSizer(wxHORIZONTAL);
	// ファイル→ファイル
	wxButton *btn1 = new wxButton(panel, wxID_CONVERTFILETOFILE,
			wxT("ファイルの変換処理"));
	bbox->Add(btn1, 0);
	// ファイル→テキストコントロール
	wxButton *btn2 = new wxButton(panel, wxID_CONVERTFILETOMEM,
			wxT("ファイルからメモリへ"));
	bbox->Add(btn2, 0, wxLEFT | wxBOTTOM, 5);
	sizer->Add(bbox, 0, wxALIGN_RIGHT | wxRIGHT, 10);

	panel->SetSizer(sizer);

	CreateStatusBar(2);
	SetStatusText("ようこそwxWidgetsへ！");

}
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}

void MyFrame::OnConvertFTF(wxCommandEvent& WXUNUSED(event)) {

	wxMessageBox(wxT("ファイルから別のファイルにデータを変換します"));

	wxString inputFilePath;
	wxString outputFilePath;

#if __WXMAC__
	inputFilePath = wxT("/Users/hiroyuki/git/libnkf/test/CP932.txt");
	outputFilePath = wxT("/Users/hiroyuki/git/libnkf/test/UTF-8.txt");
#elif __WXMSW__
	inputFilePath = wxT("C:\\Users\\learning\\git\\libnkf\\test\\CP932.txt");
	outputFilePath = wxT("C:\\Users\\learning\\git\\libnkf\\test\\UTF-8.txt");
#else
#endif

	wxNKF* nkf = new wxNKF();
	nkf->Convert(inputFilePath, outputFilePath, wxT("--ic=CP932 --oc=UTF-8"));
	delete nkf;
}

void MyFrame::OnConvertFTM(wxCommandEvent& WXUNUSED(event)) {

	wxMessageBox(wxT("ファイルからwxString内にデータをコピーします"));

	wxString inputFilePath;
	wxString outputFilePath;

#if __WXMAC__
	inputFilePath = wxT("/Users/hiroyuki/git/libnkf/test/CP932.txt");
	outputFilePath = wxT("/Users/hiroyuki/git/libnkf/test/UTF-8.txt");
#else
	inputFilePath = wxT("CP932.txt");
	outputFilePath = wxT("UTF-8.txt");
#endif

	wxNKF* nkf = new wxNKF();
	wxString test = nkf->Convert(inputFilePath, wxT("--ic=CP932 --oc=UTF-8"));
	m_tc->SetValue(test);
	delete nkf;
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(wxT("これはwxNKFのテストプログラムです"));
}
