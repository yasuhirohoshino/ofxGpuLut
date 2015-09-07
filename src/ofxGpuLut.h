#pragma once
#define STRINGIFY(e) #e

class ofxGpuLut{
    
private:
    ofShader lutShader;
    ofTexture lut;
    string fragmentShader;
    ofImage lutImage;
public:
    ofxGpuLut(){}
    ~ofxGpuLut(){}
    
    void load(ofTexture lutTexture){
        fragmentShader = "#version 120\n#extension GL_ARB_texture_rectangle : enable\n";
        fragmentShader += STRINGIFY(
                                    uniform sampler2DRect tex;
                                    uniform sampler2DRect lut;
                                    
                                    float size = 64.0;
                                    
                                    void main( void )
                                        {
                                            vec3 originalColor = floor(texture2DRect(tex, gl_TexCoord[0].st).rgb * vec3(size - 1.0));
                                            vec2 blueIndex = vec2(mod(originalColor.b, sqrt(size)), floor(originalColor.b / sqrt(size)));
                                            vec2 index = vec2((size * blueIndex.x + originalColor.r) + 0.5, (size * blueIndex.y + originalColor.g) + 0.5);
                                            gl_FragColor = vec4(texture2DRect(lut, index).rgb, 1.0);
                                        }
                                    );
        lutShader.unload();
        lutShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        lutShader.linkProgram();
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
    
    void load(ofImage lutImage){
        load(lutImage.getTextureReference());
    }
    
    void load(string path){
        lutImage.loadImage(path);
        load(lutImage.getTextureReference());
    }
    
    void begin(){
        lutShader.begin();
        lutShader.setUniformTexture("lut", lut, 1);
    }
    
    void end(){
        lutShader.end();
    }
};