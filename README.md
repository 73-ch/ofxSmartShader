## About

This addon is the wrapper of ofShader.

## Feature
- Compatible with ofShader
   - use like a ofShader 
- File Watching
   - When shader file is changed, automatically reload shader file
- Safety Loading
   - When shader file contains some error, use the source that was loaded before it was changed

## How to use
```c++

// declation
ofxSmartShader shader; // simply replace "ofShader" with "ofxSmartShader"


// loading
shader.load(VERTEX_PATH, FRAGMENT_PATH, VERTEX_WATCH_FLAG, FRAGMENT_WATCH_FLAG);

// if you don't want to watch shader, you can do it by ignoring flag arguments.
shader.load(VERTEX_PATH, FRAGMENT_PATH);

// ...
shader.begin();

shader.end();

```



## License

*This software is released under the MIT License, see LICENSE.txt.*

