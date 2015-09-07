#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    luts[0].load("default.png");
    luts[1].load("1977.png");
    luts[2].load("amaro.png");
    luts[3].load("gotham.png");
    luts[4].load("lo-fi.png");
    luts[5].load("nashville.png");
    luts[6].load("sutro.png");
    luts[7].load("willow.png");
    luts[8].load("X-ProII.png");
    
    lutNames[0] = "default";
    lutNames[1] = "1977";
    lutNames[2] = "amaro";
    lutNames[3] = "gotham";
    lutNames[4] = "lo-fi";
    lutNames[5] = "nashville";
    lutNames[6] = "sutro";
    lutNames[7] = "willow";
    lutNames[8] = "X-Pro II";
    
    description = "space key : switch thumbnail / full-size view\n";
    description += "left key : previous filter\n";
    description += "right key : next filter\n";
    
    video.initGrabber(1920, 1080);
    
    isThumbnailView = true;
    lutIndex = 0;
    
    thumbnailWidth = 1920 / 3;
    thumbnailHeight = 1080 / 3;
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(isThumbnailView){
        for(int i=0;i<9;i++){
            luts[i].begin();
            video.draw(thumbnailWidth * fmodf(i, 3), thumbnailHeight * floor(i / 3), thumbnailWidth, thumbnailHeight);
            luts[i].end();
            ofDrawBitmapStringHighlight(lutNames[i], thumbnailWidth * fmodf(i, 3) + 4, thumbnailHeight * floor(i / 3) + 14);
        }
    }else{
        luts[lutIndex].begin();
        video.draw(0, 0, 1920, 1080);
        luts[lutIndex].end();
        ofDrawBitmapStringHighlight(lutNames[lutIndex], 4, 14);
    }

    ofDrawBitmapStringHighlight("FPS : " + ofToString(ofGetFrameRate()) + "\n" + description, 4, ofGetHeight() - 48);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case ' ':
            isThumbnailView = !isThumbnailView;
            break;
            
        case OF_KEY_RIGHT:
            if(!isThumbnailView){
                lutIndex++;
                lutIndex %= 9;
            }
            break;
            
        case OF_KEY_LEFT:
            if(!isThumbnailView){
                lutIndex--;
                if(lutIndex < 0){
                    lutIndex = 8;
                }
            }
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
