# cache_simulate_with_C

## 목표 

- 설정파일은 아래예시와 같다.

  clock_rate(Ghz): 2
  
  mem_access_latency(cycles): 250

  L1_size(Bytes): 4096
  
  L1_line_size(Bytes): 16
  
  set_associativity: 2

 - 설정파일 이름은 실행시 첫번째 COMMAND-LINE 파라미터로 받는다.
 - cache는 별도의 메모리 내용을 추적하기 위하여 64MB의 메모리를 할당받아 전부 0으로 초기화 된다.
 - 메모리에 액세스 되는 정보드는 아래 형태와 같으며 COMMAND-LINE 두번째 파라미터로 받는다.
    - 첫번째는 메모리 액세스 형태를 나타내며, 두번째는 32bit 주소의 십진수 값이다.
    - cache는 해당 word 가 포함되는 블럭을 한꺼번에 읽는다.
      -ex) 주소가 11byte 면 cache block size가 8byte 일경우 8~15byte 전체를 읽어들인다.
    - Write 의 경우 저장되어있는 데이터 값 또한 3번째 인자로 표시해준다. 
  
  R 362328
  
  R 585744
  
  R 585848
  
  W 491656 18
  
  ...
  
## 구현 단계 (순서)

1단계: 설정파일을 읽어들이고 확인을 위해 읽은 값들을 화면에 출력한다. 위의 출력 예시 이전에 출력되어야 함. 

2단계: 데이터파일을 열고 한줄씩 읽어서 cache가 없는 상태를 가정하고 access를 처리하고 결과를 출력한다. 즉, cache가 사용되지 않는 경우를 처리한다. 2단계 Avg latency가 화면에 찍혀야한다. 위의 출력 예시에서 맨 처음 보이는 줄에 해당한다. 

3단계: line size가 4byte, 즉 1 word 인 경우와 set associativity가 1인 경우를 구현한다. 

4단계: line size가 1word보다 큰 경우와 set associativity가 1인 경우를 구현한다. 

//5단계: line size가 1word보다 큰 경우와 set associativity가 1보다 큰 경우를 구현한다. => 미구현

  
  
