#ifndef PARTNER_H_
#define PARTNER_H_

#include "Actor/Character/Character.h"
#include "Animation/AnimatedMesh.h"
#include "AI/BehaviorTree/BehaviorTree.h"
#include <memory>
#include "Actor/Weapon/Weapon.h"
#include "Actor/Character/Character.h"
#include "Actor/Weapon/Weapon.h"
#include <map>

class Partner : public Character
{
public:
	Partner(IWorld* world, GSvector3 position, GSuint modelId);

	void React(Actor& other) override;
	void UpdateCharacter(float deltaTime) override;
	void UpdateGravity(float deltaTime) override;
	void UpdateWeapon(float deltaTime) override;
	void CollideGround() override;
	void HandleMessage(const std::string& message, Actor* sender, void* param) override;

	// 武器を変更する
	void ChangeWeapon(Weapon::WeaponType weapon);
	void Attack();
	int GetBulletNum(Weapon::WeaponType weapon) const;
	int GetMagazineNum(Weapon::WeaponType weapon) const;
	void Reload(Weapon::WeaponType weapon) const;
	bool IsGatherOrder() const;

private:
	void DrawWeapon() const override;
	void Draw() const override;

private:
	// パートナーの状態enum
	// ステートマシンを使うほどではないと判断したため用意した
	enum class State
	{
		Healthy,
		Damage,
		Invincible,
		Die
	};

private:
	// ビヘイビアツリーを生成する関数
	void CreateBehaviorTree();
	// 無敵処理
	// TODO：名前を変更したいよ
	void InvincibleProc();
	// ダメージ処理
	// TODO：名前を変更したいよ
	void DamageProc();

private:
	// 生きている状態のパートナーの脳(AI)
	std::shared_ptr<BehaviorTree> m_behaviorTree;
	// パートナーの状態
	State m_state{State::Healthy};
	// デルタタイム取得用変数
	float m_deltaTime;
	// 感染度
	float m_infectiousness{ 0.0f };
	// 無敵時間カウンター
	float m_invincibleTimer{ 0.0f };

	// 所持している武器
	Weapon::WeaponType m_weapon{ Weapon::WeaponType::ShotGun };
	// 武器のデータ
	std::map<Weapon::WeaponType, std::shared_ptr<Weapon>> m_weapons;

	// 集合命令に使う変数
	int m_gatherOrderTime{ 0 };
};

#endif // !PARTNER_H_
