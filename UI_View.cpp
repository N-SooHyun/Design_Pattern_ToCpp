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
			printf("�߰��ϱ�(+)\n");
			printf("�ֹ��ϱ�(c or C)\n");
			printf("�����ϱ�(q or Q)\n");

			scanf_s("%c", select);

			if (select == '+') {

			}
			else if (select == 'c' || select == 'C') {

			}
			else if (select == 'q' || select == 'Q') {
				printf("����\n");
				break;
			}

			
			//Ŀ�� �ֹ��ϱ�

		}
	}

	
	void Total_UI::Side_Home() {

	}

}