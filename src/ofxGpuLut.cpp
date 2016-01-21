#include "ofxGpuLut.h"

ofxGpuLut::ofxGpuLut(){}
ofxGpuLut::~ofxGpuLut(){}

void ofxGpuLut::load(ofTexture lutTexture){
    
    if(ofIsGLProgrammableRenderer()){
        vertexShader = "#version 150\n";
        vertexShader += STRINGIFY(
                                  uniform mat4 projectionMatrix;
                                  uniform mat4 modelViewMatrix;
                                  uniform mat4 modelViewProjectionMatrix;
                                  
                                  in vec4  position;
                                  in vec2  texcoord;
                                  
                                  out vec2 texCoordVarying;
                                  
                                  void main()
                                  {
                                      texCoordVarying = texcoord;
                                      gl_Position = modelViewProjectionMatrix * position;
                                  }
                                  );
        
        fragmentShader = "#version 150\n";
        fragmentShader += STRINGIFY(
                                    uniform sampler2DRect tex;
                                    uniform sampler2DRect lut;
                                    
                                    in vec2 texCoordVarying;
                                    
                                    out vec4 fragColor;
                                    
                                    // Texture coordinates
                                    vec2 texcoord0 = texCoordVarying;
                                    
                                    float size = 64.0;
                                    
                                    void main( void )
                                    {
                                        vec3 rawColor = texture(tex, texcoord0).rgb;
                                        float rawAlpha = texture(tex, texcoord0).a;
                                        
                                        if (rawAlpha <= 0.0) {
                                            fragColor = vec4(rawColor, 0.0);
                                        }
                                        else {
                                            vec3 originalColor = floor(texture(tex, texcoord0).rgb * vec3(size - 1.0));
                                            vec2 blueIndex = vec2(mod(originalColor.b, sqrt(size)), floor(originalColor.b / sqrt(size)));
                                            vec2 index = vec2((size * blueIndex.x + originalColor.r) + 0.5, (size * blueIndex.y + originalColor.g) + 0.5);
                                            fragColor = vec4(texture(lut, index).rgb, rawAlpha);
                                        }
                                    }
                                    );
        
        lutShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        lutShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        lutShader.bindDefaults();
        lutShader.linkProgram();
    }
    else {
        fragmentShader = "#version 120\n#extension GL_ARB_texture_rectangle : enable\n";
        fragmentShader += STRINGIFY(
                                    uniform sampler2DRect tex;
                                    uniform sampler2DRect lut;
                                    
                                    float size = 64.0;
                                    
                                    void main( void )
                                    {
                                        vec3 rawColor = texture2DRect(tex, gl_TexCoord[0].st).rgb;
                                        float rawAlpha = texture2DRect(tex, gl_TexCoord[0].st).a;
                                        
                                        if (rawAlpha <= 0.0) {
                                            gl_FragColor = vec4(rawColor, 0.0);
                                        }
                                        else {
                                            vec3 originalColor = floor(texture2DRect(tex, gl_TexCoord[0].st).rgb * vec3(size - 1.0));
                                            vec2 blueIndex = vec2(mod(originalColor.b, sqrt(size)), floor(originalColor.b / sqrt(size)));
                                            vec2 index = vec2((size * blueIndex.x + originalColor.r) + 0.5, (size * blueIndex.y + originalColor.g) + 0.5);
                                            gl_FragColor = vec4(texture2DRect(lut, index).rgb, rawAlpha);
                                        }
                                    }
                                    );
        
        
        lutShader.unload();
        lutShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        lutShader.linkProgram();
    }
    
    lut.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    lut.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    if(!ofGetUsingArbTex()){
        ofEnableArbTex();
        lut = lutTexture;
        ofDisableArbTex();
    }else{
        lut = lutTexture;
    }
}

void ofxGpuLut::load(ofImage lutImage){
    load(lutImage.getTexture());
}

void ofxGpuLut::load(string path){
    lutImage.load(path);
    load(lutImage.getTexture());
}

void ofxGpuLut::begin(){
    lutShader.begin();
    lutShader.setUniformTexture("lut", lut, 1);
}

void ofxGpuLut::end(){
    lutShader.end();
}