#include "ofxSmartShader.h"
#include "ofGLProgrammableRenderer.h"

void ofxSmartShader::update(ofEventArgs &args) {
    auto updated = false;

    for (auto fp : file_paths) {
        if (watch_flags[fp.first] && last_loaded_times[fp.first] != last_write_time(fp.second)) {
            file_updated[fp.first] = true;
            updated = true;
        }
    }

    if (updated) {

        for (auto fp : file_paths) {
            if (file_updated[fp.first]) {
                reloadFile(fp.first);
            } else {
                if (current_sources[fp.first].empty() == false) {
                    setupShaderFromSource(fp.first, current_sources[fp.first]);
                }
            }

            file_updated[fp.first] = false;
        }

        if(ofIsGLProgrammableRenderer()){
            ofShader::bindDefaults();
        }

        ofShader::linkProgram();
        
    }
}

bool ofxSmartShader::load(const char * vertName, const char * fragName) {
    return load(vertName, fragName, false, false);
}

bool ofxSmartShader::load(const std::filesystem::path& shaderName, bool watch) {
    return load(shaderName.string() + ".vert", shaderName.string() + ".frag", "", watch, watch, false);
}

bool ofxSmartShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, bool vertWatch, bool fragWatch) {
    return load(vertName, fragName, "", vertWatch, fragWatch, false);
}

bool ofxSmartShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName, bool vertWatch, bool fragWatch, bool geomWatch) {
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

bool ofxSmartShader::setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath) {
    auto result = ofShader::setupShaderFromSource(type, source, sourceDirectoryPath);

    if (result == false) {
        if (current_sources[type].empty() == false) {
            ofShader::setupShaderFromSource(type, current_sources[type]);
        }
    } else {
        current_sources[type] = source;
    }
    
    return result;
}

bool ofxSmartShader::setupShaderFromFile(GLenum type, const std::filesystem::path& filename, bool watch) {
    watch_flags[type] = watch;
    file_paths[type] = ofToDataPath(filename);

    if (watch_flags[type] && !watch_enable) {
        ofAddListener(ofEvents().update, this, &ofxSmartShader::update, OF_EVENT_ORDER_BEFORE_APP);
    }

    return reloadFile(type);
}

bool ofxSmartShader::reloadFile(GLenum type) {
    bool result;

    ofBuffer buffer = ofBufferFromFile(file_paths[type]);
	// we need to make absolutely sure to have an absolute path here, so that any #includes
	// within the shader files have a root directory to traverse from.
	std::string absoluteFilePath = ofFilePath::getAbsolutePath(file_paths[type], true);
	std::string sourceDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath,false);
	if(buffer.size()) {
		result = setupShaderFromSource(type, buffer.getText(), sourceDirectoryPath);
	} else {
		ofLogError("ofShader") << "setupShaderFromFile(): couldn't load " << nameForType(type) << " shader " << " from \"" << absoluteFilePath << "\"";
		result = false;
	}

    last_loaded_times[type] = last_write_time(file_paths[type]);

    if (result) {
        ofLogNotice() << "ofxSmartShader load successs: " << file_paths[type].filename(); // ここは利用者に任せる形でもいいかも
    } else {
        if (current_sources[type].empty() == false) {
            ofShader::setupShaderFromSource(type, current_sources[type]);
        }
    }
    
    return result;
}
