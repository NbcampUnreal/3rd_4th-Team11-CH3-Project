- **프로젝트 소개**
    - 프로젝트 이름
        - Last Man
    - 프로젝트 개요
        - 퀘스트 기반 싱글 FPS 게임
    - 20250820 기준 플레이 영상
        - https://youtu.be/uWOr8CslTAI
    - 주요 기능 요약
        - 적 AI - 근거리, 원거리 일반 몬스터와 보스 몬스터가 BT 기반으로 플레이어 공격
        - 플레이어 - 히트스캔을 통한 사격으로 적 AI 공격, 아이템 획득, 아이템 사용, 아이템 장착
        - UI - 퀘스트, 플레이어 체력, 남은 탄알 수, 히트 마커, 피격 데미지, 적 사망 표시, 보스 몬스터 체력,   인벤토리 슬롯, 퀵 슬롯 등 표시
    - 조작법
        - 이동 : W, A, S, D
        - 앉기 : L_Ctrl
        - 점프 : Space Bar
        - 달리기 : L_Shift
        - 아이템사용 : 1, 2, 3, 4
        - 아이템 획득, 카드키 상호작용 : E
        - 일시정지 : P
        - 재장전 : R
        - 무기교체 : T (미구현)
        - 사격 : L_MouseButton
        - 조준 : R_MouseButton
  
- **개발 환경**
    - 개발 언어 및 도구
        - 엔진 : 언리얼 엔진 5.6
        - IDE : Visual Studio 2022
        - Compiler : MSVC
        - 언어 : C++
- **프로젝트 구조**


- **핵심 로직/클래스 설명**
    

- **주의사항**
    - LFS 용량 문제로 맵, AI 관련 애셋 폴더 추가 다운로드 필요 (다운로드 완료 후 Content폴더 아래 배치)
    - 스타트에임 중 새 프레임이 시작되기 전 리로드, 스탑에임 동시 호출하면 속도가 줄었다가 회복되지 않는 문제가 있어서 우클릭 중 재장전과 동시에 우클릭을 해제하면 버그가 발생할 수 있음.
    - T 키는 델리게이트 문제로 인해 누를 시 플레이가 중단됨.


      
🧑‍💻 Github 협업 규칙

- dev 브랜치 ← feature/기능명 브랜치에서 PR 진행
- PR 기반 개발
- 개발은 반드시 Pull Request 기반으로 진행
- 이슈 등록
- 해결이 어려운 오류는 GitHub Issue 등록 + Slack 공유
- 개발 계획 공유
- 개발 전, 오늘 어떤 기능을 구현할지 구체적으로 공유
- PR 알림
- PR 올리기 전/직후 2회, 팀원에게 Slack으로 알림
- 머지 시간 예시
- 매일 21:00경 머지 검토 및 승인 (주말 제외)
- 새 기능 시작 시
- 새로운 기능 시작할 땐 feature/기능명 브랜치 새로 생성 후 Slack 공유

- PR 시 규칙
　└ 푸시/커밋 제목 규칙
    [기능] 로그인 구현 또는 [버그] 충돌 수정 형식 사용
　└ 설명 명확히
    어떤 클래스/함수/파일을 수정했는지 명확히 기재
　└ 코드 리뷰 
    중간점검으로 총 2회 진행 / 주 1회 다른 사람 코드 (리뷰어 1명 지정해서) 진행 

- 버그 수정은 커밋 제목 앞에 [버그]
- 기능 구현은 앞에 [기능]
- 코드 구조를 개선했으면 [리펙토링]
- UI는 [UI]
- 주석 수정만 했으면 [문서]
- 디버깅이나 테스트 코드를 넣었으면 [테스트]
- 코드 제거했으면 [삭제]
<예시> 
** 고치기 힘든 이슈가 있다면 Github에 이슈 생성 후 ‘#{이슈번호}’ 꼭 붙이기
‘#{이슈번호} [태그] 한 줄 요약’ 으로 커밋/푸시



- **사운드 파일 라이선스/제작자**
    - 사격 :    Famas-1_smoothcriminal_toomuchlel.wav by quinte_besa -- https://freesound.org/s/369124/ -- License: Attribution 3.0
    - 빈 탄창 사격 : Rifle clip empty by michorvath -- https://freesound.org/s/427603/ -- License: Creative Commons 0
    - 플레이어 발소리 : footstep-concrete.wav by swuing -- https://freesound.org/s/38873/ -- License: Attribution 3.0
    - 플레이어 점프 :  jump.wav by acebrian -- https://freesound.org/s/380471/ -- License: Creative Commons 0
    - 플레이어 착지 : The sound of the jump by Ziggler_Games -- https://freesound.org/s/464336/ -- License: Attribution NonCommercial 4.0
    - 재장전 : MP5 Reloading by KeshaFilm -- https://freesound.org/s/628694/ -- License: Creative Commons 0
    - 보스레벨 bgm :
    Hydra And Ash by Makai Symphony | https://soundcloud.com/makai-symphony
    Music promoted by https://www.chosic.com/free-music/all/
    Creative Commons CC BY-SA 3.0
    https://creativecommons.org/licenses/by-sa/3.0/
    - 메인레벨 bgm :
    Sci-Fi Dramatic Theme by Twisterium | https://www.twisterium.com/
    Music promoted by https://www.chosic.com/free-music/all/
    Licensed under Creative Commons: Attribution 3.0 Unported (CC BY 3.0)
    https://creativecommons.org/licenses/by/3.0/
    - 메인메뉴레벨 bgm :
    Terminus by Scott Buckley | [www.scottbuckley.com.au](http://www.scottbuckley.com.au/)
    Music promoted by https://www.chosic.com/free-music/all/
    Creative Commons CC BY 4.0
    https://creativecommons.org/licenses/by/4.0/
    - 고양이 동상 사용 효과음 : https://pixabay.com/ko/users/soulfuljamtracks-46363515/
    - 아드레날린 사용 효과음 : https://pixabay.com/ko/users/nahtt-49618517/
    - 체력 포션 사용 효과음 : https://pixabay.com/ko/users/freesound_community-46691455/

