#include "LinkedList.h"

//1. 템플릿 클래스로 확장해야함
//2. Stack형식으로 Delete 함수 재정의해야함
//주의: first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용하여 가져와야함

//LinkedList class를 상속받음
template <typename T>
class Stack:public LinkedList<T> {
public:
    bool Delete(T &element); // 맨 뒤의 원소를 삭제
};

template <typename T>
bool Stack<T>::Delete(T &element) {
    if(this->first == NULL) return false;
    element = this->first->data;
    Node<T> *t = this->first;
    this->first = this->first->link;
    delete t;
    this->current_size--;
    return true;
}
