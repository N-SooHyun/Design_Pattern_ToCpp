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
			Menu_Main();
			//장바구니 목록
			Cart_Main();
			printf("장바구니에 있는 커피 추가하기(+)\n");
			printf("장바구니에 있는 커피 제거하기(-)\n");
			printf("주문하기(c or C)\n");
			printf("종료하기(q or Q)\n\n");//혹은 초기화

			printf("선택하세요 : ");

			scanf_s("%c", &select, 1);
			//입력 버퍼 제거
			while (getchar() != '\n') {}
			switch (select) {
			case '+':
				printf("메뉴 추가 함수 삽입\n");
				break;
			case '-':
				printf("메뉴 제거 함수 삽입\n");
				break;

			case 'r':
			case 'R':
				printf("페이지 넘기기 함수 삽입\n");
				break;

			case 'l':
			case 'L':
				printf("페이지 넘기기 함수 삽입\n");
				break;
			
			case 'c':
			case 'C':
				printf("주문하기 함수 삽입\n");
				break;

			case 'q':
			case 'Q':
				printf("프로그램 종료\n");
				return;

			default:
				printf("보기에 없는 문자입니다. 다시 입력하세요\n");
				break;
			}
			
			//커피 주문하기 

		}
	}

	void Total_UI::Menu_Main() {
		printf("-------------------커피목록-----------------------\n");
		printf("1. 아메리카노\t");
		printf("2. 커피\t");
		printf("3. 카페라떼\n");
		printf("4. 녹차라떼\t");
		printf("5. 오곡라떼\t");
		printf("6. 바닐라라떼\n");

		printf("Left(L or l)\b\bRight(R or r)\n");
		printf("--------------------------------------------------\n");
	}
	
	void Total_UI::Cart_Main() {
		printf("-------------------주문정보-----------------------\n");
		printf("~~커피(1 샷, 아이스)\n");
		printf("--------------------------------------------------\n");
	}

}