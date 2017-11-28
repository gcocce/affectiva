#include <iostream>
#include <memory>
#include <chrono>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "Frame.h"
#include "Face.h"
#include "FrameDetector.h"
#include "AffdexException.h"

#include "FaceListener.h"
#include "ProcessStatusListener.h"

#include "PhotoDetector.h"
#include "CameraDetector.h"

#include <plog/Log.h>



using namespace std;
using namespace affdex;

class MyApp : public affdex::FaceListener {
public:
	MyApp(CameraDetector* det) {
		this->detector =det;
		int camId = 10;
		int camFPS = 60;

		detector->setCameraId(camId);
		detector->setCameraFPS(camFPS);		
		detector->setFaceListener(this);
	}

	void onFaceFound( float timestamp, FaceId faceId ){
		LOG(plog::debug) << "onFaceFound... faceId:" << faceId;
		std::printf("Face Found faceId:%d\n", faceId);
		
	}
	
	void onFaceLost(float timestamp, FaceId faceId){
		LOG(plog::debug) << "onFaceLost... faceId:" << faceId;
		std::printf("Face Lost faceId:%d\n", faceId);
	}

private:
  CameraDetector* detector;
};

void onFaceFound( float timestamp, FaceId faceId );

int main(int argc, char ** argsv)
{
	//affdex::FrameDetector(1);
	
	plog::init(plog::debug, "log.txt");
	LOG(plog::debug) << "Comienza el programa!";

	LOG(plog::debug) << "Se crea el detector de fotos!";
	CameraDetector detector;
  
	LOG(plog::debug) << "Se asigna el directorio del clasificador!";
	std::string classifierPath="/home/pi/workspace/affectiva/data";
	detector.setClassifierPath(classifierPath);
	
	LOG(plog::debug) << "Se crea el objeto MyApp!";
	MyApp* myApp = new MyApp(&detector);
	
	LOG(plog::debug) << "Se configuran los gestos detectables!";
	detector.setDetectSmile(true);
	detector.setDetectJoy(true);
	
	
	LOG(plog::debug) << "Se inicia el detector...!";
	detector.getDetectSmile();	
	detector.start();
	
	//detector->process(frame);
	
	LOG(plog::debug) << "Se detiene el detector...!";
	detector.stop();

	return 0;
}


