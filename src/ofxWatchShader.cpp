#include "ofxWatchShader.h"
#include "ofGLProgrammableRenderer.h"
#include <boost/filesystem.hpp>

void ofxWatchShader::reloadShader() {
//   const string b_vs = getShaderSource(GL_VERTEX_SHADER);
//   const string b_fs = getShaderSource(GL_FRAGMENT_SHADER);

//   bool result = setupShaderFromSource(GL_VERTEX_SHADER, vertex_text);

//   if (!result) {
//     setupShaderFromSource(GL_VERTEX_SHADER, b_vs);
//     vertex_text = b_vs;
//   }

//   result = setupShaderFromSource(GL_FRAGMENT_SHADER, fragment_text);

//   if (!result) {
//     setupShaderFromSource(GL_FRAGMENT_SHADER, b_fs);
//     fragment_text = b_fs;
//   }

//   bindDefaults();
//   linkProgram();
}

void ofxWatchShader::updateWatcher() {

}

void ofxWatchShader::update() {

}

void ofxWatchShader::watchShader(GLenum type, const std::filesystem::path &filename) {

}

bool ofxWatchShader::load(const char * vertName, const char * fragName) {
    ofLogNotice() << "vert + frag";
    return load(vertName, fragName, false, false);
}

bool ofxWatchShader::load(const std::filesystem::path& shaderName, bool watch) {
    ofLogNotice() << "filename";
    return load(shaderName.string() + ".vert", shaderName.string() + ".frag", "", watch, watch, false);
}

bool ofxWatchShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, bool vertWatch, bool fragWatch) {
    ofLogNotice() << "vert + frag + bool + bool";
    return load(vertName, fragName, "", vertWatch, fragWatch, false);
}

bool ofxWatchShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName, bool vertWatch, bool fragWatch, bool geomWatch) {
    ofLogNotice() << "default";
    if(vertName.empty() == false) setupShaderFromFile(GL_VERTEX_SHADER, vertName, vertWatch);
	if(fragName.empty() == false) setupShaderFromFile(GL_FRAGMENT_SHADER, fragName, vertFrag);
#ifndef TARGET_OPENGLES
	if(geomName.empty() == false) setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, geomName, geomWatch);
#endif

    if(ofIsGLProgrammableRenderer()){
		bindDefaults();
	}
	return linkProgram();
}

bool ofxWatchShader::setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath) {
    return true;
}

bool ofxWatchShader::setupShaderFromFile(GLenum type, const std::filesystem::path &filename, bool watch) {
    // watch_flags[type] = watch;

    // if (watch_flags[type]) {

    // }
    return ofShader::setupShaderFromFile(type, filename);
    
}