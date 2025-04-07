/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Include
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "Partner.h"
#include "Assets.h"
#include "World/IWorld.h"
#include "Collide/Line.h"
#include "Field/Field.h"
#include <vector>
#include "Input/MyInput.h"
#include "Utility/Utility.h"
#include "Actor/Weapon/HandGun.h"
#include "Actor/Weapon/Knife.h"
#include "Actor/Weapon/ShotGun.h"
#include "Actor/Player/PlayerMotion.h"
#include "Actor/Player/State/PlayerState.h"
#include "Actor/AttackCollider/AttackCollider.h"

/* Behavior Tree */
#include "AI/BehaviorTree/Node/Selector.h"
#include "AI/BehaviorTree/Node/Sequencer.h"
#include "AI/BehaviorTree/Node/Root.h"
#include "AI/BehaviorTree/Node/Action.h"
#include "AI/BehaviorTree/Node/Random.h"

/* Action */
#include "Action/RunChasePlayerAction.h"
#include "Action/WalkChasePlayerAction.h"
#include "Action/IdleAction.h"
#include "Action/KnifeAttackAction.h"
#include "Action/HandGunAttackAction.h"
#include "Action/ShotGunAttackAction.h"
#include "Action/ReloadHandGunAction.h"
#include "Action/ReloadShotgunAction.h"
#include "Action/TeleportAction.h"

/* Conditional */
#include "Conditional/IsSeperatedPlayerConditional.h"
#include "Conditional/IsClosedEnemyConditional.h"
#include "Conditional/IsPlayerAttackConditional.h"
#include "Conditional/IsReceiveGatherOrderConditional.h"
#include "Conditional/ShotgunBulletIsExistsConditional.h"
#include "Conditional/ShotgunMagazineIsExistsConditional.h"
#include "Conditional/HandGunBulletIsExistsConditional.h"
#include "Conditional/HandGunMagazineIsExistsConditional.h"
#include "Conditional/LostSightOfPlayerConditional.h"


/* Debug Macro */
#define PRINT(str, ...){char c[256];sprintf_s(c, str, __VA_ARGS__);OutputDebugStringA(c);}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Motions 
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum {
	MotionIdle = 2,
};

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Const Value
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
constexpr float PartnerHeight{ 1.0f };			// 身長
constexpr float PartnerRadius{ 0.35f };			// 当たり判定用コライダーの半径
constexpr float RunSpeed{ 0.1f };				// 走るスピード
constexpr float WalkSpeed{ 0.03f };				// 歩くスピード
constexpr float AcceptableRaduis{ 1.0f };		// 目的地を円の中心と考えた時、半径何cm離れていた場合は移動をONにするか
constexpr float Gravity{ -0.023f };				// 重力
constexpr float InvincibleTimeLimit{ 60.0f };	// 無敵時間
constexpr float AttackPower{ 1.0f };			// 攻撃力
constexpr int PistolCount = 13;					// ハンドガンボーン
constexpr int KnifeCount = 40;					// ナイフボーン
constexpr int ShotGunCount = 43;				// ショットガンボーン
constexpr float PartnerHP{ 10.0f };				// パートナのHP

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Constructor
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
Partner::Partner(IWorld* world, GSvector3 position, GSuint modelID) :
	Character(modelID)
{
	m_pIWorld = world;
	m_tag = "HumanTag";	// プレイヤーのタグと名前を合わせるため
	m_name = "Partner";
	m_collider = BoundingSphere{ PartnerRadius, GSvector3{0.0f, PartnerHeight, 0.0f } };
	m_transform.position(position);
	m_mesh.Transform(m_transform.localToWorldMatrix());
	m_infectiousness = 0.0f;
	m_weapons[Weapon::WeaponType::Knife] = std::make_shared<Knife>(m_pIWorld);
	m_weapons[Weapon::WeaponType::HandGun] = std::make_shared<HandGun>(m_pIWorld);
	m_weapons[Weapon::WeaponType::ShotGun] = std::make_shared<ShotGun>(m_pIWorld);

	//近接モーションで特定のフレーム時攻撃する
	m_mesh.AddEvent(PlayerMotion::Attack, 32, [&]()
		{
			const float life{ 15.0f };
			const float radius{ 0.21f };//test 1.0f
			GSvector3 forward{ m_transform.forward() };
			GSvector3 position{ m_transform.position() };
			position = position + forward;
			position.y += PartnerHeight * 1.6f;
			m_pIWorld->AddActor(new AttackCollider{ m_pIWorld,BoundingSphere{radius, position} ,this,life, AttackPower });
		});

	// BehaviorTreeの作成
	CreateBehaviorTree();
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Public Method
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
void Partner::UpdateCharacter(float deltaTime)
{
	// デルタタイムの更新
	m_deltaTime = deltaTime;

	switch (m_state)
	{
	case Partner::State::Healthy:
		// ビヘイビアツリーの更新
		m_behaviorTree->Update(deltaTime);
		break;
;	case Partner::State::Damage:
		// ダメージ処理
		DamageProc();
	case Partner::State::Invincible:
		// 無敵処理
		InvincibleProc();
		// ビヘイビアツリーの更新
		m_behaviorTree->Update(deltaTime);
		break;
	case Partner::State::Die:
		break;
	default:
		break;
	}

	m_gatherOrderTime--;
}

void Partner::UpdateGravity(float deltaTime)
{
	m_velocity.y += Gravity * deltaTime; //std::min(Gravity * deltaTime, 0.0f);
	// 重力を加える
	m_transform.translate(0.0f, m_velocity.y * deltaTime, 0.0f);
}

void Partner::UpdateWeapon(float deltaTime)
{
	int boneNumber;
	if (m_weapon == Weapon::WeaponType::Knife)
	{
		boneNumber = KnifeCount;
	}
	else if (m_weapon == Weapon::WeaponType::HandGun)
	{
		boneNumber = PistolCount;
	}
	else if(m_weapon == Weapon::WeaponType::ShotGun)
	{
		boneNumber = ShotGunCount;
	}
	m_weapons[m_weapon]->Update(m_mesh.BoneMatrices(boneNumber), deltaTime);
}

void Partner::DrawWeapon() const
{
	m_weapons.find(m_weapon)->second->Draw();
}

void Partner::Draw() const
{
	//DrawWeapon();
	Actor* camera = m_pIWorld->GetCamera();
	if (!camera) return;
	glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);

	gsSetRimLightColor(&m_rimColor);
	gsSetRimLightExponent(m_rim);

	// メッシュ描画
	m_mesh.Draw();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	// 判定球の描画
	Collider().Draw();
	// 武器の描画
	DrawWeapon();

	GScolor col{ 0.0f,0.0f,0.0f,0.0f };
	gsSetRimLightColor(&col);
	gsSetRimLightExponent(FLT_MAX);
}

