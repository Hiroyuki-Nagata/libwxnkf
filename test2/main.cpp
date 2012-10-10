//============================================================================
// Name        : libwxnkftest2.cpp
// Author      : Hiroyuki Nagata
// Version     : 1.0.0
// Copyright   : Copyright (C) 2012 Hiroyuki Nagata, All rights reserved.
// Description : libwxnkf test program
// Licence     : wxWindows Library Licence, Version 3.1
//============================================================================

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/wxnkf.h"

#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "sample.xpm"
#endif

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
     // コンストラクタ
     MyFrame(wxWindow* parent, int id, const wxString& title,
	     const wxPoint& pos=wxDefaultPosition, 
	     const wxSize& size=wxDefaultSize, 
	     long style=wxDEFAULT_FRAME_STYLE);

     // イベントハンドラ
     void OnQuit(wxCommandEvent& event);
     void OnAbout(wxCommandEvent& event);
     void OnExecuteConv(wxCommandEvent& event);

private:
     // begin wxGlade: MyFrame::methods
     void set_properties(const wxString& title);
     void do_layout();
     // end wxGlade

protected:
     // begin wxGlade: MyFrame::attributes
     wxStaticText* label_1;
     wxComboBox* combo_box_1;
     wxButton* executeButton;
     wxPanel* upPanel;
     wxTextCtrl* inputBox;
     wxTextCtrl* outputBox;
     wxPanel* downPanel;
     // end wxGlade

private:
     DECLARE_EVENT_TABLE()

};

// 操作のためのIDとメニューコマンド
enum
{
    // メニューの項目
    Minimal_Quit = wxID_EXIT,
    Minimal_About = wxID_ABOUT,
    ID_Convert
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
    EVT_BUTTON(ID_Convert, MyFrame::OnExecuteConv)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {

    if ( !wxApp::OnInit() )
        return false;

    // メインのアプリケーションウィンドウを生成する
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, wxT("libwxnkfのテスト"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
     wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE) {

     // アイコンの設定
     SetIcon(wxICON(sample));
     // メニュー等の設定
     wxMenu *fileMenu = new wxMenu;
     wxMenu *helpMenu = new wxMenu;
     helpMenu->Append(Minimal_About, wxT("このプログラムについて...\tF1"), wxT("このプログラムについて"));
     fileMenu->Append(Minimal_Quit, wxT("終了\tAlt-X"), wxT("このプログラムを終了する"));

     wxMenuBar *menuBar = new wxMenuBar();
     menuBar->Append(fileMenu, wxT("&ファイル"));
     menuBar->Append(helpMenu, wxT("&ヘルプ"));

     SetMenuBar(menuBar);

     CreateStatusBar(2);

     // レイアウトを設定する
     // begin wxGlade: MyFrame::MyFrame
     downPanel = new wxPanel(this, wxID_ANY);
     upPanel = new wxPanel(this, wxID_ANY);
     label_1 = new wxStaticText(upPanel, wxID_ANY, wxT("以下に変換したい文字列を入力"));
     
     const wxString choices[] = {
	  wxT("--ic=UTF-16 --oc=Shift_JIS"),
	  wxT("--ic=UTF-16 --oc=EUC-JP"),
	  wxT("--ic=UTF-16 --oc=ISO-2022-JP")
     };
     wxArrayString* combo_box_choices = new wxArrayString(3, choices);

     combo_box_1 = new wxComboBox(upPanel, wxID_ANY, wxT("nkfのオプション"), wxDefaultPosition, wxDefaultSize, *combo_box_choices, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, wxT("テスト"));
     executeButton = new wxButton(upPanel, ID_Convert, wxT("変換実行"));
     inputBox = new wxTextCtrl(downPanel, wxID_ANY, wxEmptyString);
     outputBox = new wxTextCtrl(downPanel, wxID_ANY, wxEmptyString);

     set_properties(title);
     do_layout();
     // end wxGlade
     delete combo_box_choices;
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    // trueならば強制的にフレームを終了する
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}
void MyFrame::set_properties(const wxString& title) {
     // begin wxGlade: MyFrame::set_properties
     SetTitle(title);
     SetSize(wxSize(640, 480));
     label_1->SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("MS Shell Dlg 2")));
     // end wxGlade
}
void MyFrame::do_layout() {
     // begin wxGlade: MyFrame::do_layout
     wxBoxSizer* vBox = new wxBoxSizer(wxVERTICAL);
     wxBoxSizer* textCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
     wxBoxSizer* upSizer = new wxBoxSizer(wxHORIZONTAL);
     upSizer->Add(label_1, 0, wxTOP|wxALIGN_CENTER_VERTICAL, 0);
     upSizer->Add(combo_box_1, 1, wxALIGN_CENTER_VERTICAL, 0);
     upSizer->Add(executeButton, 0, wxALIGN_CENTER_VERTICAL, 0);
     upPanel->SetSizer(upSizer);
     vBox->Add(upPanel, 0, wxEXPAND, 0);
     textCtrlSizer->Add(inputBox, 1, wxEXPAND, 0);
     textCtrlSizer->Add(outputBox, 1, wxEXPAND, 0);
     downPanel->SetSizer(textCtrlSizer);
     vBox->Add(downPanel, 1, wxEXPAND, 0);
     SetSizer(vBox);
     Layout();
     // end wxGlade
}
void MyFrame::OnExecuteConv(wxCommandEvent& WXUNUSED(event)) {

     // 変換を実行する
     wxString inputStr = inputBox->GetValue();

     // nkfのインスタンスを準備する
     wxNKF* nkf = new wxNKF();
     const wxString option = combo_box_1->GetValue();
     const wxString inputString = inputBox->GetValue();
     std::string output = nkf->WxToMultiByte(inputString, option);
     wxMessageBox(wxString::Format("%d", output.size()));
     // コンボボックスに入っているオプションで条件分岐
     delete nkf;
}
