#include "Header.h"

void calculate_page(int N, int* arr){
	int i, j, prv=0;
	for (i = 1; N; i *= 10) { 
		/* i는 1부터 시작해서 10씩 곱해짐,
		  N이 0이 아닐때까지 반복 */
		int sub = N % 10; N /= 10;
		/* N을 10으로 나눠 나머지 (각 자리의 수) 얻기,
		  N을 10으로 나눠 다음 자리에 대한 연산 준비 */
		for (j = 0; j < sub; ++j) 
			arr[j] += (N + 1) * i;
		/* 각 자리의 수 전까지의 숫자는 그 윗자리 수부터의 숫자 곱하기
		   i 정도만큼 있음. */
		arr[sub] += N * i + prv + 1;
		// '나머지 숫자'의 개수도 마찬가지...+ 그 밑 자리 수에서의 개수 합
		for (j = sub + 1; j < 10; ++j) 
			arr[j] += N * i;
			
		arr[0] -= i; // 첫번째 자리의 중복 빼기
		prv += sub * i; // 밑 자리 수에서의 개수 합 저장.
	}
}
