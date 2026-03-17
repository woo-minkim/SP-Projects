# Lab 4: 동적 메모리 할당기 구현 (`mm.c`)

Lab 4는 malloc lab 스타일의 동적 메모리 할당기 구현 과제입니다.  
핵심 구현 파일은 `src/mm.c`이며, 명시적 free list 기반 할당기를 작성했습니다.

## 핵심 구현 내용

- 힙 초기화: `mm_init`
- 할당: `mm_malloc`
- 해제: `mm_free`
- 재할당: `mm_realloc`
- 가용 블록 탐색: `find_fit`
- 블록 배치/분할: `place`
- 인접 free 블록 병합: `coalesce`
- free list 삽입/제거: `add_free`, `remove_free`

## 주요 파일

- `src/mm.c`: 동적 할당기 구현 코드
- `specification.pdf`: 과제 명세
- `document.pdf`: 제출 문서

## 실행/테스트 참고

이 Lab은 단독 실행 프로그램이 아니라, 과목 제공 테스트 하네스(`mdriver` 등)와 연동해 평가하는 형태입니다.  
현재 저장소에는 구현 파일(`mm.c`) 중심으로 정리되어 있습니다.

## 내가 구현한 포인트

- explicit free list를 사용해 가용 블록을 관리
- `malloc/free` 반복 상황에서 블록 분할/병합으로 단편화 완화
- `realloc` 시 인접 블록 확장 가능 여부를 우선 확인하고, 필요 시 새 블록 할당/복사 처리

