#include "Str.h"
#include <cstring>
#include <cstdlib>

Str::Str(int length) { // string 길이 입력받아 그 길이의 문자열 생성
    len = length; // 전달받은 문자열의 길이 저장
    str = new char[len + 1];
    str[0] = '\0';
}

Str::Str(char* contents) { // string 내용 초기화
    len = strlen(contents); // 전달받은 문자열의 길이 저장

    str = new char[len + 1]; //새 문자열 생성, 초기화
    strcpy(str, contents);
    str[len] = '\0';
}

Str::~Str() { // 소멸자
    delete []str;
}

int Str::length() { // string의 길이 반환
    return len;
}

char *Str::contents() { // string의 내용 반환
    return str;
}

int Str::compare(char *a) { // strcmp 함수를 이용해 두 문자열 비교
    return strcmp(str, a);
}

int Str::compare(Str &a) {
    return (*this).compare(a);
}

void Str::operator=(char *a) { // char *a의 내용 복사
    if (a == 0) { // NULL_ptr = 0
        len = 0;
        delete []str;
		str = new char[1];
        str[0] = '\0';
        return;
    }
    int length_a = strlen(a);

    if (len < length_a) { // a의 길이보다 len이 작은 경우
		delete []str;
		str = new char[length_a + 1];
	}

    len = length_a;

    strcpy(str, a); // a의 내용을 strcpy를 이용해서 복사
    str[len] = '\0';
}

void Str::operator=(Str &a) {
    (*this) = a.contents();
}
