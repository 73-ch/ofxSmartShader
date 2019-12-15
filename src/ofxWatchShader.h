#pragma once

#include "ofShader.h"
#include "ofConstants.h"
#include "ofFileUtils.h"
#include "glm/fwd.hpp"
#include <unordered_map>
#include <map>
/*
memo:
    - リロードした時にエラーが出たら元のソースを利用する機能
    - ファイルをウォッチしてくれる機能

*/

class ofxWatchShader : public ofShader {

    void reloadShader();
    void sourceUpdated();
    void update();
    void updateWatcher();
    void watchShader(GLenum type, const std::filesystem::path &filename);

    std::map<GLenum, bool> watch_flags;
    std::map<GLenum, std::filesystem::path&> file_paths;
    std::map<GLenum, std::time_t> last_loaded_times;

public:
    void watchFile(GLenum type, const std::string path);

    // using ofShader::setupShaderFromSource;
    // using ofShader::setupShaderFromFile;
    // using ofShader::load;

    bool load(const char * vertName, const char * fragName);
    bool load(const std::filesystem::path& shaderName, bool watch);
    bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, bool vertWatch = false, bool fragWatch = false);
	bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName="", bool vertWatch = false, bool fragWatch = false, bool geomWatch = false);
    
    bool setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath = "");
    bool setupShaderFromFile(GLenum type, const std::filesystem::path &filename, bool watch);
};

