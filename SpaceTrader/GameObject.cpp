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

const SDL_Rect* GameObject::GetPositionRectangle() {
	return &sourceRect;
}

void GameObject::Destroy() { 
	destroyed = true;
}

bool GameObject::IsDestroyed() {
	return destroyed;
}

GameObject::Type GameObject::GetType() const {
	return gameObjectType;
}