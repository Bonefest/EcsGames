#ifndef AUTOSIZECONTAINER_H_INCLUDED
#define AUTOSIZECONTAINER_H_INCLUDED

#include "cocos2d.h"

USING_NS_CC;

class AutosizeContainer: public cocos2d::Node {
public:
    CREATE_FUNC(AutosizeContainer);
    static AutosizeContainer* createContainer() {
        return AutosizeContainer::create();
    }

    void addChild(Node* node) {
        Node::addChild(node);

        auto children = getChildren();

        Vec2 containerPos = getPosition();
        float minX = containerPos.x, minY = containerPos.y, maxX = containerPos.x, maxY = containerPos.y;

        for(auto child : children) {
            Vec2 pos = child->getPosition();
            Vec2 anchor = child->getAnchorPoint();
            Size size = child->getContentSize();

            minX = std::min(minX, pos.x - size.width * anchor.x);
            minY = std::min(minY, pos.y - size.height * anchor.y);

            maxX = std::max(maxX, pos.x + size.width * (1.0f - anchor.x));
            maxY = std::max(maxY, pos.y + size.height * (1.0f - anchor.y));
        }
        log("%f %f %f %f", minX, minY, maxX, maxY);

        setContentSize(Size(maxX - minX, maxY - minY));
    }
};

#endif // AUTOSIZECONTAINER_H_INCLUDED
