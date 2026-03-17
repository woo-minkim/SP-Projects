# Lab 2: myshell 구현

Lab 2는 Unix 스타일 쉘을 C로 구현한 프로젝트입니다.  
명령 실행뿐 아니라 파이프, 백그라운드 실행, 잡 제어, 시그널 처리를 포함합니다.

## 핵심 기능

- 외부 명령 실행(`execvp`)
- 파이프라인 실행(`cmd1 | cmd2 | ...`)
- 백그라운드 실행(`&`)
- 내장 명령: `cd`, `exit`, `jobs`, `kill`, `bg`, `fg`
- 시그널 처리: `SIGCHLD`, `SIGINT`, `SIGTSTP`
- Job 테이블 기반 foreground/background 상태 관리

## 주요 파일

- `src/myshell.c`: 쉘 메인 로직(파싱, 포크/실행, 잡 관리, 시그널 핸들러)
- `src/myshell.h`: 공용 선언
- `src/Makefile`: 빌드 스크립트
- `specification.pdf`, `document.pdf`: 과제 문서

## 빌드 및 실행

실행 위치는 `Lab 2/src`입니다.

```bash
make
./myshell
```

프롬프트 예시: `CSE4100-SP-P2>`

## 내가 구현한 포인트

- 파이프 개수 제한 안에서 다중 명령을 분해/연결하고 FD를 정리하는 실행 흐름 구현
- foreground 대기와 background 진행을 분리한 job control 구성
- `jobs/bg/fg/kill` 명령이 PID 기반으로 동작하도록 상태 갱신 로직 구현

