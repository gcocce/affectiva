#include <stdio.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>
//#include <string>

#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

// Affectiva
#include "Frame.h"
#include "Face.h"
#include "FrameDetector.h"
#include "AffdexException.h"
#include "FaceListener.h"
#include "ProcessStatusListener.h"
#include "PhotoDetector.h"
#include "CameraDetector.h"

// Log
#include <plog/Log.h>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv/cv.hpp>

// MYSQL CONNECT
//#include "mysql_connection.h"
//#include "mysql_driver.h"

//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>

using namespace std;
using namespace cv;
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
		
		//std::printf("onImageResults... \n");
		
		if (faces.size()>0){
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
				std::printf("Etnia: %d\t", face.appearance.ethnicity);	
				std::printf("Edad: %d 1(<18) 2(18-24) 3(25-34) 4(35-44) 5(45-54) 6(55-64) 7(>64)\n", face.appearance.age);

				
				/* *****************************************************
				 * Esta seccion muestra datos relativos a la imagen capturada
				 * */
				std::printf("getBGRByteArrayLength: %d\n", image.getBGRByteArrayLength());
				std::printf("getTimestamp() %f\n", image.getTimestamp());
				std::printf("getWidth: %d\n", image.getWidth());
				std::printf("getHeight: %d\n", image.getHeight());
				std::printf("getColorFormat() : %d\n", image.getColorFormat());				
				
				/* *****************************************************
				 * Esta seccion guarda un archivo con el 
				 * contenido devuelto por frame.getImage()
				 * */
				cv::Mat* pt_img=image.getImage();
				
				size_t msize;
				
				if(pt_img->empty()){
					std::printf("\nMatrix is empty!!!!!!!!!!!!!!!!\n");
				}else{
					std::printf("\nMatrix total: %d\n", pt_img->total());
					std::printf("Matrix channels: %d\n", pt_img->channels());
					std::printf("Matrix elemSize: %d\n", pt_img->elemSize());
					std::printf("Matrix elemSize1: %d\n", pt_img->elemSize1());
					std::printf("Matrix depth: %d\n", pt_img->depth());
					
					// El tamano total ocupado es el producto de la cantidad de canales por 
					// el espacio total ocupado por cada canal
					msize=pt_img->total() * pt_img->channels();
					
					std::printf("Matrix size: %d\n", msize);
					
					std::printf("\n");
					std::time_t rawtime;
					std::tm* timeinfo;
					char time_buffer [80];

					std::time(&rawtime);
					timeinfo = std::localtime(&rawtime);

					std::strftime(time_buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);
					//std::puts(time_buffer);
					
					string filename;     
					ostringstream buffer;  
					//buffer << "20171212_" << detection << "_" << it->first << ".dat";      
					buffer << time_buffer << detection << "_" << it->first << ".dat";      
					filename = buffer.str();
					
					FILE * fd=fopen(filename.c_str(), "wb");
					fwrite(pt_img->data, msize, 1, fd);
					fclose(fd);		
				}
				
				std::printf("Fin extraccion de datos de la cara %d\n", it->first);
			}
			
				
			/*Lo siguiente da error al compilar junto con opencv
			* habria que encontrar la manera de hacerlo

			try {
				//cv::imwrite("20171205_.png", img , compression_params);
				
				//cv::imwrite("./20171205_.png", gray_image);
			}
			catch (runtime_error& ex) {
				fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
			}
			* 
			* */			
			
			/* *****************************************************
			 * Esta seccion se intento guardar el archivo con los datos 
			 * obtenidos de frame.getBGRByteArray() pero el shared pointer
			 * produjo errores y se descarto.
			
			std::shared_ptr<byte> shp_data=image.getBGRByteArray();
			
			shp_data.~shared_ptr();	
			delete(shp_data*);

			FILE * fd=fopen(filename.c_str(), "wb");
			fwrite(shp_data.get(), image.getBGRByteArrayLength(), 1, fd);
			fclose(fd);
			
			*/		
		
			std::printf("Deteccion %d\n", detection);
			
			detection++;
		
		}
		//std::printf("Fin onImageResults\n");
		
	}
	
	void onImageCapture(Frame image) {
		//LOG(plog::debug) << "onImageCapture... ";
		//std::printf("onImageCapture :\n");
	}

private:
  CameraDetector* detector;
  
  FaceId mfaceId = -1;
  long int detection=0;
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
	
	/*
	cv::Mat image;
	image = cv::imread( "foto.jpg", 1 );
	
	if(!image.data)
    {
      printf("No image data \n");
      return -1;
    }
    	
	cv::imwrite("./20171205_.png", image);
	*/
	
	plog::init(plog::debug, "log.txt");
	LOG(plog::debug) << "Comienza el programa!";

	LOG(plog::debug) << "Se crea el detector!";
	cout << "Se crea el detector..." << endl;
	CameraDetector detector;
  
	LOG(plog::debug) << "Se asigna el directorio del clasificador!";
	std::string classifierPath="/home/pi/workspace/affectiva/data";
	detector.setClassifierPath(classifierPath);
	
	LOG(plog::debug) << "Se crea el objeto MyApp!";
	cout << "Se crea el objeto MyApp..." << endl;
	MyApp* myApp = new MyApp(&detector, cameraId);
	
	LOG(plog::debug) << "Se configuran los gestos detectables!";
	detector.setDetectSmile(true);
	detector.setDetectJoy(true);
	detector.setDetectAllExpressions(true);
	detector.setDetectAllEmotions(true);
	detector.setDetectAllEmojis(true);
	detector.setDetectAllAppearances(true);	

	cout << "Se inicia el detector." << endl;	
	cout << "Para terminar el programa use Ctrl+C"  << endl;
	LOG(plog::debug) << "Se inicia el detector...!";
	detector.start();	
	while(true){
		sleep(0);
	}
	
	LOG(plog::debug) << "Se detiene el detector...!";
	detector.stop();

	return 0;
}


