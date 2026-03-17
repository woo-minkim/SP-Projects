# SP-Projects

시스템프로그래밍 과목에서 수행한 Lab 프로젝트 모음 저장소입니다.  

## 저장소 성격

- 과목: System Programming
- 구성: Lab 1 ~ Lab 4
- 범위: 코드, 명세서, 제출 문서 정리

## 빠른 탐색 가이드

1. 전체 개요 확인: 이 문서
2. 관심 주제 선택: [Lab 1](./Lab%201/README.md), [Lab 2](./Lab%202/README.md), [Lab 3](./Lab%203/README.md), [Lab 4](./Lab%204/README.md)
3. 각 Lab README에서 실행 위치/핵심 파일/기능 확인

## Lab 비교

| Lab | 주제 | 핵심 기술 | 주요 파일 | 실행 위치 |
|---|---|---|---|---|
| Lab 1 | 자료구조 라이브러리 + 명령형 테스트 | Doubly Linked List, Hash Table, Bitmap | `Lab 1/src/main.c`, `Lab 1/src/Makefile` | `Lab 1/src` |
| Lab 2 | 사용자 쉘 구현 | Process, Pipe, Signal, Job Control | `Lab 2/src/myshell.c` | `Lab 2/src` |
| Lab 3 | 네트워크 주식 서버 | Socket I/O, 동시성(Select/Thread), 동기화 | `Lab 3/task_1/stockserver.c`, `Lab 3/task_2/stockserver.c` | `Lab 3/task_1`, `Lab 3/task_2` |
| Lab 4 | 동적 메모리 할당기 | Explicit Free List, Coalescing, Realloc | `Lab 4/src/mm.c` | (테스트 하네스 연동형) |

## 핵심 구현 내용 요약

- Lab 1: list/hash/bitmap 자료구조 기능 구현 및 명령 기반 테스트 인터페이스 구성
- Lab 2: 파이프, 시그널 처리, bg/fg/jobs/kill을 포함한 `myshell` 구현
- Lab 3: 동일 도메인(주식 서버)을 I/O multiplexing 버전과 thread+semaphore 버전으로 각각 구현
- Lab 4: 힙 확장, 블록 분할/병합, free list 관리, `malloc/free/realloc` 동작 구현

## 문서 링크

- [Lab 1 README](./Lab%201/README.md)
- [Lab 2 README](./Lab%202/README.md)
- [Lab 3 README](./Lab%203/README.md)
- [Lab 4 README](./Lab%204/README.md)

