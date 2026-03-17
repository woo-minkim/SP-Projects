# Lab 4. Dynamic Memory Allocator

사용자 수준 동적 메모리 할당기를 직접 구현한 프로젝트입니다.  
`mm.c`에서 `malloc`, `free`, `realloc`에 대응하는 핵심 동작을 구현하며, **블록 메타데이터 관리, free list 운영, 블록 분할과 병합, 재할당 최적화**를 다뤘습니다.

## Overview

이 프로젝트의 목표는 heap 상에서 메모리 블록을 직접 관리하며,  
가용 블록 탐색, 할당, 해제, 병합, 재할당까지의 흐름을 일관된 allocator 구조로 구현하는 것이었습니다.

- 헤더 / 푸터 기반 블록 관리
- explicit free list 운영
- `malloc`, `free`, `realloc` 구현
- 블록 분할과 병합을 통한 단편화 완화
- 불필요한 메모리 복사를 줄이는 재할당 전략 적용

## Block Model

### Header / Footer-based Block Layout
- 헤더와 푸터를 사용해 블록 크기와 할당 여부를 관리했습니다.
- `HDRP`, `FTRP`, `GET_SIZE`, `GET_ALLOC` 매크로를 통해 블록 메타데이터를 처리했습니다.

### Free Block Metadata
- 가용 블록 내부에 free list 연결 정보를 저장했습니다.
- `NEXT_FP`, `PREV_FP`를 사용해 explicit free list를 구성했습니다.

## Allocation Flow

### `mm_init()`
- 초기 힙 구조를 설정하고 `extend_heap()`을 호출해 첫 가용 블록을 확보했습니다.

### `mm_malloc(size)`
- `find_fit()`으로 요청 크기에 맞는 가용 블록을 탐색했습니다.
- 적합한 블록이 없으면 `extend_heap()`으로 힙을 확장했습니다.
- `place()`에서 블록을 배치하고, 남는 공간이 충분하면 split했습니다.

### `mm_free(bp)`
- 블록을 free 상태로 전환한 뒤 `coalesce()`를 호출해 인접 가용 블록과 병합했습니다.

### `mm_realloc(oldptr, size)`
- 축소 요청이면 기존 블록을 유지했습니다.
- 확장이 필요한 경우 인접 가용 블록을 활용할 수 있는지 먼저 확인했습니다.
- 확장이 불가능하면 새 블록을 할당하고 데이터를 복사했습니다.

## Free List Management

- `add_free()` : 가용 블록을 free list에 삽입
- `remove_free()` : 할당 또는 병합 시 free list에서 제거

explicit free list 기반으로 가용 블록을 관리해,  
전체 힙을 선형 탐색하는 방식보다 탐색 및 갱신 구조를 더 명확하게 유지하도록 구현했습니다.

## What I Built

- 헤더 / 푸터 기반 블록 메타데이터 구조를 설계하고 관리 로직을 구현했습니다.
- explicit free list를 구성해 가용 블록의 삽입, 제거, 탐색 흐름을 구현했습니다.
- `mm_malloc`, `mm_free`, `mm_realloc`, `mm_init`의 핵심 경로를 직접 구현했습니다.
- `place()`와 `coalesce()`를 분리해 블록 분할과 병합이 일관되게 동작하도록 구성했습니다.
- `realloc`에서 인접 가용 블록 확장을 우선 시도해 불필요한 새 할당 및 메모리 복사를 줄이도록 구현했습니다.

## Technical Focus

이 프로젝트에서 중점적으로 다룬 부분은 다음과 같습니다.

- **Heap Block Management**: 블록 크기와 할당 상태를 메타데이터로 관리
- **Explicit Free List**: 가용 블록을 연결 리스트로 유지하며 탐색 및 갱신
- **Fragmentation Control**: split과 coalescing을 통한 단편화 완화
- **Reallocation Strategy**: 인접 블록 확장을 우선 고려하는 `realloc` 최적화
- **Low-level Memory Handling**: 포인터 연산과 매크로 기반 메모리 제어

## Validation

이 프로젝트는 일반적으로 과목 제공 테스트 하네스(`mdriver`)와 연동해  
정확성(correctness)과 성능(performance)을 검증하는 형태로 수행됩니다.

현재 저장소에는 다음이 포함되어 있습니다.

- `src/mm.c` — allocator 구현 코드
- 명세 및 관련 문서

## Implementation Highlights

- `coalesce()`와 `place()` 경로를 분리해 병합과 분할 로직의 역할을 명확히 했습니다.
- free list 링크 조작을 함수화해 `NEXT_FP` / `PREV_FP` 업데이트의 일관성을 유지했습니다.
- `realloc`에서 인접 free 블록을 먼저 활용하도록 설계해 불필요한 데이터 복사를 줄였습니다.