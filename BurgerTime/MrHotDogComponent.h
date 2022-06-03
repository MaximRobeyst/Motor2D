#pragma once
#include <Component.h>
#include <Transform.h>

#include <map>

namespace dae
{
	class AnimatorComponent;
	class RigidbodyComponent;
	class TransformComponent;
	class ColliderComponent;
}

class Subject;
class GameStateManager;
class b2World;

class EnemyComponent : public dae::Component
{
public:
	EnemyComponent();
	EnemyComponent(dae::GameObject* pGameobject, dae::TransformComponent* pPlayerTransform, int score = 100);
	~EnemyComponent() override;

	void Start() override;
	void Update() override;
	void Render() const override;

	void RenderGUI() override;

	void Serialize(rapidjson::PrettyWriter< rapidjson::StringBuffer>& writer) override;
	void Deserialize(dae::GameObject* /*pGameobject*/, rapidjson::Value& /*value*/) override;

	void EnemyDeath();
	void EnemyDrop();
	
	int GetScore() const;

	std::unique_ptr<Subject>& GetSubject();
private:
	bool SpaceUp() const;
	bool SpaceDown() const;
	bool SpaceLeft() const;
	bool SpaceRight() const;
	friend class EnemyStateMachine;
	friend class IEnemyState;

	float m_Timer{ 0.0f };
	float m_TimeBetweenTransitionChecks{ 0.05f };

	int m_Score{100};
	bool m_Dead{};

	float m_MinDistance{ 0.25f };

	dae::AnimatorComponent* m_pAnimatorComponent{};
	dae::RigidbodyComponent* m_pRigidbodyComponent{};
	dae::TransformComponent* m_pTransformComponent{};
	dae::ColliderComponent* m_pColliderComponent{};

	dae::TransformComponent* m_pPlayerTransform{nullptr};
	std::unique_ptr<Subject> m_pSubject{};

	glm::vec3 m_PreviousPosition{};
	glm::vec2 m_CurrentTarget{};

	EnemyStateMachine* m_pEnemyStateMachine;
};

class IEnemyState
{
public:
	IEnemyState(EnemyComponent* pEnemyComponent);
	virtual ~IEnemyState() = default;

	virtual void OnEnter() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void OnExit() = 0;
protected:
	EnemyComponent* m_pEnemyComponent;
};

class LeftState : public IEnemyState
{
public:
	LeftState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform);
	~LeftState() = default;

	void OnEnter() override;
	void Update() override;
	void Render() override;
	void OnExit() override;

private:
	float m_Speed{ 32.f };

	dae::TransformComponent* m_pTransformComponent;
	dae::RigidbodyComponent* m_pRigidbodyComponent;
	dae::ColliderComponent* m_pColliderComponent;

	dae::TransformComponent* m_pPlayertransform;
};

class UpState : public IEnemyState
{
public:
	UpState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform);

	void OnEnter() override;
	void Update() override;
	void Render() override;
	void OnExit() override;

private:

	float m_Speed{ 32.f };

	dae::TransformComponent* m_pTransformComponent;
	dae::RigidbodyComponent* m_pRigidbodyComponent;
	dae::ColliderComponent* m_pColliderComponent;

	dae::TransformComponent* m_pPlayertransform;

	std::shared_ptr<b2World> m_pWorld;
};

class DownState : public IEnemyState
{
public:
	DownState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform);

	void OnEnter() override;
	void Update() override;
	void Render() override;
	void OnExit() override;

private:
	float m_Speed{ 32.f };

	dae::TransformComponent* m_pTransformComponent;
	dae::RigidbodyComponent* m_pRigidbodyComponent;
	dae::ColliderComponent* m_pColliderComponent;

	dae::TransformComponent* m_pPlayertransform;
};

class RightState : public IEnemyState
{
public:
	RightState(EnemyComponent* pEnemyComponent, dae::TransformComponent* pPlayerTransform);

	void OnEnter() override;
	void Update() override;
	void Render() override;
	void OnExit() override;

private:
	float m_Speed{ 32.f };

	dae::TransformComponent* m_pTransformComponent;
	dae::RigidbodyComponent* m_pRigidbodyComponent;
	dae::ColliderComponent* m_pColliderComponent;

	dae::TransformComponent* m_pPlayertransform;
};


// From the GPP course
class EnemyStateMachine
{
public:
	EnemyStateMachine(EnemyComponent* pEnemyComponent, IEnemyState* pEnemyState);
	~EnemyStateMachine();

	EnemyStateMachine(const EnemyStateMachine& other) = delete;
	EnemyStateMachine(EnemyStateMachine&& other) = delete;
	EnemyStateMachine& operator=(const EnemyStateMachine& other) = delete;
	EnemyStateMachine& operator=(EnemyStateMachine&& other) = delete;

	void SwitchGameState(IEnemyState* pGameState);
	void AddState(IEnemyState* pState);

	void AddTransition(IEnemyState* pFromState, IEnemyState* pToState, std::function<bool()> condition);

	void UpdateTransitions();
	void Update();
	void Render();
	
private:
	typedef std::pair<IEnemyState*, std::function<bool()>> TransitionPair;

	std::map < IEnemyState*, std::vector < TransitionPair>> m_pTransitions;
	std::vector<IEnemyState*> m_pStates;

	EnemyComponent* m_pEnemyComponent;
	IEnemyState* m_pCurrentState;
};

