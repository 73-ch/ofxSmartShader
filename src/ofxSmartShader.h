#pragma once

#include "ofShader.h"
#include "ofConstants.h"
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "glm/fwd.hpp"
#include <unordered_map>
#include <map>
#include <boost/filesystem.hpp>
#include <sys/stat.h>
/*
memo:
    - リロードした時にエラーが出たら元のソースを利用する機能
    - ファイルをウォッチしてくれる機能 ok
    typeを一つの構造体にしたほうが良さそう
*/

class ofxSmartShader : public ofShader {

    void update(ofEventArgs &args);
    void watchShader(GLenum type, const std::filesystem::path &filename);
    bool reloadFile(GLenum type);

    std::map<GLenum, bool> watch_flags;
    std::map<GLenum, bool> file_updated;
    std::map<GLenum, std::filesystem::path> file_paths{
        std::make_pair(GL_VERTEX_SHADER, std::filesystem::path("")),
        std::make_pair(GL_FRAGMENT_SHADER, std::filesystem::path("")),
#ifndef TARGET_OPENGLES
        std::make_pair(GL_GEOMETRY_SHADER_EXT, std::filesystem::path("")),
#endif
    };
    std::map<GLenum, std::time_t> last_loaded_times;
    std::map<GLenum, std::string> current_sources{
        std::make_pair(GL_VERTEX_SHADER, ""),
        std::make_pair(GL_FRAGMENT_SHADER, ""),
        std::make_pair(GL_GEOMETRY_SHADER_EXT, "")
    };
    bool watch_enable = false;

    std::string nameForType(GLenum type){
	    switch(type) {
            case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
#ifndef TARGET_OPENGLES
		    case GL_GEOMETRY_SHADER_EXT: return "GL_GEOMETRY_SHADER_EXT";
#ifdef glDispatchCompute
		    case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
#endif
#endif
		    default: return "UNKNOWN SHADER TYPE";
	    }
    }

public:
    void watchFile(GLenum type, const std::string path);

    const std::time_t last_write_time(const std::filesystem::path& filename) {
        auto absolute = boost::filesystem::absolute(filename);
        struct stat stat_buf;
        stat(absolute.c_str(), &stat_buf);

        return stat_buf.st_mtime;
    }

    bool load(const char * vertName, const char * fragName);
    bool load(const std::string vertName, std::string fragName);
    bool load(const std::filesystem::path& shaderName, bool watch);
    bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, bool vertWatch = false, bool fragWatch = false);
	bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName="", bool vertWatch = false, bool fragWatch = false, bool geomWatch = false);
    
    bool setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath = "");
    bool setupShaderFromFile(GLenum type, const std::filesystem::path& filename, bool watch);
};

