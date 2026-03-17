# Lab 3. Concurrent Stock Server

동일한 주식 재고 서버를 두 가지 동시성 모델로 구현하고 비교한 프로젝트입니다.  
`select` 기반 I/O multiplexing 방식과 `pthread + semaphore` 기반 멀티스레드 방식을 각각 구현하며 **같은 서버 기능이 동시성 구조에 따라 어떻게 달라지는지**를 확인했습니다.

## Overview

이 프로젝트의 목표는 주식 재고 조회 및 갱신 서버를 구현하고  
이벤트 루프 기반 서버와 스레드 기반 서버를 각각 구성해 구조적 차이와 동기화 방식의 차이를 비교하는 것이었습니다.

- 주식 재고 데이터를 메모리 상에서 관리하는 서버 구현
- `show`, `buy`, `sell` 명령 처리
- `select` 기반 이벤트 루프 서버 구현
- `pthread + semaphore` 기반 멀티스레드 서버 구현
- 동일 기능을 두 가지 동시성 모델에서 비교

## Common Domain Logic

두 구현 모두 동일한 도메인 로직을 공유합니다.

- `stock.txt`를 읽어 BST 기반 재고 데이터 구조를 구성
- `show`: 전체 종목 조회
- `buy <id> <qty>`: 재고 차감
- `sell <id> <qty>`: 재고 증가
- 명령 처리 후 클라이언트에 결과 반환
- 서버 종료 또는 상태 변경 시 파일에 저장

## Concurrency Models

### Task 1. `select`-based I/O Multiplexing
단일 이벤트 루프에서 여러 클라이언트를 처리하는 방식입니다.

- 연결 풀(`pool`)에 클라이언트 FD와 RIO 상태 저장
- `FD_SET`과 `select()`를 사용해 readable FD를 순회 처리
- 하나의 루프에서 다중 클라이언트 요청을 관리
- 상대적으로 구조가 단순하고 제어 흐름이 명확한 방식

**Key Functions**
- `init_pool()`
- `add_client()`
- `handle_clients()`

### Task 2. `pthread + semaphore`-based Multi-threading
클라이언트 연결마다 worker thread를 생성하는 방식입니다.

- 연결마다 `Pthread_create()`로 개별 worker thread 생성
- 공유 데이터 접근 구간에 semaphore 및 mutex 적용
- `show`와 상태 변경 구간에서 동기화 처리
- 병렬 처리량은 높지만 동기화 복잡도가 증가하는 방식

**Key Functions**
- `thread()`
- `Sem_init()`
- `P() / V()`
- `process_stock()`

## What I Built

- `stock.txt`를 읽어 BST 기반 재고 데이터 구조를 구성하고 조회 및 갱신 명령을 처리하는 서버를 구현했습니다.
- `show`, `buy`, `sell` 명령에 대해 공통 비즈니스 로직을 유지하면서 동시성 구조만 다르게 적용했습니다.
- `select()` 기반 서버에서는 연결 풀과 이벤트 루프를 구성해 단일 프로세스에서 다중 클라이언트를 처리했습니다.
- `pthread` 기반 서버에서는 클라이언트마다 worker thread를 생성하고 semaphore / mutex를 사용해 공유 자원 접근을 동기화했습니다.
- `stockclient`, `multiclient`를 통해 단일 요청과 다중 클라이언트 부하 상황을 테스트할 수 있도록 구성했습니다.

## Technical Focus

이 프로젝트에서 중점적으로 다룬 부분은 다음과 같습니다.

- **Network Server Design**: 클라이언트 요청을 처리하는 TCP 서버 구현
- **Concurrency Models**: 이벤트 기반 I/O multiplexing과 멀티스레드 구조 비교
- **Synchronization**: semaphore와 mutex를 이용한 공유 데이터 보호
- **Data Structure Management**: BST 기반 재고 데이터 관리
- **Scalability Trade-off**: 단순한 제어 흐름과 병렬 처리량 사이의 구조적 차이 이해
