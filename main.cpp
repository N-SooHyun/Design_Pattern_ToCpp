#include "Lib.h"
#include "Model.h"
#include "UI_View.h"
#include "Ctrl.h"

union opt_temperature {
	bool iced;
	bool hot;
	bool lukewarm;
};

void main() {
	//객체지향에 대한 깊은 이해를 위해서
	//계층구조에 대한 특화 프로그램인 XML,DOM
	//디자인 패턴에 대해서 간략하게 배워보는 시간 가져보기	

	
	//Xml_Parser_Main();

	//User_Interface::UI_Main();

	//Coffee_Main();
	
	//nDynamic::DynamicStr path(1024);
	
	//path.OperStr("..\\Access_File_Security_Git\\Xml");

	//printf("%s\n", path.p_d_str);

	//File_Handle::File_Main test(path.p_d_str, ".xml");
	//File_Handle::File_Main test(path.p_d_str, ".xml");
	
	//test.Files_Print();
	//Xml_Parser_Main();

	//UI_View::Main_UI uMain;

	//uMain.home_main();

	
	//UI_View::Total_UI UI_Handler;


	//UI_Handler.Home_Main();

	Json_Struct::Data_Json Json_Model;
}
