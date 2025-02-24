//SOLID 전부 적용 프로젝트
//커피 주문 시스템
//S : 각 클래스는 한가지 역할만 하기
//O : 새로운 커피 메뉴 추가 시 기존 코드 수정 없이 확장 가능
//L : Coffee 타입으로 Espresso나 Latte를 대체 가능
//I : Barista는 필요한 인터페이스(BrewCoffee)만 구현, 불필요한 기능 강요 없음
//D : OrderManager는 구체적인 PaymentProcessor 대신 추상화된 Payment 인터페이스에 의존.
// 커피주문 -> 결제 -> 제작 흐름으로 구현