void Partner::CollideGround()
{
	//足元のオフセット
	const float footOffset{ 0.1f };
	GSfloat LerpTime{ 1.65f };    // 補間フレーム数
	const float SmoothTime{ 0.5f };    // 補間フレーム数
	const float MaxSpeed{ 0.29f };       // 移動スピードの最大値
	GSvector3 posY{ 0.0f,0.0f,0.0f };
	//地面との衝突判定（線分との交差判定）
	GSvector3 position = m_transform.position();
	Line line;
	line.m_start = position + m_collider.m_center;
	line.m_end = position + GSvector3{ 0.0f,-footOffset,0.0f };
	GSvector3 intersect;//地面との交点

	if (m_pIWorld->GetField()->Collide(line, &intersect))
	{

		//交差した点からY座標のみ補正
		position.y = intersect.y;

		//DampよりLerpのほうがいいかも
		/* position.y= GSvector3::lerp(m_velocity,position, LerpTime).y;*/
		position.y = GSvector3::smoothDamp(m_transform.position(),
			position, m_velocity, SmoothTime, MaxSpeed, m_deltaTime).y;

		//座標を変更
		m_transform.position(position);
		//重力の初期化
		m_velocity.y = 0.0f;
	}
}

void Partner::HandleMessage(const std::string& message, Actor* sender, void* param)
{
	if (message == "GatherOrder")
	{
		m_gatherOrderTime = 60;
	}
}

void Partner::ChangeWeapon(Weapon::WeaponType weapon)
{
	m_weapon = weapon;
}

void Partner::Attack()
{
	if (m_weapons[m_weapon]->is_Attack())
		m_weapons[m_weapon]->Attack(this);
}

int Partner::GetBulletNum(Weapon::WeaponType weapon) const
{
	return m_weapons.find(weapon)->second->GetBulletNum();
}

int Partner::GetMagazineNum(Weapon::WeaponType weapon) const
{
	return  m_weapons.find(weapon)->second->GetMagazineNum();
}

void Partner::Reload(Weapon::WeaponType weapon) const
{
	m_weapons.find(weapon)->second->Reload();
}

bool Partner::IsGatherOrder() const
{
	return m_gatherOrderTime > 0;
}

