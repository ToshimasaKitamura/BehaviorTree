#include "ShotGunAttackAction.h"
#include "Actor/Player/PlayerMotion.h"
#include "Actor/Partner/Partner.h"
#include "Actor/Weapon/Weapon.h"
#include "Actor/PlayerBullet/BulletStatus.h"
#include "Actor/Zombie/Zombie.h"
#include <numeric>

constexpr float floorDistance = 1.0f;
constexpr float attackPower = 2.0f;

void ShotGunAttackAction::Initialize()
{
	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	if (cowner) cowner->ChangeState(0, PlayerMotion::ShotGun, false);
	auto powner = dynamic_cast<Partner*>(m_common.m_owner);
	powner->ChangeWeapon(Weapon::WeaponType::ShotGun);

	m_isEnd = !Attack();
}

bool ShotGunAttackAction::Update(float deltaTime)
{
	if (m_isEnd) return false;

	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	if (cowner->EndAnimation()) return false;
	else return true;
}

void ShotGunAttackAction::Finalize()
{
	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	cowner->ChangeState(1, PlayerMotion::Idle, true);
}

bool ShotGunAttackAction::Attack() const
{
	// <Partner*>でパートナーを取得
	// <Character*>でパートナーを取得
	// <vector<Actor*>>ですべての敵を取得
	auto powner = dynamic_cast<Partner*>(m_common.m_owner);
	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	auto enemies = m_common.m_world->FindActorWithTag("Enemy");

	// 情報が取れなかった場合、終了
	if (!powner || !cowner || enemies.empty()) return false;

	// ターゲットのインスタンス
	Actor* target = 0;
	// パートナーの座標
	const auto partnerPos = powner->Transform().position();
	// 最小距離(最初はfloatのmax値で初期化)
	auto minDistance = std::numeric_limits<float>::max();

	// 敵を全探索
	for (const auto& enemy : enemies)
	{
		// 敵の座標
		auto enemyPos = enemy->Transform().position();
		// 他のフロアに反応しないようにする
		if (std::abs(enemyPos.y - partnerPos.y) > floorDistance) continue;
		// <Zombie*>でターゲットを取得
		auto zombie = dynamic_cast<Zombie*>(enemy);
		if (!zombie) continue;
		if (zombie->IsDeadMotion()) continue;

		// 最小距離を更新できるならば更新
		if (partnerPos.distance(enemyPos) < minDistance)
		{
			target = enemy;
			minDistance = partnerPos.distance(enemyPos);
		}
	}

	// 情報が取れなかった場合、終了フラグをオン
	if (!target) return false;

	// <Zombie*>でターゲットを取得
	auto zombie = dynamic_cast<Zombie*>(target);

	// 情報が取れなかった場合、終了
	// 死亡モーションに入っている場合、終了
	if (!zombie) return false;
	if (zombie->IsDeadMotion()) return false;

	// 敵の方向を向く
	auto lookAtTarget = target->Transform().position();
	lookAtTarget.y = m_common.m_owner->Transform().position().y;
	powner->Transform().lookAt(lookAtTarget);

	// 弾を生成
	BulletStatus bulletStatus(m_common.m_owner, attackPower);
	// 直接メッセージでの攻撃
	target->HandleMessage("ApplyShot", powner, &bulletStatus);

	powner->Attack();

	return true;
}