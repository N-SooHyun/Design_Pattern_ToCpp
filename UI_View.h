#pragma once
#ifndef UI_VIEW_H
#define UI_VIEW_H

//����ȭ��
namespace UI_View {
	class Main_UI {
	public:
		Main_UI() : select(0){}
		~Main_UI() {}

		void home_main();


	private:
		int select;
	};
}


#endif