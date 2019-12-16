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
    - ファイルをウォッチしてくれる機能

*/

class ofxWatchShader : public ofShader {

    void update(ofEventArgs &args);
    void watchShader(GLenum type, const std::filesystem::path &filename);
    bool reloadFile(GLenum type);

    std::map<GLenum, bool> watch_flags;
    std::map<GLenum, std::filesystem::path> file_paths{
        std::make_pair(GL_VERTEX_SHADER, std::filesystem::path("")),
        std::make_pair(GL_FRAGMENT_SHADER, std::filesystem::path("")),
        std::make_pair(GL_GEOMETRY_SHADER_EXT, std::filesystem::path("")),
        std::make_pair(GL_COMPUTE_SHADER, std::filesystem::path(""))
    };
    std::map<GLenum, std::time_t> last_loaded_times;
    bool watch_enable = false;

public:
    void watchFile(GLenum type, const std::string path);

    const std::time_t last_write_time(const std::filesystem::path& filename) {
        auto absolute = boost::filesystem::absolute(filename);
        struct stat stat_buf;
        stat(absolute.c_str(), &stat_buf);

        return stat_buf.st_mtime;
    }

    bool load(const char * vertName, const char * fragName);
    bool load(const std::filesystem::path& shaderName, bool watch);
    bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, bool vertWatch = false, bool fragWatch = false);
	bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName="", bool vertWatch = false, bool fragWatch = false, bool geomWatch = false);
    
    bool setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath = "");
    bool setupShaderFromFile(GLenum type, const std::filesystem::path& filename, bool watch);
};

