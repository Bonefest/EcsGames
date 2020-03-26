#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class EntityAnimation {
public:
    EntityAnimation(): _currentFrameIndex(0) { }
    EntityAnimation(const vector<string>& frames, float frameDuration): _frames(frames),
                                                                        _currentFrameIndex(0),
                                                                        _frameDuration(frameDuration),
                                                                        _elapsedFrameTime(0.0f) { }

    void addFrame(const string& frameName) {
        _frames.push_back(frameName);
    }

    void update(float delta) {
        if(_frameDuration <= 0.0f) return;

        _elapsedFrameTime += delta;
        while(_elapsedFrameTime > _frameDuration) {
            nextFrame();
            _elapsedFrameTime -= _frameDuration;
        }
    }

    void nextFrame() {
        _currentFrameIndex++;
        if(_frames.size() >= _currentFrameIndex) _currentFrameIndex = 0;
    }

    void previousFrame() {
        if(_currentFrameIndex == 0) {
            _currentFrameIndex = _frames.size() - 1;
        } else _currentFrameIndex--;
    }

    const string& getCurrentFrame() const {
        return _frames[_currentFrameIndex];
    }

private:
    vector<string> _frames;
    size_t _currentFrameIndex;

    float _frameDuration;
    float _elapsedFrameTime;
};

class AnimationsContainer {
public:
    AnimationsContainer(): _currentState(0) { }

    void setAnimation(uint32_t tag, EntityAnimation animation) {
        _animations[tag] = animation;
    }

    void setCurrentState(uint32_t tag) {
        _currentState = tag;
    }

    void update(float delta) {
        auto animationIter = _animations.find(_currentState);
        if(animationIter != _animations.end()) {
            animationIter->second.update(delta);
        }
    }


    string getCurrentFrame() const {
        auto animationIter = _animations.find(_currentState);
        if(animationIter != _animations.end())
            return animationIter->second.getCurrentFrame();

        return "";
    }
private:
    map<uint32_t, EntityAnimation> _animations;
    uint32_t _currentState;
};


#endif // ANIMATION_H_INCLUDED
