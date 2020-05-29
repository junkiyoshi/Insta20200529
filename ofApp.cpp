#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofSetLineWidth(3);

	this->cap.open("D:\\MP4\\video.mp4");
	this->cap_size = cv::Size(512, 288);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_frames = 25 * 27;
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			return;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}

	for (int i = 0; i < 128; i++) {

		ofMesh mesh;
		mesh.addVertex(glm::vec3(-128, -72, 0));
		mesh.addVertex(glm::vec3(128, -72, 0));
		mesh.addVertex(glm::vec3(128, 72, 0));
		mesh.addVertex(glm::vec3(-128, 72, 0));

		mesh.addTexCoord(glm::vec3(0, 0, 0));
		mesh.addTexCoord(glm::vec3(512, 0, 0));
		mesh.addTexCoord(glm::vec3(512, 288, 0));
		mesh.addTexCoord(glm::vec3(0, 288, 0));

		mesh.addIndex(0); mesh.addIndex(1); mesh.addIndex(2);
		mesh.addIndex(0); mesh.addIndex(2); mesh.addIndex(3);

		this->mesh_list.push_back(mesh);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	
	ofSetColor(255);

	int i = 0;
	for (auto& mesh : this->mesh_list) {

		auto location = glm::vec3(0, 0, ofMap(i, 0, 128, -200, 200));
		int n = (i + ofGetFrameNum()) % this->number_of_frames;
		this->frame_list[n].copyTo(this->frame);
		this->image.update();

		ofPushMatrix();
		ofTranslate(location);

		this->image.bind();
		mesh.draw();
		this->image.unbind();

		ofPopMatrix();

		i++;
	}

	ofNoFill();
	ofSetColor(39);
	ofDrawBox(0, 0, 0, 256, 144, 400);


	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}