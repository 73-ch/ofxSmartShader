#include "ofxWatchShader.h"
#include "ofGLProgrammableRenderer.h"

void ofxWatchShader::update(ofEventArgs &args) {
    for (auto fp : file_paths) {
        if (watch_flags[fp.first] && last_loaded_times[fp.first] != last_write_time(fp.second)) {
            reloadFile(fp.first);
        }
    }
}

bool ofxWatchShader::load(const char * vertName, const char * fragName) {
    return load(vertName, fragName, false, false);
}

bool ofxWatchShader::load(const std::filesystem::path& shaderName, bool watch) {
    return load(shaderName.string() + ".vert", shaderName.string() + ".frag", "", watch, watch, false);
}

bool ofxWatchShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, bool vertWatch, bool fragWatch) {
    return load(vertName, fragName, "", vertWatch, fragWatch, false);
}

bool ofxWatchShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName, bool vertWatch, bool fragWatch, bool geomWatch) {
    if(vertName.empty() == false) setupShaderFromFile(GL_VERTEX_SHADER, vertName, vertWatch);
	if(fragName.empty() == false) setupShaderFromFile(GL_FRAGMENT_SHADER, fragName, fragWatch);
#ifndef TARGET_OPENGLES
	if(geomName.empty() == false) setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, geomName, geomWatch);
#endif

    if(ofIsGLProgrammableRenderer()){
		bindDefaults();
	}
	return linkProgram();
}

bool ofxWatchShader::setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath) {
    std::string before_source = "";

    if (ofShader::getShader(type)) {
        before_source = ofShader::getShaderSource(type);
    }

    auto result = ofShader::setupShaderFromSource(type, source, sourceDirectoryPath);

    if (result == false) {
        if (before_source.empty() == false) {
            ofShader::setupShaderFromSource(type, before_source);
        }
    }
    
    return result;
}

bool ofxWatchShader::setupShaderFromFile(GLenum type, const std::filesystem::path& filename, bool watch) {
    watch_flags[type] = watch;
    file_paths[type] = ofToDataPath(filename);

    if (watch_flags[type] && !watch_enable) {
        ofAddListener(ofEvents().update, this, &ofxWatchShader::update, OF_EVENT_ORDER_BEFORE_APP);
    }

    return reloadFile(type);
}

bool ofxWatchShader::reloadFile(GLenum type) {
    std::string before_source = "";

    if (ofShader::getShader(type)) {
        before_source = ofShader::getShaderSource(type);
    }

    auto result = ofShader::setupShaderFromFile(type, file_paths[type]);
    last_loaded_times[type] = last_write_time(file_paths[type]);

    if (result) {
        ofLogNotice() << "ofxWatchShader load successs: " << file_paths[type].filename(); // ここは利用者に任せる形でもいいかも
    } else {
        if (before_source.empty() == false) {
            ofShader::setupShaderFromSource(type, before_source);
        }
    }
    
    return result;
}
