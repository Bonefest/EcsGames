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

        int untransparentCounter = 0;

        //Normal
        if(abs(dx) > abs(dy)) {
            float k = float(dy)/dx;
            int sign = (targetX > fromX) ? 1 : -1;
            for(int x = fromX; x != targetX; x += sign) {
                int y = std::round(k*(x - fromX)) + fromY;

                if(!data.objects[y][x].empty()) {
                    entt::entity object = data.objects[y][x][0];
                    Cell& cellComponent = registry.get<Cell>(object);
                    if(!cellComponent.transparent) untransparentCounter++;
                }

                if(untransparentCounter > 1) return false;
            }
        } else {
            float k = float(dx)/dy;
            int sign = (targetY > fromY) ? 1 : -1;
            for(int y = fromY; y != targetY; y += sign) {
                int x = std::round(k*(y - fromY)) + fromX;

                if(!data.objects[y][x].empty()) {

                    entt::entity object = data.objects[y][x][0];
                    Cell& cellComponent = registry.get<Cell>(object);
                    if(!cellComponent.transparent) untransparentCounter++;
                }

                if(untransparentCounter > 1) return false;
            }
        }

        if(untransparentCounter > 0) {
            if(data.objects[fromY][fromX].empty()) return false;

            entt::entity targetEntity = data.objects[fromY][fromX][0];
            Cell& cellComponent = registry.get<Cell>(targetEntity);

            if(cellComponent.transparent) return false;
        }

        return true;
}


std::string cutMessage2(const std::string& message, float fontSize, float viewWidth) {
    std::string result = "";

    int splitNumber = std::ceil(message.size() * (fontSize + 1.0f) / viewWidth);
    int maxLineChars = viewWidth / (fontSize + 1.0f);

    int graphCharsCounter = 0;
    //cocos2d::log("%d", maxLineChars);
    for(size_t i = 0;i < message.size(); ++i) {
        if(message[i] != '\n') graphCharsCounter++;
        else graphCharsCounter = 0;

        result += message[i];
        if(graphCharsCounter >= maxLineChars) {
            result += "\n";
            graphCharsCounter = 0;
        }
    }
    return result;
}

#include <sstream>

std::string cutMessage(const std::string& message, float fontSize, float viewWidth) {
    std::string result = "";

    int maxLineChars = viewWidth / (fontSize + 1.0f);

    std::stringstream sstream(message);
    std::string line = "";

    int scannedLines = 0;
    int scannedLineChars = 0;
    while(std::getline(sstream, line)) {
        if(scannedLines > 0) result += "\n";

        std::size_t spaceDelimPos = 0;
        std::size_t previousDelimPos = 0;
        while( spaceDelimPos != line.size() && (spaceDelimPos = line.find(" ", previousDelimPos))) {
            if(spaceDelimPos == std::string::npos) spaceDelimPos = line.size();

            std::string word = line.substr(previousDelimPos, spaceDelimPos - previousDelimPos);
            if(scannedLineChars + word.size() >= maxLineChars) {
                result += "\n";
                while(word.size() > maxLineChars) {
                    result += word.substr(0, maxLineChars);
                    word = word.substr(maxLineChars);
                    result += "\n";
                }

                scannedLineChars = word.size() + 1;
            } else {

                scannedLineChars += word.size() + 1;
            }

            result += word + " ";

            previousDelimPos = spaceDelimPos + 1;
        }

        scannedLines++;
        scannedLineChars = 0;
    }

    return result;
}

bool isValidPosition(uint16_t x, uint16_t y) {
    return (x >= 0 && y >=0 && x < Constants::MAP_WIDTH && y < Constants::MAP_HEIGHT);
}


bool isDirectionKeyType(KeyType type) {
    return type >= MOVE_TOP_LEFT && type <= MOVE_LEFT;
}

Vec2 directionKeyTypeToVector(KeyType type) {
    if(isDirectionKeyType(type)) {
        switch(type) {
        case MOVE_TOP_LEFT: return Vec2(-1, 1);
        case MOVE_TOP: return Vec2(0, 1);
        case MOVE_TOP_RIGHT: return Vec2(1, 1);
        case MOVE_RIGHT: return Vec2(1, 0);
        case MOVE_BOTTOM_RIGHT: return Vec2(1, -1);
        case MOVE_BOTTOM: return Vec2(0, -1);
        case MOVE_BOTTOM_LEFT: return Vec2(-1, -1);
        case MOVE_LEFT: return Vec2(-1, 0);
        }
    }

    return Vec2::ZERO;
}

std::string directionVectorToString(Vec2 direction) {
    if(direction.x == -1 && direction.y ==  1)  return "north-west";
    if(direction.x ==  0 && direction.y ==  1)  return "north";
    if(direction.x ==  1 && direction.y ==  1)  return "north-east";
    if(direction.x ==  1 && direction.y ==  0)  return "east";
    if(direction.x ==  1 && direction.y == -1)  return "south-east";
    if(direction.x ==  0 && direction.y == -1)  return "south";
    if(direction.x == -1 && direction.y == -1)  return "south-west";
    if(direction.x == -1 && direction.y ==  0)  return "west";

    return "";
}

Color3B integerToColor(uint32_t integer) {
    uint8_t r = integer & 0xFF000000;
    uint8_t g = integer & 0x00FF0000;
    uint8_t b = integer & 0x0000FF00;

    return Color3B(r, g, b);
}

