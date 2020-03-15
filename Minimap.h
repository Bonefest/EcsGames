#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include "cocos2d.h"

USING_NS_CC;

class Minimap {
public:

    Minimap(Node* scene,
            Size minimapSize,
            Color4F backgroundColor,
            Color4F borderColor, float borderSize): _size(minimapSize),
                                                    _backgroundColor(backgroundColor),
                                                    _borderColor(borderColor),
                                                    _borderSize(borderSize),
                                                    _position(Vec2::ZERO)
    {
        _minimapRenderer = cocos2d::DrawNode::create();
        scene->addChild(_minimapRenderer);
    }

    void setPosition(Vec2 position) {
        _position = position;
    }

    void setBorderRect(Rect borderRect) {
        _borderRect = borderRect;
    }

    cocos2d::DrawNode* getRenderer() { return _minimapRenderer; }

    void draw(entt::registry& registry, float delta) {

        _minimapRenderer->clear();

        _minimapRenderer->drawSolidRect(_position, _position + Vec2(_size), _backgroundColor);

        auto view = registry.view<Transform, MinimapTarget>();
        view.each([&](Transform& transform, MinimapTarget& minimapTarget) {
            if(_borderRect.containsPoint(transform.position)) {
                Vec2 targetPosition = transform.position - _borderRect.origin;
                targetPosition.x = (targetPosition.x / _borderRect.size.width) * _size.width;
                targetPosition.y = (targetPosition.y / _borderRect.size.height) * _size.height;

                targetPosition = targetPosition + _position;

                switch(minimapTarget.shape) {
                case Shape::SQUARE:
                    _minimapRenderer->drawRect(targetPosition - Vec2(1, 1), targetPosition + Vec2(1, 1), minimapTarget.color);
                    break;
                case Shape::CIRCLE:
                    _minimapRenderer->drawCircle(targetPosition, 1.0f, 0.0f, 9, false, minimapTarget.color);
                    break;
                case Shape::TRIANGLE:
                    _minimapRenderer->drawTriangle(targetPosition + Vec2(0, 2), targetPosition + Vec2(-1.414, -1.414), targetPosition + Vec2(1.414, -1.414), minimapTarget.color);
                    break;

                }
            }
        });
    }


private:
    cocos2d::DrawNode* _minimapRenderer;

    cocos2d::Color4F _backgroundColor;
    cocos2d::Color4F _borderColor;

    float _borderSize;

    Vec2 _position;

    Rect _borderRect;

    Size _size;
};

#endif // MINIMAP_H_INCLUDED
