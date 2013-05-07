#include "testApp.h"

// REPLACE THESE WITH YOUR API KEY AND API SECRET
// http://www.flickr.com/services/developer/

string API_KEY = "b4e841075977098cb808f802e4f0caae";
string API_SECRET = "5cbe7a922d259e49";

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0);
    
    //-----Load Traceroute Data into Vector from traceroute.txt------
    
    latestImage.resize(200);
    
    ofBuffer buffer = ofBufferFromFile("traceroute.txt");
    while (!buffer.isLastLine()) {
        ipAddress.push_back(buffer.getNextLine());
    }
    ipAddress.push_back(buffer.getNextLine());
    
    string url;
    for (int i=0; i<ipAddress.size()-1 ; i++){
        url = ipAddress[i];
        cout<< "IM IN THE FOR LOOP "<< ipAddress[i] <<endl;
        
        
        //-----Make get request to /json/ipAddress-------
        
        action_url = "http://freegeoip.net/json/";
        full_ip_url = action_url + ipAddress[i];
        cout<< "full_ip_url = " + full_ip_url <<endl;
        
        ofAddListener(httpUtils.newResponseEvent,this,&testApp::newResponse);
        httpUtils.start();
        
        ofxHttpForm form;
        form.action = full_ip_url;
        form.method = OFX_HTTP_GET;
        httpUtils.addForm(form);
        requestStr = "message sent: " + ofToString(counter);
        counter++;
        cout<< "GET request sent..." <<endl;
        
        //-----Now parse the JSON --------
        
        parsed = json.open(full_ip_url);
        if (parsed) {
            cout<< json.getRawString() <<endl;
            city.push_back(json["city"]);
            //cout<< json.toStyledString() <<endl;
            
            
        }
        else{
            cout<< "FAILED TO PARSE JSON!!!" <<endl;
        }
    }
    
    
    
    //-----Flickr API Connection-------
    
    for (vector<ofxJSONElement>::iterator iter = city.begin(); iter != city.end(); ++iter) {
        bool bAuthenticated = flickrAPI.authenticate( API_KEY, API_SECRET, ofxFlickr::FLICKR_WRITE );
        const char * cityChar = iter->getRawString().c_str();
        string newCity;
        for (int i = 1; cityChar[i] != '\0'; ++i) {
            cout << cityChar[i] << endl;
            newCity += cityChar[i];
            if (cityChar[i+3] == '\0') {
                break;
            }
        }
        results.push_back(flickrAPI.search(newCity));
        currentResult = 0;
        lastChanged = ofGetElapsedTimeMillis();
        
    }
    vector<ofImage>::iterator imageit = latestImage.begin();
    
    for (vector< vector<ofxFlickr::Media> >::iterator crazyit = results.begin(); crazyit != results.end(); ++crazyit) {
        string lasturl;
        int i = 0;
        //cout << "GETTING " << (*crazyit).size() << " IMAGES FROM FLICKR..." << endl;
        cout << "GETTING IMAGES FROM FLICKR..." << endl;
        //cout << "GETTING IMAGES FROM FLICKR..." << endl;
        for (int i = 0; i < (*crazyit).size(); ++i) {
            if ( imageit != latestImage.end() && lasturl != (*crazyit)[i].getURL() && i < 20){
                imageit->loadImage((*crazyit)[i].getURL());
                imageit++;
                lasturl = (*crazyit)[i].getURL();
                //cout << "added an image" << endl;
                //cout<< "GETTING IMAGE FROM...."<< lasturl <<endl;
            }
        }
        crazyit++;
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    //    if ( ofGetElapsedTimeMillis() - lastChanged > 1000 ){
    //        lastChanged = ofGetElapsedTimeMillis();
    //        currentResult++;
    //        if ( currentResult >= results.size()){
    //            currentResult = 0;
    //        }
    //
    //        vector<ofImage>::iterator imageit = latestImage.begin();
    //        for (vector< vector<ofxFlickr::Media> >::iterator crazyit = results.begin(); crazyit != results.end(); ++crazyit) {
    //            if ( results.size() > 0 ){
    //                imageit->loadImage((*crazyit)[currentResult].getURL());
    //            }
    //            imageit++;
    //
    //        }
    //    }
}

//--------------------------------------------------------------
void testApp::draw(){
    int i = 0;
    int x = 0;
    int y = 0;
    for (vector<ofImage>::iterator imageit = latestImage.begin(); imageit != latestImage.end(); ++imageit) {
        if (x > ofGetWindowWidth()) {
            x = 0 + x;
            y = 500 + y;
        }
        
        if (i == 0 && imageit->isAllocated()) {
            imageit->draw(0, 0);
            x = 20 + x;
        } else if (imageit->isAllocated()){
            imageit->draw(imageit->getWidth()+x, 0+y);
            x = 10 + x;
        }
        i++;
    }
    
    
}

//--------------------------------------------------------------
void testApp::newResponse(ofxHttpResponse & response){
    //------Get response ready to draw to screen-----
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
    //cout << response.responseBody << endl;
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}