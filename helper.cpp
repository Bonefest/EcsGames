#include "helper.h"

void prepareQuad(SpriteFrame* frame, Color4B color, Rect contentSize, cocos2d::V3F_C4B_T2F_Quad* outQuad) {
    Texture2D* tex = frame->getTexture();
    if(tex == nullptr) return;

    memset(outQuad, 0, sizeof(cocos2d::V3F_C4B_T2F_Quad));

    const auto rectInPixels = CC_RECT_POINTS_TO_PIXELS(frame->getRect());

    const float atlasWidth = (float)tex->getPixelsWide();
    const float atlasHeight = (float)tex->getPixelsHigh();

    float rw = rectInPixels.size.width;
    float rh = rectInPixels.size.height;

    float left    = rectInPixels.origin.x / atlasWidth;
    float right   = (rectInPixels.origin.x + rw) / atlasWidth;
    float top     = rectInPixels.origin.y / atlasHeight;
    float bottom  = (rectInPixels.origin.y + rh) / atlasHeight;


    outQuad->bl.texCoords.u = left;
    outQuad->bl.texCoords.v = bottom;
    outQuad->br.texCoords.u = right;
    outQuad->br.texCoords.v = bottom;
    outQuad->tl.texCoords.u = left;
    outQuad->tl.texCoords.v = top;
    outQuad->tr.texCoords.u = right;
    outQuad->tr.texCoords.v = top;

    ////////////////////////// VERTEX COORDS

    // Atlas: Vertex
    const float x1 = 0.0f + contentSize.origin.x;
    const float y1 = 0.0f + contentSize.origin.y;
    const float x2 = x1 + contentSize.size.width;
    const float y2 = y1 + contentSize.size.height;


    // Don't update Z.
    outQuad->bl.vertices.set(x1, y1, 0.0f);
    outQuad->br.vertices.set(x2, y1, 0.0f);
    outQuad->tl.vertices.set(x1, y2, 0.0f);
    outQuad->tr.vertices.set(x2, y2, 0.0f);

    outQuad->bl.colors = color;
    outQuad->br.colors = color;
    outQuad->tl.colors = color;
    outQuad->tr.colors = color;

    cocos2d::PolygonInfo* newPolygonInfo = new cocos2d::PolygonInfo();
    newPolygonInfo->setQuad(outQuad);
    newPolygonInfo->setRect(contentSize);

    frame->setPolygonInfo(*newPolygonInfo);


}

bool isVisible(entt::registry& registry,
               Coordinate fromX, Coordinate fromY,
               Coordinate targetX, Coordinate targetY,
               uint16_t visibleRadius,
               WorldData& data) {

        int dx = targetX - fromX;
        int dy = targetY - fromY;
        if( (dx*dx + dy*dy) > visibleRadius * visibleRadius) return false;

        if(dx == 0 && dx == dy) return true;
        //Normal
        if(abs(dx) > abs(dy)) {
            float k = float(dy)/dx;
            int sign = (targetX > fromX) ? 1 : -1;
            for(int x = fromX; x != targetX; x += sign) {
                int y = std::round(k*(x - fromX)) + fromY;

                entt::entity object = data.objects[y][x][0];
                Cell& cellComponent = registry.get<Cell>(object);
                if(!cellComponent.transparent) return false;
            }
        } else {
            float k = float(dx)/dy;
            int sign = (targetY > fromY) ? 1 : -1;
            for(int y = fromY; y != targetY; y += sign) {
                int x = std::round(k*(y - fromY)) + fromX;

                entt::entity object = data.objects[y][x][0];
                Cell& cellComponent = registry.get<Cell>(object);
                if(!cellComponent.transparent) return false;
            }
        }


        return true;
}
