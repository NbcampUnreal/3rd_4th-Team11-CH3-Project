# Last Man

> 퀘스트 기반 FPS 게임 - Unreal Engine 5.6

## 프로젝트 소개

### 개요
- **프로젝트명**: Last Man
- **목적**: 게임 개발 협업과 FPS 게임 기획부터 개발까지의 전체 과정 실습
- **장르**: 퀘스트 기반 1인칭 슈팅 게임 (FPS)
- **게임 흐름**: 퀘스트를 통해 몬스터를 처치하며 진행 → 최종 보스 몬스터 도달 → 보스 처치 후 게임 클리어

### 플레이 영상
- [YouTube 플레이 영상 (2025.08.20)](https://youtu.be/uWOr8CslTAI)

---

## 개발 환경

| 항목 | 내용 |
|------|------|
| 엔진 | Unreal Engine 5.6 |
| IDE | Visual Studio 2022 |
| 컴파일러 | MSVC |
| 언어 | C++ |

---

## 주요 기능

### 적 AI 시스템
- **Behavior Tree 기반**: 근거리/원거리 일반 몬스터와 보스 몬스터 AI
- **AI Perception System**: 시각(Sight), 청각(Hearing), 피해(Damage) 감지
- **EQS (Environment Query System)**: 환경을 고려한 지능형 포지셔닝
- **보스 페이즈 시스템**: HP에 따른 3단계 페이즈, 5종류 공격 패턴

### 플레이어 시스템
- **히트스캔 사격**: 1인칭 시점 전투
- **무기 시스템**: 라이플, 샷건 등 다양한 무기
- **아이템 시스템**: 획득, 사용, 장착 기능
- **스탯 관리**: 체력, 방어력, 크리티컬 대미지

### UI 시스템
- **HUD**: 체력바, 탄약 수, 퀘스트 텍스트, 보스 체력바
- **전투 피드백**: 히트 마커, 킬 마커, 대미지 텍스트
- **인벤토리**: 퀵슬롯(1-4), 드래그 앤 드롭 인벤토리

### 퀘스트 시스템
- **멀티 스테이지**: 9단계 진행 (Progress 00~08)
- **동적 스폰**: 퀘스트 진행에 따른 적 스폰 관리

---

## 프로젝트 구조

```
Source/CH03Project/
├── Public/
│   ├── AI/                    # AI 시스템
│   │   ├── EnemyAIController.h
│   │   ├── EEnemyState.h
│   │   ├── BTS_AttackSelect.h
│   │   ├── BTT_DefaultAttack.h
│   │   ├── BTT_ChargeAttack.h
│   │   ├── BTT_LasorAttack.h
│   │   ├── BTT_LRMAttack.h
│   │   ├── BTT_MissileAttack.h
│   │   └── BTT_MoveAlongWayPoints.h
│   │
│   ├── Items/                 # 아이템 시스템
│   │   ├── BaseItem.h
│   │   ├── InventoryComponent.h
│   │   ├── HealthPotion.h
│   │   ├── Adrenaline.h
│   │   └── CardKey.h
│   │
│   ├── Weapons/               # 무기 시스템
│   │   ├── BaseWeapon.h
│   │   ├── BaseRangedWeapon.h
│   │   ├── Rifle_1.h
│   │   └── Shotgun_1.h
│   │
│   ├── Character/             # 캐릭터 시스템
│   │   ├── MyCharacter.h
│   │   ├── MyPlayerController.h
│   │   ├── BaseEnemy.h
│   │   └── BaseStatComponent.h
│   │
│   ├── UI/                    # UI 시스템
│   │   ├── HUDWidget.h
│   │   ├── MenuWidget.h
│   │   └── InventoryItemSlot.h
│   │
│   └── Game/                  # 게임 로직
│       ├── GameModePlay.h
│       ├── GameStatePlay.h
│       └── QuestTypeA.h
│
└── Private/                   # 구현 파일들
    └── [대응하는 .cpp 파일들]

Content/AI/
├── Characters/
│   ├── BP_AI_Boss.uasset
│   ├── BP_AI_Melee.uasset
│   └── BP_AI_Ranged.uasset
└── BT/
    ├── BB_Enemy.uasset
    ├── BT_Enemy_Melee.uasset
    ├── BT_Enemy_Ranged.uasset
    ├── BT_Enemy_Boss.uasset
    └── EQS/
        ├── EQS_NPoints.uasset
        ├── EQS_FindIdealRange.uasset
        └── EQS_FindCover.uasset
```

---

## 핵심 시스템 설명

### AI 시스템 아키텍처

#### AI Controller (EnemyAIController)
- **AI Perception Component**: 시각(800m), 청각(600m), 피해 감지
- **Blackboard**: 상태, 타겟, 공격 거리 등 데이터 저장
- **상태 관리**: Passive → Attacking → Investigating → Dead

#### Behavior Tree Tasks
| Task | 설명 |
|------|------|
| `BTT_DefaultAttack` | 기본 공격, 2단계 검증 시스템 (도달 가능성 + 공격 가능성) |
| `BTT_ChargeAttack` | 돌진 공격, 4단계 상태 머신 (Ready→JumpUp→Dash→Finish) |
| `BTT_MissileAttack` | 다중 미사일, EQS로 착탄점 계산 |
| `BTT_LasorAttack` | 레이저 빔, Niagara 이펙트 연동 |
| `BTT_LRMAttack` | 추적 미사일, 6단계 상태 머신 |

#### 보스 AI 페이즈 시스템
- **Phase 1 (HP 67-100%)**: Primary Fire, LRM, Charge
- **Phase 2 (HP 34-66%)**: + Missile 추가
- **Phase 3 (HP 0-33%)**: + Laser 추가

### 아이템 시스템
- **쿨타임 시스템**: 델리게이트 기반 실시간 UI 업데이트
- **드롭 시스템**: 확률 기반 가중치 랜덤 드롭
- **인벤토리**: 동적 크기, 아이템 추가/제거 이벤트

### 대미지 시스템
- **공격 토큰 시스템**: 동시 공격 제한
- **크리티컬 대미지**: 헤드샷 배율 적용
- **대미지 면역**: 무적 상태 관리

---

## 조작법

| 입력 | 기능 |
|------|------|
| W, A, S, D | 이동 |
| L_Shift | 달리기 |
| L_Ctrl | 앉기 |
| Space | 점프 |
| L_MouseButton | 사격 |
| R_MouseButton | 조준 |
| R | 재장전 |
| 1, 2, 3, 4 | 아이템 사용 |
| E | 아이템 획득 / 상호작용 |
| P | 일시정지 |

---

## 주의사항

- **LFS 용량 문제**: 맵, AI 관련 애셋 폴더 추가 다운로드 필요 (Content 폴더 아래 배치)
- **재장전 버그**: 조준 중 재장전과 동시에 조준 해제 시 이동 속도 감소 버그 발생 가능
- **T키 비활성화**: 델리게이트 문제로 인해 사용 시 플레이 중단

---

## Github 협업 규칙

### 브랜치 전략
- `dev` 브랜치 ← `feature/기능명` 브랜치에서 PR 진행
- PR 기반 개발 필수

### 커밋 메시지 규칙
| 태그 | 용도 |
|------|------|
| [기능] | 새 기능 구현 |
| [버그] | 버그 수정 |
| [리펙토링] | 코드 구조 개선 |
| [UI] | UI 관련 변경 |
| [문서] | 주석/문서 수정 |
| [테스트] | 테스트 코드 |
| [삭제] | 코드 제거 |

### 이슈 관리
- 해결이 어려운 오류는 GitHub Issue 등록 + Slack 공유
- 커밋 시 `#{이슈번호} [태그] 한 줄 요약` 형식 사용

---

## 사운드 파일 라이선스

| 사운드 | 출처 | 라이선스 |
|--------|------|----------|
| 사격 | [Freesound - quinte_besa](https://freesound.org/s/369124/) | Attribution 3.0 |
| 빈 탄창 | [Freesound - michorvath](https://freesound.org/s/427603/) | CC0 |
| 발소리 | [Freesound - swuing](https://freesound.org/s/38873/) | Attribution 3.0 |
| 점프 | [Freesound - acebrian](https://freesound.org/s/380471/) | CC0 |
| 착지 | [Freesound - Ziggler_Games](https://freesound.org/s/464336/) | Attribution NC 4.0 |
| 재장전 | [Freesound - KeshaFilm](https://freesound.org/s/628694/) | CC0 |
| 보스레벨 BGM | [Makai Symphony - Hydra And Ash](https://soundcloud.com/makai-symphony) | CC BY-SA 3.0 |
| 메인레벨 BGM | [Twisterium - Sci-Fi Dramatic Theme](https://www.twisterium.com/) | CC BY 3.0 |
| 메뉴 BGM | [Scott Buckley - Terminus](http://www.scottbuckley.com.au/) | CC BY 4.0 |
| 아이템 효과음 | Pixabay | Pixabay License |
