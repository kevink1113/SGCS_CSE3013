#include "Header.h"

/* 테스트 케이스의 수와 페이지 수를 입력받고
계산 함수, 출력 함수를 실행하는 main 함수 */
int main(){
int N, T;
	scanf("%d", &T); //테스트 케이스 입력

	for(int i=0; i<T; i++){ //테스트 케이스의 수만큼 반복
		int arr[10]={0, }; //각 숫자들의 개수를 저장하는 배열 초기화
		scanf("%d", &N); //N 입력받기
		calculate_page(N, arr); //계산 함수 실행
        print_result(arr); //출력 함수 실행
	}

	return 0; //종료
}
