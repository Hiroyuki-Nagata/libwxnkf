#include <wx/wx.h>
#include <wx/wxnkf.h>
#include <memory>

/**
 * Test of wxNKF::ConvertSTDString
 */
class wxNKFTest2 : public wxAppConsole {

public:
     virtual bool OnInit();
     virtual int  OnRun();

private:
     int Test1();
     int Test2();
     void Dump(const std::string& stdStr);
     void ErrorReport1(size_t realSize, size_t convSize);
};

IMPLEMENT_APP(wxNKFTest2)

bool wxNKFTest2::OnInit() {
     return true;
}

int wxNKFTest2::OnRun() {

     Test1(); // <UTF-8>  ->  <CP932>
     //Test2(); // <UTF-8>  ->  <EUC-JP>

     return EXIT_SUCCESS;
}

void wxNKFTest2::Dump(const std::string& stdStr)
{
     for (std::string::const_iterator it = stdStr.begin(); it != stdStr.end(); ++it) 
     {
	  std::cout << (int)(*it & 0xff) << " ";
     }

     std::cout << std::endl;
}

void wxNKFTest2::ErrorReport1(size_t realSize, size_t convSize)
{
     printf("ERROR:   real string is %d byte.\n", realSize);
     printf("But converted string is %d byte.\n", convSize);
}

/**
 * std::string -> std::string
 * <UTF-8>     ->  <CP932>
 */
int wxNKFTest2::Test1()
{
     const std::string test1  = u8"AbCdEfGhあいうえお";
     const std::string option = "--ic=UTF-8 --oc=CP932";
     std::unique_ptr<wxNKF> nkf(new wxNKF());

     std::string stdStr;
     nkf->ConvertSTDString(test1, stdStr, option);

     std::cout << "変換対象：" << stdStr << std::endl;

     std::cout.setf(std::ios::hex, std::ios::basefield);
     std::cout.setf(std::ios::showbase);

     /**
      * test for UTF-8 to CP932
      */
     const unsigned char answer1[] 
	  = {0x41, 0x62, 0x43, 0x64, 0x45, 0x66, 0x47, 0x68, 0x82, 
	     0xa0, 0x82, 0xa2, 0x82, 0xa4, 0x82, 0xa6, 0x82, 0xa8 };

     if ( sizeof(answer1) != stdStr.size() )
     {
	  ErrorReport1(sizeof(answer1), stdStr.size());
	  return -2;
     }

     int index = 0;
     for (std::string::const_iterator it = stdStr.begin(); it != stdStr.end(); ++it, ++index) 
     {
	  if ( (int)(*it & 0xff) != answer1[index] )
	  {
	       std::cout << "ERROR: converted string index[" << index << "]" << std::endl;
	       Dump(stdStr);
	       return -2;
	  }
     }

     std::cout << "OK!" << std::endl;
     Dump(stdStr);

     return EXIT_SUCCESS;
}

/**
 * wxString -> std::string
 * <UTF-8>  ->  <EUC-JP>
 */
int wxNKFTest2::Test2()
{
     wxString test2 = wxT("AbCdEfGhあいうえお");

     const wxString option = wxT("--ic=UTF-8 --oc=EUC-JP");
     std::unique_ptr<wxNKF> nkf(new wxNKF());

     const std::string stdStr = nkf->WxToMultiByte(test2, option);

     std::cout << "変換対象：" << stdStr << std::endl;

     std::cout.setf(std::ios::hex, std::ios::basefield);
     std::cout.setf(std::ios::showbase);

     /**
      * test for UTF-8 to EUC-JP
      */
     const unsigned char answer2[] 
	  = {0x41, 0x62, 0x43, 0x64, 0x45, 0x66, 0x47, 0x68, 0xa4, 
	     0xa2, 0xa4, 0xa4, 0xa4, 0xa6, 0xa4, 0xa8, 0xa4, 0xaa};

     if ( sizeof(answer2) != stdStr.size() )
     {
	  std::cout << "ERROR: real string is " << sizeof(answer2) << " byte." << std::endl;
	  std::cout << "but converted string is " << stdStr.size() << " byte." << std::endl;
	  return -2;
     }

     int index = 0;
     for (std::string::const_iterator it = stdStr.begin(); it != stdStr.end(); ++it, ++index) 
     {
	  if ( (int)(*it & 0xff) != answer2[index] )
	  {
	       std::cout << "ERROR: converted string index[" << index << "]" << std::endl;
	       Dump(stdStr);
	       return -2;
	  }
     }

     std::cout << "OK!" << std::endl;
     Dump(stdStr);

     return EXIT_SUCCESS;
}
