# SP-Projects

시스템프로그래밍 과목에서 수행한 Lab 프로젝트 아카이브입니다.  
각 Lab은 **연속 과제**가 아니라 **독립 과제**이며, 주제별로 다른 시스템 프로그래밍 문제를 다룹니다.

## 개요

- 과목: System Programming
- 구성: `Lab 1` ~ `Lab 4`
- 특징: 자료구조, 쉘, 네트워크 동시성, 메모리 할당기까지 주제별 실습 분리

## 읽는 순서 (추천)

1. 과제 전체 그림 파악: 이 README
2. 과제별 상세 확인:
   - [Lab 1 README](./Lab%201/README.md)
   - [Lab 2 README](./Lab%202/README.md)
   - [Lab 3 README](./Lab%203/README.md)
   - [Lab 4 README](./Lab%204/README.md)
3. 각 Lab의 `specification.pdf` / `document.pdf`로 요구사항 및 제출 결과 교차 확인

## Lab 비교 관점

| Lab | 핵심 문제 | 구현 초점 | 대표 파일 | 실행 위치 |
|---|---|---|---|---|
| Lab 1 | 추상 자료구조 연산 검증 | 명령 파싱 + List/Hash/Bitmap 연산 연결 | `Lab 1/src/main.c` | `Lab 1/src` |
| Lab 2 | 쉘 실행 모델 구현 | 명령 파싱, 파이프/백그라운드, job control, signal | `Lab 2/src/myshell.c` | `Lab 2/src` |
| Lab 3 | 동시성 서버 모델 비교 | `select` vs `pthread+semaphore` 구조 비교 | `Lab 3/task_1/stockserver.c`, `Lab 3/task_2/stockserver.c` | `Lab 3/task_1`, `Lab 3/task_2` |
| Lab 4 | 동적 메모리 할당기 | explicit free list, coalescing, realloc | `Lab 4/src/mm.c` | 테스트 하네스 연동 |

## 내가 수행한 내용 요약

- Lab 1: 명령형 테스트 드라이버와 자료구조 API를 통합해 연산 검증 루프 구성
- Lab 2: shell loop부터 pipeline/bg 처리, `jobs/bg/fg/kill` 및 `SIGCHLD` 기반 상태 갱신까지 구현
- Lab 3: 동일 도메인(주식 서버)을 이벤트 루프(`select`)와 스레드 기반(`pthread`)으로 각각 구현/비교
- Lab 4: `mm_init`/`mm_malloc`/`mm_free`/`mm_realloc` 흐름과 free list 관리 로직 구현

## 환경 참고

- 기본적으로 Linux + `gcc` + `make` 환경을 전제로 작성되었습니다.
- Lab 4는 단독 실행 프로그램보다 과목 제공 테스트 하네스(`mdriver` 등) 연동을 전제로 합니다.
