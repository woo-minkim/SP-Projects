# Lab 1. Data Structure Command Interpreter

List, Hash Table, Bitmap 자료구조를 구현하고
명령형 인터페이스를 통해 각 연산을 실행·검증할 수 있도록 만든 C 기반 프로젝트입니다.

단순히 자료구조를 구현하는 데서 끝나지 않고  
입력 명령을 파싱해 적절한 자료구조 API로 연결하는 실행기를 함께 구성해 테스트 가능하도록 만들었습니다.

## Overview

이 프로젝트의 목표는 세 가지 자료구조를 직접 구현하고  
명령 기반 인터페이스를 통해 생성, 수정, 조회, 삭제 연산을 일관된 방식으로 검증하는 것이었습니다.

- `list`, `hashtable`, `bitmap` 자료구조 구현
- 명령 파싱 및 자료구조별 함수 라우팅
- 객체 생성 및 상태 관리
- 표준 입력 기반 테스트 루프 구성

## Architecture

### 1. Command Interface Layer (`src/main.c`)
- `create`, `dumpdata`, `delete`와 같은 공통 명령을 처리합니다.
- `list_*`, `hash_*`, `bitmap_*` 명령을 파싱해 해당 함수로 라우팅합니다.
- 표준 입력 기반 루프를 통해 자동 채점 스크립트와 수동 테스트 모두 가능하도록 구성했습니다.

### 2. Object Management
- `my_list[10]`, `my_hash[10]`, `my_bitmap[10]` 배열로 객체 슬롯을 관리했습니다.
- `find_index_func()`를 통해 객체 이름의 숫자 suffix를 인덱스로 변환해 접근했습니다.
- `*_in_use` 플래그를 두어 각 객체의 활성 상태를 추적했습니다.

### 3. Data Structure Layer
- **List**: `push`, `pop`, `insert`, `remove`, `sort`, `splice`, `unique`
- **Hash Table**: `insert`, `find`, `delete`, `replace`, `apply`, iterator 기반 출력
- **Bitmap**: `set`, `flip`, `reset`, `scan`, `count`, `contains`

## What I Built

- 명령 문자열을 파싱해 자료구조별 API 호출로 연결하는 dispatcher 구조를 구현했습니다.
- 자료구조 객체를 배열 기반 슬롯으로 관리해 생성, 삭제, 재사용 흐름을 제어했습니다.
- List, Hash Table, Bitmap 각각의 핵심 연산을 직접 구현하고, 명령 인터페이스와 연결했습니다.
- `dumpdata` 출력 포맷을 통일해 디버깅과 연산 결과 검증이 쉽도록 구성했습니다.
- 자동 채점과 수동 테스트가 모두 가능하도록 표준 입력 기반 실행 루프를 구현했습니다.

## Technical Focus

이 프로젝트에서 중점적으로 다룬 부분은 다음과 같습니다.

- **Command Parsing**: 입력 문자열을 해석해 적절한 연산으로 연결
- **API Dispatching**: 자료구조별 명령을 공통 인터페이스에서 분기 처리
- **State Management**: 객체 생성 여부와 활성 상태를 배열/플래그 기반으로 관리
- **C Data Structure Implementation**: List, Hash Table, Bitmap의 핵심 연산 직접 구현
