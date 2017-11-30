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

class MyApp : public affdex::FaceListener ,  affdex::ImageListener{
public:
	MyApp(CameraDetector* det, int cameraId) {
		this->detector =det;
		int camId = cameraId;
		int camFPS = 60;

		detector->setCameraId(camId);
		detector->setCameraFPS(camFPS);		
		detector->setFaceListener(this);
		detector->setImageListener(this);
	}

	void onFaceFound( float timestamp, FaceId faceId ){
		LOG(plog::debug) << "onFaceFound... faceId:" << faceId;
		std::printf("Face Found faceId:%d\n", faceId);
		mfaceId = faceId;
		
	}
	
	void onFaceLost(float timestamp, FaceId faceId){
		LOG(plog::debug) << "onFaceLost... faceId:" << faceId;
		std::printf("Face Lost faceId:%d\n", faceId);
	}
	
	void onImageResults(std::map<FaceId, Face> faces, Frame image){
		LOG(plog::debug) << "onImageResults... faces.size():" << faces.size();
		std::printf("onImageResults... faces.size():%l\n", faces.size());
		
		// Log content:
		for (std::map<FaceId, Face>::iterator it=faces.begin(); it!=faces.end(); ++it){
			LOG(plog::debug) << "FaceId: " << it->first << "FaceId: " << "\n";
			//std::cout << it->first << " => " << it->second << '\n';
		}
		
		if(mfaceId != -1){
			
			Face face = faces[mfaceId];
						
			LOG(plog::debug) << "onImageCapture... ";
			
			std::printf("smile:%f\n", face.expressions.smile);
			
			std::printf("mouthopen:%f\n", face.expressions.mouthOpen);
			
			//std::printf("sadness:%f\n", face.expressions.sadness);
			
			//face.emotions.surprise
			std::printf("surprise:%f\n", face.emotions.surprise);
		}
		
		std::printf("Image Results :\n");	
	}
	
	void onImageCapture(Frame image) {
		//LOG(plog::debug) << "onImageCapture... ";
		//std::printf("onImageCapture :\n");
	}

private:
  CameraDetector* detector;
  FaceId mfaceId = -1;
};

void onFaceFound( float timestamp, FaceId faceId );

int main(int argc, char ** argsv)
{
	int cameraId=0;
	
	cout << "Se inicia el programa con " << argc << " parametros\n";
	if(argc>1){
		cameraId=atoi(argsv[1]);
	}
	
	cout << "Id de la camara: " << cameraId << endl;
	
	plog::init(plog::debug, "log.txt");
	LOG(plog::debug) << "Comienza el programa!";

	LOG(plog::debug) << "Se crea el detector!";
	CameraDetector detector;
  
	LOG(plog::debug) << "Se asigna el directorio del clasificador!";
	std::string classifierPath="/home/pi/workspace/affectiva/data";
	detector.setClassifierPath(classifierPath);
	
	LOG(plog::debug) << "Se crea el objeto MyApp!";
	MyApp* myApp = new MyApp(&detector, cameraId);
	
	LOG(plog::debug) << "Se configuran los gestos detectables!";
	detector.setDetectSmile(true);
	detector.setDetectJoy(true);
	detector.setDetectAllExpressions(true);
	detector.setDetectAllEmotions(true);
	detector.setDetectAllEmojis(true);
	detector.setDetectAllAppearances(true);	
	
	
	cout << "Para terminar el programa use Ctrl+C";
	LOG(plog::debug) << "Se inicia el detector...!";
	detector.start();	
	while(true){
		sleep(0);
	}
	
	LOG(plog::debug) << "Se detiene el detector...!";
	detector.stop();

	return 0;
}


