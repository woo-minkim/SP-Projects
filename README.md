# System Programming Projects

시스템 프로그래밍 과목에서 수행한 프로젝트 모음입니다.  
자료구조 추상화, Unix Shell 구현, 네트워크 동시성 서버, 동적 메모리 할당기까지 **시스템 소프트웨어의 핵심 주제를 직접 구현하며 다룬 프로젝트들**을 정리했습니다.

## Overview

이 저장소는 운영체제와 시스템 프로그래밍의 핵심 개념을 실제 코드로 구현한 4개의 Lab으로 구성되어 있습니다.

- 자료구조 연산과 명령 기반 테스트 드라이버 구현
- Unix Shell의 명령 실행, 파이프라인, job control 구현
- 이벤트 기반 / 스레드 기반 네트워크 서버 구현 및 비교
- explicit free list 기반 동적 메모리 할당기 구현

## Projects

### Lab 1. Data Structure Command Interpreter
명령어 입력을 기반으로 List, Hash, Bitmap 연산을 수행하는 테스트 드라이버를 구현한 프로젝트입니다.  
자료구조 API와 명령 파서를 연결해 생성, 수정, 조회, 삭제 연산을 검증할 수 있도록 구성했습니다.

[Lab 1 README](./Lab%201/README.md)

### Lab 2. Unix Shell Implementation
Unix Shell의 동작 원리를 직접 구현한 프로젝트입니다.  
명령 파싱, foreground/background 실행, 파이프라인 처리, job control, signal 기반 프로세스 상태 관리를 포함합니다.

[Lab 2 README](./Lab%202/README.md)

### Lab 3. Concurrent Stock Server
주식 거래 서버를 두 가지 동시성 모델로 구현한 프로젝트입니다.  
`select` 기반 이벤트 루프 방식과 `pthread + semaphore` 기반 멀티스레드 방식을 각각 구현해 구조적 차이를 비교했습니다.

[Lab 3 README](./Lab%203/README.md)

### Lab 4. Dynamic Memory Allocator
C 표준 라이브러리의 메모리 할당기와 유사한 동작을 목표로, explicit free list 기반 동적 메모리 할당기를 구현한 프로젝트입니다.  
블록 분할, 병합, 재할당 로직을 포함해 메모리 관리의 핵심 동작을 직접 구현했습니다.

[Lab 4 README](./Lab%204/README.md)

## Technical Focus

이 저장소에서는 다음과 같은 시스템 프로그래밍 주제를 다루었습니다.

- **Process Control**: fork, exec, wait, signal 처리
- **Shell Execution Model**: foreground/background job control, pipeline
- **Concurrency**: I/O multiplexing, multi-threading, semaphore synchronization
- **Memory Management**: dynamic allocation, explicit free list, coalescing, realloc
- **Low-level C Programming**: pointer manipulation, manual memory control, system-level debugging

## What I Built

- 자료구조 API를 명령 기반 인터페이스와 연결해 연산 검증 루프를 구성했습니다.
- Unix Shell에서 명령 실행 흐름, pipeline, background job 처리, signal 기반 상태 갱신을 구현했습니다.
- 동일한 서버 문제를 `select` 기반과 스레드 기반으로 각각 구현하며 동시성 모델 차이를 비교했습니다.
- 동적 메모리 할당기에서 `mm_malloc`, `mm_free`, `mm_realloc`과 free list 관리 로직을 구현했습니다.

## Repository Structure

```text
SP-Projects/
├─ Lab 1/
├─ Lab 2/
├─ Lab 3/
└─ Lab 4/