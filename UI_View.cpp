#include "Lib.h"

namespace UI_View {
	//클래스 함수의 기능들만 넣기
	
	void Total_UI::Home_Main() {
		//커피 사려면 상황
		//<사용자입장>
		//커피선택, 커피주문, 커피장바구니, 커피추가
		//<주인입장>
		//커피주문내역, 커피출고내역
		while (1) {
			system("cls");
			printf("<스타복스>\n");
			//커피목록
			printf("추가하기(+)\n");
			printf("주문하기(c or C)\n");
			printf("종료하기(q or Q)\n");

			scanf_s("%c", select);

			if (select == '+') {

			}
			else if (select == 'c' || select == 'C') {

			}
			else if (select == 'q' || select == 'Q') {
				printf("종료\n");
				break;
			}

			
			//커피 주문하기

		}
	}

	
	void Total_UI::Side_Home() {

	}

}