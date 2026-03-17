# Lab 2: `myshell` 구현 (Phase 1 ~ Phase 3)

## 개요

Lab 2는 Unix shell의 핵심 기능을 단계적으로 구현한 과제입니다.  
최종 목표는 명령 실행기 수준을 넘어, 파이프/백그라운드/잡 제어/시그널 처리를 포함한 interactive shell을 완성하는 것입니다.

## 구현 단계 (Phase)

### Phase 1: 기본 shell loop + 명령 실행

- 무엇을 구현했나
  - 프롬프트 출력 및 입력 루프
  - `parse_line()` 기반 인자 분해
  - `Fork()` + `execvp()`로 외부 명령 실행
  - 최소 built-in 처리(`cd`, `exit` 등)
- 왜 필요한가
  - shell의 최소 실행 단위(입력 → 파싱 → 실행 → 복귀)를 안정화해야 이후 phase 확장이 가능
- 코드 포인트
  - `main()`, `eval()`, `parse_line()`, `builtin_command()`

### Phase 2: 파이프라인 + 백그라운드 실행

- 무엇을 구현했나
  - `parse_pipe()`로 `cmd1 | cmd2 | ...` 분해
  - `pipe()` + `dup2()`로 프로세스 간 stdin/stdout 연결
  - `&` 파싱 후 foreground/background 분기 실행
  - 파이프 명령 종료 후 FD 정리
- 왜 필요한가
  - 실제 shell 사용성의 핵심인 조합 명령과 비동기 실행을 지원하기 위함
- 코드 포인트
  - `parse_pipe()`, `eval()` 내부 `cmdidx/cmdcnt` 분기, `bgflag`

### Phase 3: Job control + Signal 처리

- 무엇을 구현했나
  - job 테이블(`jobs[]`) 기반 상태 관리
  - `jobs`, `kill`, `bg`, `fg` built-in 구현
  - `SIGCHLD` 핸들러로 자식 종료/중지/재개 상태 반영
  - `SIGINT`, `SIGTSTP` 처리 및 foreground 대기 동기화
- 왜 필요한가
  - interactive shell에서 foreground/background 프로세스 제어를 일관되게 보장하기 위함
- 코드 포인트
  - `sigchld_handler()`, `show_jobs()`, `kill_job()`, `bg_wake()`, `fg_run()`, `parse_job()`

## 실행 방법

실행 위치: `Lab 2/src`

```bash
make
./myshell
```

프롬프트 예시:

```text
CSE4100-SP-P2>
```

테스트 예시:

```text
ls
ls | grep .c
sleep 10 &
jobs
fg %1
```

## 핵심 구현 포인트

- 파싱/실행/시그널/잡 관리를 분리해 phase 확장 시 충돌을 줄임
- foreground 대기(`sigsuspend`)와 background 진행을 명확히 분기
- `SIGCHLD` 기반 상태 갱신으로 zombie 프로세스 누적을 방지

## 한계 및 개선 여지

- 리다이렉션(`>`, `<`) 및 고급 quoting/escaping 처리는 미지원
- job ID/프로세스 그룹(`pgid`) 관리 체계를 더 엄격히 정리할 여지 있음
- 파싱 오류 메시지/복구 로직을 강화하면 interactive UX가 좋아짐
