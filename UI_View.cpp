#include "Lib.h"

namespace UI_View {
	//Ŭ���� �Լ��� ��ɵ鸸 �ֱ�
	
	void Total_UI::Home_Main() {
		//Ŀ�� ����� ��Ȳ
		//<���������>
		//Ŀ�Ǽ���, Ŀ���ֹ�, Ŀ����ٱ���, Ŀ���߰�
		//<��������>
		//Ŀ���ֹ�����, Ŀ�������
		while (1) {
			system("cls");
			printf("<��Ÿ����>\n");
			//Ŀ�Ǹ��
			Menu_Main();
			//��ٱ��� ���
			Cart_Main();
			printf("��ٱ��Ͽ� �ִ� Ŀ�� �߰��ϱ�(+)\n");
			printf("��ٱ��Ͽ� �ִ� Ŀ�� �����ϱ�(-)\n");
			printf("�ֹ��ϱ�(c or C)\n");
			printf("�����ϱ�(q or Q)\n\n");//Ȥ�� �ʱ�ȭ

			printf("�����ϼ��� : ");

			scanf_s("%c", &select, 1);
			//�Է� ���� ����
			while (getchar() != '\n') {}
			switch (select) {
			case '+':
				printf("�޴� �߰� �Լ� ����\n");
				break;
			case '-':
				printf("�޴� ���� �Լ� ����\n");
				break;

			case 'r':
			case 'R':
				printf("������ �ѱ�� �Լ� ����\n");
				break;

			case 'l':
			case 'L':
				printf("������ �ѱ�� �Լ� ����\n");
				break;
			
			case 'c':
			case 'C':
				printf("�ֹ��ϱ� �Լ� ����\n");
				break;

			case 'q':
			case 'Q':
				printf("���α׷� ����\n");
				return;

			default:
				printf("���⿡ ���� �����Դϴ�. �ٽ� �Է��ϼ���\n");
				break;
			}
			
			//Ŀ�� �ֹ��ϱ� 

		}
	}

	void Total_UI::Menu_Main() {
		printf("-------------------Ŀ�Ǹ��-----------------------\n");
		printf("1. �Ƹ޸�ī��\t");
		printf("2. Ŀ��\t");
		printf("3. ī���\n");
		printf("4. ������\t");
		printf("5. �����\t");
		printf("6. �ٴҶ��\n");

		printf("Left(L or l)\b\bRight(R or r)\n");
		printf("--------------------------------------------------\n");
	}
	
	void Total_UI::Cart_Main() {
		printf("-------------------�ֹ�����-----------------------\n");
		printf("~~Ŀ��(1 ��, ���̽�)\n");
		printf("--------------------------------------------------\n");
	}

}