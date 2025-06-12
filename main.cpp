#include "Lib.h"
#include "Model.h"
#include "UI_View.h"
#include "Ctrl.h"

union opt_temperature {
	bool iced;
	bool hot;
	bool lukewarm;
};



void test_main() {
	Model_Interface::CRUD_Struct model_test;
	//model_test.Delete("..\\Access_File_Security_Git\\File\\test_1");

	Model_Interface::Logic_Ctrl model_ctrl(0);

	model_test.Ctrl_Box(&model_ctrl);
	
	model_test.SetData(const_cast<char*>("Hello Data"));
	model_test.SetName(const_cast<char*>("tests"));


	model_test.AppendData(const_cast<char*>("Bye Data"));
	


	model_test.Create(R"(C:\Users\NSH\Documents\Visual_studio_pro\Access_File_Security_Git\File\tes.txt)");

}

void main() {
	test_main();

	//��ü���⿡ ���� ���� ���ظ� ���ؼ�
	//���������� ���� Ưȭ ���α׷��� XML,DOM
	//������ ���Ͽ� ���ؼ� �����ϰ� ������� �ð� ��������	

	//File_Handle::File_Main test(path.p_d_str, ".xml");
	//File_Handle::File_Main test(path.p_d_str, ".xml");
	
	//test.Files_Print();
	//Xml_Parser_Main();

	//UI_View::Main_UI uMain;

	//uMain.home_main();

	
	//UI_View::Total_UI UI_Handler;


	//UI_Handler.Home_Main();

	//Json_Struct::Data_Json Json_Model;
	//Model_Interface::CRUD_Struct model_test;
	//model_test.Delete("..\\Access_File_Security_Git\\Xml\\RollMount_EtherCAT.xml");


}
