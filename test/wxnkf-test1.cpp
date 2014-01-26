#include <wx/wx.h>
#include <wx/wxnkf.h>
#include <memory>

class wxNKFTest1 : public wxAppConsole {

public:
     virtual bool OnInit();
     virtual int  OnRun();

private:
     int Test1();
     int Test2();
};

IMPLEMENT_APP(wxNKFTest1)

bool wxNKFTest1::OnInit() {
     return true;
}

int wxNKFTest1::OnRun() {

     Test1();
     //Test2();

     return 0;
}

int wxNKFTest1::Test1() {

     wxString test1 = wxT("aあいうえお");

     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     std::unique_ptr<wxNKF> nkf(new wxNKF());
     const std::string stdStr = nkf->WxToMultiByte(test1, option);

     std::cout << "変換対象：" << stdStr << std::endl;

     std::cout.setf(std::ios::hex, std::ios::basefield);
     std::cout.setf(std::ios::showbase);

     for (std::string::const_iterator it = stdStr.begin(); it != stdStr.end(); ++it) {
	  std::cout << (int)(*it & 0xff) << " ";
     }

     return 0;
}

int wxNKFTest1::Test2() {

     wxString test2 = wxT("testあいうえお");

     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     std::unique_ptr<wxNKF> nkf(new wxNKF());
     const std::string stdStr = nkf->WxToMultiByte(test2, option);
     std::cout << "変換対象：" << stdStr << std::endl;

     std::cout.setf(std::ios::hex, std::ios::basefield);
     std::cout.setf(std::ios::showbase);

     for (std::string::const_iterator it = stdStr.begin(); it != stdStr.end(); ++it) {
	  std::cout << (int)(*it & 0xff) << " ";
     }

     return 0;
}

