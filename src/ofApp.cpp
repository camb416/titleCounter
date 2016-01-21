#include "ofApp.h"

#define IGNORECHARS 32


// sort on abc's
//--------------------------------------------------------------
bool ofApp::sortOnABC(const LyricWord &a, const LyricWord &b) {
    return a.word < b.word;
}

// sort on word length
//--------------------------------------------------------------
bool ofApp::sortOnLength(const LyricWord &a, const LyricWord &b) {
    return (int)a.word.size() > (int)b.word.size();
}

// sort on occurrences
//--------------------------------------------------------------
bool ofApp::sortOnOccurrences(const LyricWord &a, const LyricWord &b) {
    return a.occurrences > b.occurrences;
}


// remove runction
//--------------------------------------------------------------
bool ofApp::removeWordIf(LyricWord &wrd) {
    
    bool bRemove = false;
    static string ignoreWords[15] = {"with", "for", "at", "ii","the", "to", "of", "a", "and", "i", "it", "if", "is", "in", "be"};
    
    // if this word empty
    if(wrd.word.empty()) bRemove = true;
    
    // are we a word that we do now want
    for (int j=0; j<15; j++) {
        if(wrd.word == ignoreWords[j]) {
            bRemove = true;
            break;
        }
    }
    
    return bRemove;
}

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackground(255);
    
    ofTrueTypeFont::setGlobalDpi(96);

    // load the font
	font.load("sans-serif", 9, true);
    sortTypeInfo = "no sort";
    words.clear();
    
    // load the txt document into a ofBuffer 
    ofBuffer buffer = ofBufferFromFile("query_result.csv");
    string   content = buffer.getText();
    
    
    // take the content and split it up by spaces
    // we need to also turn new lines into spaces so we can seperate words on new lines as well 
    ofStringReplace(content, "\r", " ");
    ofStringReplace(content, "\n", " ");
    
    // first get rid of rando characters
    char ignoreList[IGNORECHARS] = {'[',']',';','&','+','=',',', '.', '(', ')', '?', '!', '-', ':', '"', '\'','\\' , '/', '\n', '\t','#','1','2','3','4','5','6','7','8','9','0'};
    for(int j=0; j<(IGNORECHARS-1); j++) {
        
        // make string from char
        string removeStr;
        removeStr += ignoreList[j];
        
        // remove and of the chars found
        ofStringReplace(content, removeStr, " ");
    }


    vector <string> splitString = ofSplitString(content, " ", true, true);
    
    // copy over the words to our object
    for (unsigned int i=0; i<splitString.size(); i++) {
        LyricWord wrd;
        wrd.occurrences = 1;
        wrd.word = ofToLower( splitString[i] );
        words.push_back(wrd);
    }
    
    // clean up the words removing any 
    // characters that we do not want
    for (unsigned int i=0; i<words.size(); i++) {
        // run throught this ignore list and replace
        // that char with nothing
            }
    
 
    // count the amount of times that we see a word
    for (unsigned int i=0; i<words.size(); i++) {
        int c = 1;
        for (unsigned int j=0; j<words.size(); j++) {
            if(words[i].word == words[j].word) c ++;
        }
        words[i].occurrences = c;
    }
    
    // remove duplicates of the words
    vector<LyricWord>tempWord;
    for (unsigned int i=0; i<words.size(); i++) {
        bool bAdd = true;
        for(unsigned int j=0; j<tempWord.size(); j++) {
            if(words[i].word == tempWord[j].word) bAdd = false;
        }
        
        if(bAdd) {
            tempWord.push_back(words[i]);
        }
    }
    
    words = tempWord;
    
    // remove word we do not want
    ofRemove(words, ofApp::removeWordIf);

}


//--------------------------------------------------------------
void ofApp::update() {
	
}

//--------------------------------------------------------------
void ofApp::draw() {

    ofBackground(100);
    ofSetColor(255);
    ofDrawRectangle(50,150,1100,300);
    

    ofSetColor(255);
    font.drawString(sortTypeInfo, 50, 50);
    ofSetColor(50);
    ofPushMatrix();
    //ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofTranslate(0,ofGetHeight()/2);
    
    float radius = 350;
    
    
    for(unsigned int i=0; i<words.size()/2; i++) {
        float t = -HALF_PI + ofMap(i, 0, (words.size()/2), 0, TWO_PI);
        // float x = cos( t ) * radius;
        // float y = sin( t ) * radius;
        float x = i*2+50 ;
        float y = 0;
        // float a = ofRadToDeg(atan2(y, x));
        float a = 90;
        
        
        
        ofPushMatrix();
        ofTranslate(x, y);
        ofRotateZ(a);
        
        float barWidth = 1;
        ofSetColor(0);
        
        if(curWord == i){
            float scl = 1;
            glScalef(scl, scl, scl);
            ofSetColor(255);
            font.drawString(words[i].word, 0, 0);
            barWidth = 1;
            ofSetColor(255,0,0);
        }

        
        ofDrawRectangle( 0,0,- words[i].occurrences*4,barWidth);
        
        ofPopMatrix();
        
    }    
    
        ofPopMatrix();
    
    
    // instruction
    //ofSetColor(10);
    //ofDrawBitmapString("\nPress 1 for no sort\nPress 2 for alphabetical\nPress 3 for word length\nPress 4 for word occurrence", 20, 20);
    

  
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
 
    // sort raw 
    if(key == '1')     {
        sortTypeInfo = "no sort";
        setup();
    }
    
    // sort alphabetically
    if(key == '2') {
        sortTypeInfo = "sorting alphabetically";
        ofSort(words, ofApp::sortOnABC);
    }
    
    // sort by length of word
    if(key == '3')     {
        sortTypeInfo = "sorting on word length";
        ofSort(words, ofApp::sortOnLength);
    }
    
    // sort by length of word
    if(key == '4')     {
        sortTypeInfo = "sorting on word occurrences";
        ofSort(words, ofApp::sortOnOccurrences);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    float nearestDist = 9999.9f;
    int nearestWord = -1;
    for(int i=0; i<words.size(); i++){
        int thisX = i*2;
        float thisDist = abs(x- 25 - thisX);
        if(thisDist<nearestDist){
            nearestWord = i;
            nearestDist = thisDist;
        }
    }
    if(nearestWord>-1){
        curWord = nearestWord;
    }
    // ofLog() << nearestDist;
    
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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