void Partner::React(Actor& other)
{
	if (m_state != State::Healthy) return;

	if (other.Tag() == "Enemy")
	{
		m_infectiousness += 1.0f;
		if (m_infectiousness >= PartnerHP)
		{
			ChangeState(PlayerMotion::Die, PlayerMotion::Die, false);
			m_state = State::Die;
		}
		else
		{
			ChangeState(PlayerMotion::Damage, PlayerMotion::Damage, false);
			m_state = State::Damage;
		}
	}
	CollideActor(other);
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Private Method
/*------------------------------------------------------------------------------------------------------------------------------------------------------*/
void Partner::CreateBehaviorTree()
{
	/* Composite Node */
	auto root = std::make_shared<Root>();
	auto mother = std::make_shared<Selector>();
	auto order = std::make_shared<Sequencer>();
	auto attack = std::make_shared<Sequencer>();
	auto move = std::make_shared<Selector>();
	auto doAttack = std::make_shared<Selector>();
	auto attackCondibion = std::make_shared<Selector>();
	auto longRange = std::make_shared<Sequencer>();
	auto shortRange = std::make_shared<Sequencer>();
	auto shotGun = std::make_shared<Selector>();
	auto handGun = std::make_shared<Selector>();
	auto onShotGun = std::make_shared<Sequencer>();
	auto shotGunReload = std::make_shared<Sequencer>();
	auto onHandGun = std::make_shared<Sequencer>();
	auto handGunReload = std::make_shared<Sequencer>();
	auto doTeleport = std::make_shared<Sequencer>();
	auto doRun = std::make_shared<Sequencer>();
	auto doWalk = std::make_shared<Sequencer>();


	/* Action Node */
	auto idleAction = std::make_shared<IdleAction>();
	auto runChasePlayerAction = std::make_shared<RunChasePlayerAction>();
	auto walkChasePlayerAction = std::make_shared<WalkChasePlayerAction>();
	auto knifeAttackAction = std::make_shared<KnifeAttackAction>();
	auto handGunAttackAction = std::make_shared<HandGunAttackAction>();
	auto reloadHandGunAction = std::make_shared<ReloadHandGunAction>();
	auto shotGunAttackAction = std::make_shared<ShotGunAttackAction>();
	auto reloadShotGunAction = std::make_shared<ReloadShotGunAction>();
	auto teleportAction = std::make_shared<TeleportAction>();

	/* Conditional Node */
	auto isClosedLongEnemyConditional = std::make_shared<IsClosedEnemyConditional>(4.5f);
	auto isClosedShortEnemyConditional = std::make_shared<IsClosedEnemyConditional>(0.5f);
	auto isPlayerAttackConditional = std::make_shared<IsPlayerAttackConditional>();
	auto isReceiveGatherOrderConditional = std::make_shared<IsReceiveGatherOrderConditional>();
	auto isSeperatedLongPlayerConditional = std::make_shared<IsSeperatedPlayerConditional>(3.0f);
	auto isSeperatedShortPlayerConditional = std::make_shared<IsSeperatedPlayerConditional>(2.0f);
	auto shotgunBulletIsExistsConditional = std::make_shared<ShotgunBulletIsExistsConditional>();
	auto shotgunMagazineIsExistsConditional = std::make_shared<ShotgunMagazineIsExistsConditional>();
	auto handGunBulletIsExistsConditional = std::make_shared<HandGunBulletIsExistsConditional>();
	auto handGunMagazineIsExistsConditional = std::make_shared<HandGunMagazineIsExistsConditional>();
	auto lostSightOfPlayerConditional = std::make_shared<LostSightOfPlayerConditional>();

	/* Add Nodes*/
	onShotGun->AddChild(shotgunBulletIsExistsConditional);
	onShotGun->AddChild(shotGunAttackAction);
	shotGunReload->AddChild(shotgunMagazineIsExistsConditional);
	shotGunReload->AddChild(reloadShotGunAction);
	onHandGun->AddChild(handGunBulletIsExistsConditional);
	onHandGun->AddChild(handGunAttackAction);
	handGunReload->AddChild(handGunMagazineIsExistsConditional);
	handGunReload->AddChild(reloadHandGunAction);
	shotGun->AddChild(onShotGun);
	shotGun->AddChild(shotGunReload);
	handGun->AddChild(onHandGun);
	handGun->AddChild(handGunReload);
	longRange->AddChild(shotGun);
	longRange->AddChild(handGun);
	shortRange->AddChild(isClosedShortEnemyConditional);
	shortRange->AddChild(knifeAttackAction);
	//attackCondibion->AddChild(isPlayerAttackConditional);
	attackCondibion->AddChild(isClosedLongEnemyConditional);
	doAttack->AddChild(longRange);
	doAttack->AddChild(shortRange);
	order->AddChild(isReceiveGatherOrderConditional);
	order->AddChild(runChasePlayerAction);
	attack->AddChild(attackCondibion);
	attack->AddChild(doAttack);
	doTeleport->AddChild(lostSightOfPlayerConditional);
	doTeleport->AddChild(teleportAction);
	doRun->AddChild(isSeperatedLongPlayerConditional);
	doRun->AddChild(runChasePlayerAction);
	doWalk->AddChild(isSeperatedShortPlayerConditional);
	doWalk->AddChild(walkChasePlayerAction);
	move->AddChild(doTeleport);
	move->AddChild(doRun);
	move->AddChild(doWalk);
	mother->AddChild(order);
	mother->AddChild(attack);
	mother->AddChild(move);
	mother->AddChild(idleAction);
	root->AddChild(mother);

	/* Make Tree*/
	m_behaviorTree = std::make_shared<BehaviorTree>(root, m_pIWorld, this);
}

void Partner::InvincibleProc()
{
	m_invincibleTimer += m_deltaTime;
	if (m_invincibleTimer >= InvincibleTimeLimit)
	{
		m_invincibleTimer = 0.0f;
		m_state = State::Healthy;
	}
}

void Partner::DamageProc()
{
	if (EndAnimation())
	{
		ChangeState(0, PlayerMotion::Idle, true);
		m_state = State::Invincible;
		m_behaviorTree->ResetActionNode();
	}
}