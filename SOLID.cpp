//SOLID 전부 적용 프로젝트
//커피 주문 시스템
//S : 각 클래스는 한가지 역할만 하기
//O : 새로운 커피 메뉴 추가 시 기존 코드 수정 없이 확장 가능
//L : Coffee 타입으로 Espresso나 Latte를 대체 가능
//I : Barista는 필요한 인터페이스(BrewCoffee)만 구현, 불필요한 기능 강요 없음
//D : OrderManager는 구체적인 PaymentProcessor 대신 추상화된 Payment 인터페이스에 의존.
// 커피주문 -> 결제 -> 제작 흐름으로 구현

#include "SOLID.h"

UI_Class::Main_UI* First_UI = nullptr;

//각종 자료구조 선언부 함수(동적변수로 진행)*시스템호출부분은 전역으로 진행
namespace Struct_Var {
	void Var() {
		First_UI = new UI_Class::Main_UI();
	}
}


//UI Class 영역
void UI_Class::Main_UI::First_Main() {
		int select = 0;
		while (1) {
			system("cls");
			printf("커피 주문 키오스크\n");
			printf("------------------------\n");
			printf("장바구니 내역\n");
			//장바구니 보여주는 함수
			printf("------------------------\n");
			printf("1. 커피선택, 2. 결제, 3. 종료\n");
			scanf_s("%d", &select);

			switch (select) {
			case 1:
				printf("커피 선택 영역\n");
				//함수
				break;
			case 2:
				printf("커피 결제 영역(영수증)\n");
				//함수
				break;
			case 3:
				printf("프로그램 종료\n");
				return;

			case -1:
				printf("개발자 모드\n");
				//개발자 모드를 만들어서 커피 추가 라던가 로그 보기를 만들꺼임
				//함수
				break;

			default:
				printf("버튼을 잘못누르셨습니다.\n");
				break;
			}
		}
}



//Control Class 영역
namespace Control_Class {
	
}


//Main
void Coffee_Main() {
	Struct_Var::Var();
	First_UI->First_Main();
	delete First_UI;
}