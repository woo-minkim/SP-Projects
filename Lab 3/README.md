# Lab 3: 주식 서버 동시성 구현 비교 (`task_1` vs `task_2`)

## 개요

Lab 3는 주식 재고 서버를 구현하고, 동일 기능을 서로 다른 동시성 모델로 비교하는 과제입니다.  
핵심 질문은 "같은 기능을 이벤트 루프 기반과 스레드 기반으로 구현했을 때 구조와 동기화가 어떻게 달라지는가"입니다.

## 구현 구조

### 공통 도메인 로직

- `stock.txt`를 읽어 BST로 재고 데이터 구성
- 공통 명령
  - `show`: 전체 종목 조회
  - `buy <id> <qty>`: 재고 차감
  - `sell <id> <qty>`: 재고 증가
- 명령 처리 후 응답 반환 + 파일 저장

### Task 1: `select` 기반 I/O multiplexing

- 연결 풀(`pool`)에 클라이언트 FD/RIO 상태 저장
- `FD_SET` + `Select`로 readable FD를 순회 처리
- 단일 이벤트 루프에서 다중 클라이언트를 관리
- 관련 코드 포인트: `init_pool()`, `add_client()`, `handle_clients()`

### Task 2: `pthread` + `semaphore` 기반

- 클라이언트 연결마다 `Pthread_create()`로 worker thread 생성
- `show` 읽기 처리와 상태 갱신 구간에 세마포어 적용
- 종목 단위 mutex 및 전역 동기화(`mutex`, `state_mutex`) 사용
- 관련 코드 포인트: `thread()`, `Sem_init()`, `P()/V()`, `process_stock()`

## 실행 방법

`task_1` 또는 `task_2` 디렉터리에서 실행합니다.

```bash
make
./stockserver <port>
```

다른 터미널:

```bash
./stockclient <host> <port>
```

부하 테스트:

```bash
./multiclient <host> <port> <client_count>
```

## 핵심 구현 포인트

- BST를 사용해 재고 조회/갱신의 기본 탐색 구조를 단순화
- 동일 명령 집합을 유지한 채 동시성 모델만 바꿔 비교 가능하도록 설계
- `task_1`은 단일 루프 제어 단순성, `task_2`는 동시 처리량과 동기화 복잡도 학습에 초점
