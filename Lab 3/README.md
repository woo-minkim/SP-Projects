# Lab 3: 주식 서버 동시성 구현 (Task 1 / Task 2)

Lab 3는 소켓 기반 주식 관리 서버를 구현한 프로젝트입니다.  
동일한 기능 요구를 두 가지 동시성 모델로 각각 구현했습니다.

## 공통 기능

- `stock.txt`를 읽어 서버 시작 시 재고 트리 구성
- 클라이언트 명령 처리
- `show`: 전체 재고 조회
- `buy <id> <qty>`: 재고 차감
- `sell <id> <qty>`: 재고 증가
- 처리 결과를 클라이언트로 반환하고 상태를 파일에 반영

## Task 1 vs Task 2

- `task_1`: `select` 기반 I/O multiplexing 서버
- `task_2`: `pthread` 기반 thread-per-connection 서버 + semaphore 동기화

동일 도메인 로직을 이벤트 루프 방식과 스레드 방식으로 비교 실습한 것이 핵심입니다.

## 주요 파일

- `task_1/stockserver.c`: select 기반 서버
- `task_2/stockserver.c`: thread + semaphore 기반 서버
- `task_1/stockclient.c`, `task_2/stockclient.c`: 대화형 클라이언트
- `task_1/multiclient.c`, `task_2/multiclient.c`: 다중 클라이언트 부하 테스트
- `task_1/Makefile`, `task_2/Makefile`: 빌드 스크립트
- `document.pdf`: 제출 문서

## 빌드 및 실행

각 task 디렉터리에서 동일하게 실행합니다.

```bash
make
./stockserver <port>
```

다른 터미널에서:

```bash
./stockclient <host> <port>
```

부하 테스트:

```bash
./multiclient <host> <port> <client_count>
```

## 내가 구현한 포인트

- 재고 데이터를 BST 형태로 관리하고 명령별 탐색/갱신 루틴 구성
- Task 1에서 연결 풀/FD 집합 기반 비동기 처리 구조 구현
- Task 2에서 동시 접근 충돌을 줄이기 위한 스레드 및 세마포어 보호 구간 구성

