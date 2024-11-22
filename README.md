# MapleTutorial_Client
## 2인 개발 프로젝트(소켓 통신을 이용한 2인 파티플레이 사냥)에서 본인 분야를 가져와 정리. 
## 개발 분야 
## - (map , play , scene , monster) manager
## - monster(snail1 , snail2 , mushMom) , NPC , PlayerFinalAttack
## - 사운드 작업 , UI
 
캐릭터 : hp , mp , 죽음 이펙트 , 스폰 , level(경험치 , 최대레벨 100) , 공격력

- 플레이어 : 이동, 점프 , 공격 , 매달리기 , 스킬 ( 전직마다 스킬 1개)
    - 초보자 : 달팽이 3마리, 깃털, 이상한 회복
    - 직업 : 1-4차 각 1개씩 스킬
- NPC : 스탠드 , 상호작용
- 몬스터 : 이동(추적), 점프 , 획득 경험치
    - 일반 : 달팽이 , 색깔별 레벨 다름 
    - → 패턴 : 일정 좌표 좌우 이동 + 피격 시 플레이어 추적(일정 거리에서만 추적, 다른 발판 이동 시 쫓아오지 못함)
    - 보스 : 머쉬맘 
    - → 패턴 : idle ,(이동 , 랜덤으로 점프(점프 도중에는 점프모션, 바닥에 있을 시 idle로)) , 히트,히트 시 추적, 일정거리 이내 attack , death, 
    맵 (헤네시스) 
    - 발판 : 충돌판정
    - 밧줄 : 상호작용 함수
    - 포탈 : 맵 이동

- 공유사항
플레이어 좌표 , 레벨 , 직업 , hp
몬스터 좌표, hp

사운드 : 배경음 , 점프 사운드 , 공격 사운드 , 스킬 사운드, 레벨업 , 포탈 , 죽음
ui : hp, mp,경험치, level , 퀵 슬롯 
scene : 로그인 , 캐릭터 생성 , 맵 , 엔딩 신

개발 순서 : 제작 중 매니저 함께 제작

1. 맵 엔진, 초보자
2. 플레이어 확장 , (NPC , 몬스터)
3. 맵 제작 , ui 세팅 , 사운드 작업
4. Scene 제작
5. 서버 : 몬스터 스폰 , 캐릭터 좌표 공유 등 적용

클래스 정의
Object : 속성 -> Collision(width,height) , Draw, Sound , POINT ( 좌표)
- Character : 속성 -> Speed , Direction , Move, Jump , hp,mp , level , exp,공격력/방어력 , death, spawn, Attack
- Player : 속성 -> Skills  , Hanging Rope , inventory
- Base
- 직업 1
- 직업 2
- Monster : 속성 -> Move override (추적)
- snail1
- snail12
- snail3
- Boss (머쉬맘) : 속성 -> attack override ( 패턴 별 공격 : 몸박 , 점프, 추적 등)
- NPC : 속성 -> interaction

- Map
    - Platform : 속성 -> collsion : 플레이어.y > platform.y 인 경우만
    - Rope : 속성 -> collsion : 충돌 도중 윗방향키 클릭 시
    - Portal : 속성 -> collsion : 충돌 도중 윗방향키 클릭 시
    Scene
- Login : 닉네임 입력
- 캐릭터 직업 선택
- 맵 : 마을 -> 몬스터 -> 보스몬스터
- 엔딩 씬 : 마무리 이미지
UI
- QuickSlot
- Hp , mp , exp, level

Manager
- GameManger
- SceneManger
- ObjectManger
- SoundManager
- UiManager
- InputManager
