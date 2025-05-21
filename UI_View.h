#pragma once
#ifndef UI_VIEW_H
#define UI_VIEW_H

//메인화면
namespace UI_View {
	//Interface
	class Main_UI {
	public:
		Main_UI(){}
		~Main_UI() {}

		//메인 커피메뉴 영역
		virtual void Home_Main() = 0;

	};

	//Interface
	class Side_UI{
	public:
		Side_UI() {}
		~Side_UI() {}

		//장바구니 영역
		virtual void Cart_Main() = 0;
		virtual void Menu_Main() = 0;
	};


	class Total_UI : public Main_UI, public Side_UI{
	public:
		void Home_Main();

		void Cart_Main();

		void Menu_Main();

	private:
		char select;
	};
}


#endif