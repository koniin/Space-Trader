#include "GameObject.h"

GameObject::GameObject(SDL_Texture* tex, SDL_Point* world)
	: texture(tex),
	worldBounds(world) {
	angle = 90.0f;
	angleAdjustment = -90;
}


GameObject::~GameObject() {
}

void GameObject::Update(float dt) {}

SDL_Rect* GameObject::GetPositionRectangle() {
	return &sourceRect;
}

void GameObject::SetSpeed(int newSpeed) {
	speed = newSpeed;
	precalc_speed = speed == 0 ? 0.0f : (speed / 100.f);
}