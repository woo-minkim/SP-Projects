# Lab 4: 동적 메모리 할당기 구현 (`mm.c`)

## 개요

Lab 4는 malloc lab 계열 과제로, `mm.c`에 사용자 수준 동적 메모리 할당기를 구현하는 프로젝트입니다.  
핵심은 블록 메타데이터 관리, free list 운영, 할당/해제/재할당 경로를 일관되게 유지하는 것입니다.

## 구현 구조

### 1) 블록/메타데이터 모델

- 헤더/푸터 기반 블록 관리 (`HDRP`, `FTRP`, `GET_SIZE`, `GET_ALLOC`)
- 가용 블록 내부에 free list 링크 저장 (`NEXT_FP`, `PREV_FP`)

### 2) free list 운영

- `add_free()`: free 블록을 리스트에 삽입
- `remove_free()`: 할당/병합 시 free 리스트에서 제거
- 기본 전략: explicit free list 기반 탐색/갱신

### 3) 할당/해제 흐름

- `mm_init()`
  - 초기 힙 구성 후 `extend_heap()`로 초기 가용 블록 확보
- `mm_malloc(size)`
  - `find_fit()`으로 적합 블록 탐색
  - 없으면 `extend_heap()` 호출
  - `place()`에서 분할 가능 시 split
- `mm_free(bp)`
  - 블록 free 표시 후 `coalesce()`로 인접 free 블록 병합
- `mm_realloc(oldptr, size)`
  - 축소면 기존 블록 유지
  - 확장 필요 시 인접 블록 확장 가능 여부 우선 확인 후, 불가하면 새 할당/복사

## 실행/검증 방법

이 Lab은 보통 과목 제공 테스트 하네스(`mdriver`)와 연동해 성능/정확도를 평가합니다.  
현재 저장소에는 구현 코드(`src/mm.c`)와 명세/문서가 정리되어 있습니다.

## 핵심 구현 포인트

- 병합(`coalesce`)과 분할(`place`) 경로를 분리해 단편화 완화
- free list 링크 조작(`NEXT_FP`/`PREV_FP`)을 함수화해 일관성 유지
- `realloc`에서 인접 free 블록 확장을 먼저 시도해 불필요한 복사를 줄임
