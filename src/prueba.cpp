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
		std::printf("onImageResults... faces.size():%d\n", faces.size());
		
		// Log content:
		for (std::map<FaceId, Face>::iterator it=faces.begin(); it!=faces.end(); ++it){
			LOG(plog::debug) << "FaceId: " << it->first << endl;
		
			Face face=it->second;
			
			std::printf("FaceId:%d\n", it->first);
			
			// Expresiones detectables
			std::printf("\nExpresiones:\n");
			std::printf("Sonrisa:%.2f\t", face.expressions.smile);
			std::printf("SonrisaFalsa:%.2f\t", face.expressions.smirk);
			std::printf("BocaAbierta:%.2f\t", face.expressions.mouthOpen);
			std::printf("LabiosApredados:%.2f\n", face.expressions.lipPress);

			std::printf("EntreCejasArriba:%.2f\t", face.expressions.innerBrowRaise);
			std::printf("CejasArriba:%.2f\t", face.expressions.browRaise);
			std::printf("CejasFruncidas:%.2f\t", face.expressions.browFurrow);
			std::printf("NarizArrugada:%.2f\n", face.expressions.noseWrinkle);

			std::printf("OjosCerrados:%.2f\t", face.expressions.eyeClosure);
			std::printf("OjosMuyAbierto:%.2f\t", face.expressions.eyeWiden);
			std::printf("Atenciòn:%.2f\t", face.expressions.attention);			
			std::printf("MandìbulaCaida:%.2f\n", face.expressions.jawDrop);
			
			// Emociones detectables
			std::printf("\nEmociones:\n");
			std::printf("Alegrìa:%.2f\t", face.emotions.joy);
			std::printf("Miedo:%.2f\t", face.emotions.fear);
			std::printf("Disgusto:%.2f\t", face.emotions.disgust);
			std::printf("Tristeza:%.2f\t", face.emotions.sadness);
			
			std::printf("Enojo:%.2f\n", face.emotions.anger);
			std::printf("Sorpresa:%.2f\t", face.emotions.surprise);
			std::printf("Positividad:%.2f\t", face.emotions.valence);
			std::printf("Desprecio:%.2f\n", face.emotions.contempt);			
			
			// Apariencias detectables
			std::printf("\nApariencia\n");
			std::printf("Lentes: %d\t", face.appearance.glasses);
			std::printf("Sexo: %d\t", face.appearance.gender);
			std::printf("Edad: %d\t", face.appearance.age);
			std::printf("Etnia: %d\n", face.appearance.ethnicity);
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


