#pragma once
#ifndef UI_VIEW_H
#define UI_VIEW_H

//����ȭ��
namespace UI_View {
	//Interface
	class Main_UI {
	public:
		Main_UI() : select(0){}
		~Main_UI() {}

		//���� Ŀ�Ǹ޴� ����
		virtual void Home_Main() = 0;




	protected:
		char select;
	};

	//Interface
	class Side_UI{
	public:
		Side_UI() {}
		~Side_UI() {}

		//��ٱ��� ����
		virtual void Side_Home() = 0;
	};


	class Total_UI : public Main_UI, public Side_UI{
	public:
		void Home_Main();

		void Side_Home();
	};
}


#endif