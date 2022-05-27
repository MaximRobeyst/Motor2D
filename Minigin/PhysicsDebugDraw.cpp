#include "MiniginPCH.h"
#include "PhysicsDebugDraw.h"

#include "Renderer.h"

PhysicsDebugDraw::PhysicsDebugDraw()
	: m_pRenderer{&dae::Renderer::GetInstance()}
{
}

void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	m_pRenderer->RenderPolygon(vertices, vertexCount, SDL_Color{ static_cast<Uint8>(color.r * 255), static_cast<uint8>(color.g * 255), static_cast<uint8>(color.b * 255) ,static_cast<uint8>(color.a * 255) });
}

void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	m_pRenderer->RenderPolygon(vertices, vertexCount, SDL_Color{ static_cast<Uint8>(color.r * 255), static_cast<uint8>(color.g * 255), static_cast<uint8>(color.b * 255) ,static_cast<uint8>(color.a * 255) });
}

void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	m_pRenderer->RenderCircle(center, radius, SDL_Color{ static_cast<Uint8>(color.r * 255), static_cast<uint8>(color.g * 255), static_cast<uint8>(color.b * 255) ,static_cast<uint8>(color.a * 255) });
}

void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& /*axis*/, const b2Color& color)
{
	m_pRenderer->RenderCircle(center, radius, SDL_Color{ static_cast<Uint8>(color.r * 255), static_cast<uint8>(color.g * 255), static_cast<uint8>(color.b * 255) ,static_cast<uint8>(color.a * 255) });
}

void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_pRenderer->RenderLine(p1, p2, SDL_Color{ static_cast<Uint8>(color.r * 255), static_cast<uint8>(color.g * 255), static_cast<uint8>(color.b * 255) ,static_cast<uint8>(color.a * 255) });
}

void PhysicsDebugDraw::DrawTransform(const b2Transform& /*xf*/)
{
}

void PhysicsDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	m_pRenderer->RenderCircle(p, size, SDL_Color{ static_cast<Uint8>(color.r * 255), static_cast<uint8>(color.g * 255), static_cast<uint8>(color.b * 255) ,static_cast<uint8>(color.a * 255) });
}