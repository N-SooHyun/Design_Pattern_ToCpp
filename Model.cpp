#include "Lib.h"

namespace Tree_Struct {
	//Coffee_Menu_Node Ŭ���� �Լ�����






	//Coffee_Tree Ŭ���� �Լ�����
	//����
	void Coffee_Tree::Insert() {

	}

	//����
	void Coffee_Tree::Delete() {

	}

	//�˻�
	void Coffee_Tree::Select() {

	}



	void Coffee_Tree::start() {
		const char* name_1 = "�Ƹ޸�ī��";
		const char* name_2 = "ī�� ��";
		const char* name_3 = "�ٴҶ� ��";
		const char* name_4 = "���� ��";
		const char* name_5 = "���� ��";


		
		while (1) {
			if (Root_Node == nullptr) {
				Root_Node = new Coffee_Menu_Node();
				Root_Node->coffee_name->OperStr(name_1);
			}


		}

	}
}