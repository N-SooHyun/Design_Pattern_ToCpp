#pragma once
#ifndef UI_VIEW_H
#define UI_VIEW_H

//메인화면
namespace UI_View {
	//Interface
	class Main_UI {
	public:
		Main_UI() : select(0){}
		~Main_UI() {}

		//메인 커피메뉴 영역
		virtual void Home_Main() = 0;




	protected:
		char select;
	};

	//Interface
	class Side_UI{
	public:
		Side_UI() {}
		~Side_UI() {}

		//장바구니 영역
		virtual void Side_Home() = 0;
	};


	class Total_UI : public Main_UI, public Side_UI{
	public:
		void Home_Main();

		void Side_Home();
	};
}


#endif