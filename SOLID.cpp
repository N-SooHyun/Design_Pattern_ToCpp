//SOLID ���� ���� ������Ʈ
//Ŀ�� �ֹ� �ý���
//S : �� Ŭ������ �Ѱ��� ���Ҹ� �ϱ�
//O : ���ο� Ŀ�� �޴� �߰� �� ���� �ڵ� ���� ���� Ȯ�� ����
//L : Coffee Ÿ������ Espresso�� Latte�� ��ü ����
//I : Barista�� �ʿ��� �������̽�(BrewCoffee)�� ����, ���ʿ��� ��� ���� ����
//D : OrderManager�� ��ü���� PaymentProcessor ��� �߻�ȭ�� Payment �������̽��� ����.
// Ŀ���ֹ� -> ���� -> ���� �帧���� ����

#include "SOLID.h"

UI_Class::Main_UI* First_UI = nullptr;

//���� �ڷᱸ�� ����� �Լ�(���������� ����)*�ý���ȣ��κ��� �������� ����
namespace Struct_Var {
	void Var() {
		First_UI = new UI_Class::Main_UI();
	}
}


//UI Class ����
void UI_Class::Main_UI::First_Main() {
		int select = 0;
		while (1) {
			system("cls");
			printf("Ŀ�� �ֹ� Ű����ũ\n");
			printf("------------------------\n");
			printf("��ٱ��� ����\n");
			//��ٱ��� �����ִ� �Լ�
			printf("------------------------\n");
			printf("1. Ŀ�Ǽ���, 2. ����, 3. ����\n");
			scanf_s("%d", &select);

			switch (select) {
			case 1:
				printf("Ŀ�� ���� ����\n");
				//�Լ�
				break;
			case 2:
				printf("Ŀ�� ���� ����(������)\n");
				//�Լ�
				break;
			case 3:
				printf("���α׷� ����\n");
				return;

			case -1:
				printf("������ ���\n");
				//������ ��带 ���� Ŀ�� �߰� ����� �α� ���⸦ ���鲨��
				//�Լ�
				break;

			default:
				printf("��ư�� �߸������̽��ϴ�.\n");
				break;
			}
		}
}



//Control Class ����
namespace Control_Class {
	
}


//Main
void Coffee_Main() {
	Struct_Var::Var();
	First_UI->First_Main();
	delete First_UI;
}