# Lab 1: 자료구조 구현 (List / Hash / Bitmap)

Lab 1은 시스템프로그래밍 과목의 자료구조 실습으로, `list`, `hashtable`, `bitmap`을 직접 구현하고 명령형 인터페이스로 동작을 검증하는 프로젝트입니다.

## 핵심 내용

- Doubly Linked List 기반 연산 구현
- Hash Table 구현(체이닝 방식, 리스트 활용)
- Bitmap 생성/조회/수정/카운트 연산 구현
- 표준 입력 명령을 읽어 자료구조를 생성/수정/출력하는 테스트 드라이버 구현

## 주요 파일

- `src/main.c`: 명령 파싱 및 자료구조 연산 호출
- `src/list.c`, `src/hash.c`, `src/bitmap.c`: 자료구조 로직
- `src/Makefile`: 빌드 스크립트
- `specification.pdf`: 과제 명세

## 빌드 및 실행

실행 위치는 `Lab 1/src`입니다.

```bash
make
./testlib
```

프로그램은 stdin에서 명령을 읽습니다.  
예: `create list list0`, `dumpdata list0`, `list_push_back list0 10`

## 내가 구현한 포인트

- 자료구조별 핵심 API 동작을 과제 명령 집합에 맞게 연결
- 여러 객체(`list0`, `hash0`, `bm0` 등)를 인덱스로 관리하는 실행 구조 구현
- `dumpdata`/연산 명령을 통해 실습 중간 상태를 빠르게 검증할 수 있게 구성

