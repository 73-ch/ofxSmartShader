#include "ofMain.h"

#include "ofxSmartShader.h"

class ofApp : public ofBaseApp {

    ofPlanePrimitive plane;
    ofxSmartShader shader;
    // it can be replaced for: ofShader shader;

public:
    void setup() {
        ofDisableArbTex();
        ofSetBackgroundColor(0);
        // shader
        shader.load("default.vert", "uv.frag");

        // plane
        plane.set(2, 2, 2, 2);
    }

    void update() {

    }

    void draw() {
        shader.begin();
        plane.draw();
        shader.end();
    }
};

//========================================================================
int main() {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);

    const auto window = ofCreateWindow(settings);

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
