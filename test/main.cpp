/*
 * main.cpp
 *
 * GUIのテスト用プログラム
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#include <wx/wx.h>
#include <wx/filedlg.h>
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
	void OnSetInputFile(wxCommandEvent& event);
	void OnOutputFile(wxCommandEvent& event);

private:
	wxTextCtrl* m_tc;
	wxTextCtrl* m_inputFileName;
	wxTextCtrl* m_outputFileName;

	DECLARE_EVENT_TABLE()
};

enum {
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT,
	wxID_CONVERTFILETOFILE,
	wxID_CONVERTFILETOMEM,
	wxID_SETINPUTFILE,
	wxID_SETOUTPUTFILE
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
EVT_BUTTON(wxID_CONVERTFILETOFILE, MyFrame::OnConvertFTF)
EVT_BUTTON(wxID_CONVERTFILETOMEM, MyFrame::OnConvertFTM)
EVT_BUTTON(wxID_SETINPUTFILE, MyFrame::OnSetInputFile)
EVT_BUTTON(wxID_SETOUTPUTFILE, MyFrame::OnOutputFile)
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

	SetSize(wxSize(960, 640));

	wxMenu *fileMenu = new wxMenu;
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, wxT("LibNKFをスタートさせる..."),
			wxT("LibNKFをスタートさせる"));
	fileMenu->Append(Minimal_Quit, wxT("終了"), wxT("プログラムを終了させる"));
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));
	SetMenuBar(menuBar);

	// とりあえずテキストで画面を埋めておく
	wxPanel* panel = new wxPanel(this, -1);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// 入力ファイル設定用ダイアログボックス
	wxBoxSizer* ibox = new wxBoxSizer(wxHORIZONTAL);
	m_inputFileName = new wxTextCtrl(panel, wxID_ANY);
	ibox->Add(m_inputFileName, 1, wxEXPAND);
	wxButton* iButton = new wxButton(panel, wxID_SETINPUTFILE, wxT("参照"));
	ibox->Add(iButton, 0, wxALIGN_RIGHT);
	sizer->Add(ibox, 0, wxEXPAND);

	// 出力ファイル設定用ダイアログボックス
	wxBoxSizer* obox = new wxBoxSizer(wxHORIZONTAL);
	m_outputFileName = new wxTextCtrl(panel, wxID_ANY);
	obox->Add(m_outputFileName, 1, wxEXPAND);
	wxButton* oButton = new wxButton(panel, wxID_SETOUTPUTFILE, wxT("参照"));
	obox->Add(oButton, 0, wxALIGN_RIGHT);
	sizer->Add(obox, 0, wxEXPAND);

	// 変換後のテキストを表示するテキストコントロール
	m_tc = new wxTextCtrl(panel, wxID_ANY);
	sizer->Add(m_tc, 1, wxEXPAND);

	// bbox　変換処理ボタン配置部分
	wxBoxSizer *bbox = new wxBoxSizer(wxHORIZONTAL);
	// ファイル→ファイル
	wxButton *btn1 = new wxButton(panel, wxID_CONVERTFILETOFILE,
			wxT("ファイルからファイルへの変換処理"));
	bbox->Add(btn1, 0);
	// ファイル→テキストコントロール
	wxButton *btn2 = new wxButton(panel, wxID_CONVERTFILETOMEM,
			wxT("ファイルからメモリへの変換処理"));
	bbox->Add(btn2, 0, wxLEFT | wxBOTTOM, 5);
	sizer->Add(bbox, 0, wxALIGN_RIGHT | wxRIGHT, 10);

	panel->SetSizer(sizer);

	CreateStatusBar(2);
	SetStatusText(wxT("ようこそwxWidgetsへ！"));

}
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}

void MyFrame::OnConvertFTF(wxCommandEvent& WXUNUSED(event)) {

	wxString inputFilePath = m_inputFileName->GetValue();
	wxString outputFilePath = m_outputFileName->GetValue();

	wxNKF* nkf = new wxNKF();
	nkf->Convert(inputFilePath, outputFilePath, wxT("--ic=CP932 --oc=UTF-8"));
	delete nkf;
}

void MyFrame::OnConvertFTM(wxCommandEvent& WXUNUSED(event)) {

	wxString inputFilePath = m_inputFileName->GetValue();
	wxString outputFilePath = m_outputFileName->GetValue();

	wxNKF* nkf = new wxNKF();
	wxString test = nkf->Convert(inputFilePath, wxT("--ic=CP932 --oc=UTF-8"));
	wxMessageBox(test);
	m_tc->SetValue(test);
	delete nkf;
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(wxT("これはwxNKFのテストプログラムです"));
}

void MyFrame::OnSetInputFile(wxCommandEvent& event) {
	wxString caption = wxT("入力ファイルの設定");
	wxString defaultDir = wxGetCwd();
	wxString wildCard = wxT("txt files (*.txt) |*.txt");

	wxFileDialog dialog(this, caption, defaultDir, wxEmptyString, wildCard,
			wxFD_OPEN);
	if (dialog.ShowModal() == wxID_OK) {
		m_inputFileName->SetValue(dialog.GetPath());
	}
}

void MyFrame::OnOutputFile(wxCommandEvent& event) {
	wxString caption = wxT("出力ファイルの設定");
	wxString defaultDir = wxGetCwd();
	wxString wildCard = wxT("txt files (*.txt) |*.txt");

	wxFileDialog dialog(this, caption, defaultDir, wxEmptyString, wildCard,
			wxFD_OPEN);
	if (dialog.ShowModal() == wxID_OK) {
		m_outputFileName->SetValue(dialog.GetPath());
	}
}
