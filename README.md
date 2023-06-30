# LHJSample

git 주소 : https://github.com/joo6411/LHJSample.git

1. 이헌주 포트폴리오용 채팅서버 설명

c++20 이상에서 작동하고 DB로 Redis와 MS-SQL Server를 사용합니다
각 서버의 ip와 port는 Config.xml에서 변경 가능합니다.

iocp 기반의 채팅서버로 멀티스레드를 사용하여 워커스레드 4개와 패킷프로세서 1개, Redis 스레드 1개를 사용합니다.

DB생성은 
01_Account.sql
02_Account_Table.sql
03_Account_Procedure.sql
04_Account_Init.sql
을 순서대로 MS-SQL SERVER에 쿼리문을 날려 생성하시면 됩니다.

- Config.xml파일에서 ChatServer DB Redis 세부정보 변경 가능합니다

아직 코드에 미흡한 부분이 많아 미구현된 부분과 버그가 많고 서버가 제대로 돌아가지 않습니다
코드 참고용으로만 사용해주시면 감사하겠습니다
빨리 만들겠습니다
양해 부탁드립니다


2. todo list
 - 봇 구현 및 벤치마크 테스트(에코클라이언트만 테스트해봄)
 - Redis에 채팅 로그 저장
 - sql server에 계정정보 저장
 - DB connection 비동기로 돌리기
 - 