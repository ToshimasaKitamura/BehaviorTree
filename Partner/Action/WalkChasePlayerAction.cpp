#include "WalkChasePlayerAction.h"
#include "AI/BehaviorTree/Node/Action.h"
#include "Collide/BoundingSphere.h"
#include "Field/Field.h"
#include "Collide/Line.h"
#include "../Partner.h"
#include "Actor/Player/PlayerMotion.h"
#include "Actor/Character/Character.h"
#include "Actor/Object/Door/DoorOpen.h"
#include <numeric>

constexpr float ArrivalDistance = 1.0f;
constexpr float Speed = 0.025f;

void WalkChasePlayerAction::Initialize()
{
	m_agent = std::make_shared<NavGraphAgent>(m_common.m_owner, m_common.m_world, Speed);

	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	if (cowner) cowner->ChangeState(0, PlayerMotion::Walk, true);
	auto powner = dynamic_cast<Partner*>(m_common.m_owner);
	powner->ChangeWeapon(Weapon::WeaponType::Knife);
}

bool WalkChasePlayerAction::Update(float deltaTime)
{
	// プレイヤーを取得
	auto player = m_common.m_world->FindActor("Player");
	if (!player) return false;

	// ターゲットを設定
	m_agent->SetTarget(player);
	// 近ければ追いかけない
	if (isClose()) return false;

	// 近ければ追いかけない
	if (isClose()) return false;
	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	if (!cowner) return false;
	auto items = cowner->World()->FindContainsTag("Item");
	if (items.empty()) return false;

	auto GetNearestDoor = [=]() -> Actor*
	{
		constexpr float doorHitRange = 1.5f;

		// 最小距離(最初はfloatのmax値で初期化)
		auto minDistance = std::numeric_limits<float>::max();
		Actor* result = 0;
		for (const auto& item : items)
		{
			if (!item->ContainsName("Door")) continue;

			// それぞれの位置
			auto partnerPos = m_common.m_owner->Transform().position();
			auto itemPos = item->Transform().position();
			// パートナーとドアの距離を測る
			float distance = partnerPos.distance(itemPos);
			if (distance > doorHitRange) continue;

			if (distance < minDistance)
			{
				minDistance = distance;
				result = item;
			}
		}
		return result;
	};

	auto doorActor = GetNearestDoor();
	if (doorActor)
	{
		auto door = dynamic_cast<DoorOpen*>(doorActor);
		if (!door->IsOpenDoor())
			cowner->World()->Message("Interact", cowner, door);
	}

	// アージェントのアップデート
	m_agent->Update(deltaTime);

	return true;
}

void WalkChasePlayerAction::Finalize()
{
}

bool WalkChasePlayerAction::isClose() const
{
	// プレイヤーを取得
	auto player = m_common.m_world->FindActor("Player");
	if (!player) return false;
	Line line{ m_common.m_owner->Collider().m_center, player->Collider().m_center };
	if (m_common.m_world->GetField()->Collide(line)) return false;
	// プレイヤーとの距離
	float distance = m_common.m_owner->TargetDistance(player->Transform().position());
	return distance <= ArrivalDistance;
}
