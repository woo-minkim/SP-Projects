# Lab 1: List / Hash / Bitmap 자료구조 실습

## 개요

Lab 1은 세 가지 자료구조(`list`, `hashtable`, `bitmap`)를 구현하고, 명령형 인터페이스로 연산을 검증하는 과제입니다.  
핵심은 "자료구조 구현" 자체와 "명령을 자료구조 API로 안전하게 연결하는 실행기"를 함께 만드는 것입니다.

## 구현 구조

### 1) 명령 인터페이스 계층 (`src/main.c`)

- `create`, `dumpdata`, `delete` 같은 공통 명령 처리
- `list_*`, `hash_*`, `bitmap_*` 명령군을 파싱해 해당 함수로 라우팅
- 표준 입력 기반 테스트 루프를 통해 자동 채점 스크립트/수동 테스트 모두 대응

### 2) 객체 관리 방식

- `my_list[10]`, `my_hash[10]`, `my_bitmap[10]` 배열로 객체 슬롯 관리
- `find_index_func()`로 이름의 숫자 suffix를 인덱스로 변환해 접근
- `*_in_use` 플래그로 객체 활성 상태 추적

### 3) 자료구조 구현 계층

- List: `push/pop/insert/remove/sort/splice/unique` 등 순차 구조 연산
- Hash: `insert/find/delete/replace/apply` 및 iterator 기반 출력
- Bitmap: `set/flip/reset/scan/count/contains` 등 비트 단위 연산

## 실행 방법

실행 위치: `Lab 1/src`

```bash
make
./testlib
```

입력 예시:

```text
create list list0
list_push_back list0 10
list_push_back list0 20
dumpdata list0
```

## 핵심 구현 포인트

- 명령 문자열을 함수 호출로 변환하는 얇은 dispatcher 구조를 유지해 구현 복잡도 분리
- 자료구조별 출력(`dumpdata`) 포맷을 통일해 디버깅/검증 속도 향상
- List/Hash/Bitmap 연산을 각각 별도 함수군으로 분리해 유지보수성 확보

## 한계 및 개선 여지

- 객체 이름에서 숫자 suffix를 전제로 하는 인덱싱은 확장성이 낮음 (`find_index_func` 의존)
- `delete` 시점 즉시 자원 회수 대신 상태 플래그 중심 처리라, 수명 관리 명확화 여지 존재
- 잘못된 입력 포맷에 대한 예외 메시지를 더 체계화하면 테스트 안정성이 올라감
