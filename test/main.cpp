/*
 * main.cpp
 *
 * GUIのテスト用プログラム
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#include <wx/wx.h>

class MyApp: public wxApp {
public:
	virtual bool OnInit();
};
class MyFrame: public wxFrame {
public:
	MyFrame(const wxString& title);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
private:
	DECLARE_EVENT_TABLE()
};
enum {
	Minimal_Quit = wxID_EXIT, Minimal_About = wxID_ABOUT
};
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
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

	CreateStatusBar(2);
	SetStatusText("ようこそwxWidgetsへ！");

}
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
//	LibNKF* nkf = new LibNKF();
//	std::wstring c1 = nkf->Convert("./sjis.txt", "--ic=CP932 --oc=UTF-8");
//
//	wchar_t* wc = (wchar_t*) c1.c_str();
//	wxString c2(wc);
//	wxMessageBox(c2);
//
//	delete nkf;
}
