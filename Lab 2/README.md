# Lab 2. Unix Shell Implementation

Unix Shell의 핵심 실행 모델을 직접 구현한 프로젝트입니다.  
기본 명령 실행부터 파이프라인, background 실행, job control, signal 처리까지 확장하며 **interactive shell의 주요 동작을 단계적으로 구현**했습니다.

## Overview

이 프로젝트의 목표는 
실제 shell처럼 foreground/background 프로세스를 관리하고 파이프라인과 signal 기반 상태 제어를 지원하는 실행 환경을 만드는 것이었습니다.

- 명령 입력 및 파싱
- `fork` / `execvp` 기반 외부 명령 실행
- 파이프라인 처리
- background job 실행
- job control 및 signal 기반 상태 관리

## Development Stages

### Phase 1. Basic Shell Loop
기본적인 shell 실행 흐름을 구현한 단계입니다.

- 프롬프트 출력 및 입력 루프 구성
- `parse_line()` 기반 인자 분해
- `fork()` + `execvp()`를 통한 외부 명령 실행
- `cd`, `exit` 등 최소 built-in 명령 처리

이 단계에서는 shell의 기본 구조인 **입력 → 파싱 → 실행 → 복귀** 흐름을 안정적으로 만드는 데 집중했습니다.

### Phase 2. Pipeline and Background Execution
명령 조합과 비동기 실행을 지원하도록 확장한 단계입니다.

- `parse_pipe()`를 통해 `cmd1 | cmd2 | ...` 형태의 파이프라인 분해
- `pipe()`와 `dup2()`를 사용해 프로세스 간 표준 입출력 연결
- `&` 파싱을 통한 foreground / background 실행 분기
- 파이프 실행 후 파일 디스크립터 정리

이 단계에서는 shell 사용성의 핵심인 **명령 연결과 비동기 실행**을 구현했습니다.

### Phase 3. Job Control and Signal Handling
interactive shell에 필요한 프로세스 제어 기능을 구현한 단계입니다.

- `jobs[]` 테이블 기반 job 상태 관리
- `jobs`, `kill`, `bg`, `fg` built-in 명령 구현
- `SIGCHLD` 핸들러를 통한 자식 프로세스 종료 / 중지 / 재개 상태 반영
- `SIGINT`, `SIGTSTP` 처리 및 foreground job 동기화

이 단계에서는 foreground와 background 프로세스를 구분해 관리하고  
signal 기반으로 상태를 갱신하는 **실제 shell에 가까운 제어 흐름**을 구현했습니다.

## What I Built

- 명령 문자열을 파싱해 외부 명령 실행 및 built-in 처리로 연결하는 shell 루프를 구현했습니다.
- 파이프라인 명령을 분해하고 `pipe()` / `dup2()`를 사용해 프로세스 간 입출력을 연결했습니다.
- `&` 기반 background 실행을 지원하고 foreground 실행과 명확히 분기했습니다.
- job 테이블을 구성해 실행 중인 프로세스 상태를 관리했습니다.
- `SIGCHLD`, `SIGINT`, `SIGTSTP`를 처리해 자식 프로세스 상태 갱신과 foreground 제어를 구현했습니다.
- `jobs`, `bg`, `fg`, `kill` 명령을 통해 interactive shell 수준의 job control 기능을 구성했습니다.

## Technical Focus

이 프로젝트에서 중점적으로 다룬 부분은 다음과 같습니다.

- **Process Creation**: `fork`, `execvp`, `wait` 기반 명령 실행
- **Pipeline Handling**: `pipe`, `dup2`를 이용한 프로세스 간 데이터 전달
- **Job Control**: foreground / background job 관리
- **Signal Handling**: `SIGCHLD`, `SIGINT`, `SIGTSTP` 처리
- **Shell Execution Model**: 입력 파싱부터 실행, 상태 갱신까지의 전체 흐름 구현

## Key Functions

- `main()`
- `eval()`
- `parse_line()`
- `parse_pipe()`
- `builtin_command()`
- `sigchld_handler()`
- `show_jobs()`
- `kill_job()`
- `bg_wake()`
- `fg_run()`
- `parse_job()`
