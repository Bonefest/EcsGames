#ifndef DRAWTEXTURENODE_H_INCLUDED
#define DRAWTEXTURENODE_H_INCLUDED

#include "cocos2d.h"
#include "helper.h"
#include <list>
USING_NS_CC;
using std::list;

struct DrawElement {
    int z;
    cocos2d::Texture2D* texture;
    cocos2d::TrianglesCommand* command;
    cocos2d::V3F_C4B_T2F_Quad* quad;
    cocos2d::PolygonInfo* info;
};

class DrawTextureNode: public Node {
public:
    CREATE_FUNC(DrawTextureNode);

    static DrawTextureNode* createDrawer() {
        return DrawTextureNode::create();
    }

    void drawSpriteFrame(const string& frameName, Vec2 position, int z, Size size, Color4B color) {
        cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

        cocos2d::PolygonInfo* info = new cocos2d::PolygonInfo();
        cocos2d::V3F_C4B_T2F_Quad* quad = new cocos2d::V3F_C4B_T2F_Quad;
        prepareQuad(frame, color, Rect(position.x, position.y, size.width, size.height), quad);

        info->setQuad(quad);

        TrianglesCommand* command = new TrianglesCommand;

        _elements.push_back({z, frame->getTexture(), command, quad, info});

    }

    virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) {
        for(DrawElement& element : _elements) {
            element.command->init(0,
                      element.texture,
                      cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP, element.texture),
                      cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED,
                      element.info->triangles,
                      transform,
                      flags);

            renderer->addCommand(element.command);


        }
    }

    void clear() {
        for(DrawElement& element : _elements) {
            delete element.command;
            delete element.info;
            delete element.quad;
        }

        _elements.clear();
    }

private:
    list<DrawElement> _elements;

};

#endif // DRAWTEXTURENODE_H_INCLUDED
