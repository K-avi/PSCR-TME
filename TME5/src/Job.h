#pragma once

#include "Barrier.h"
#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include <iostream>

namespace pr {



class Job {
public:
	virtual void run () = 0;
	virtual ~Job() {};
};

// Job concret : exemple

//
/*
class SleepJob : public Job {
	int calcul (int v) {
		std::cout << "Computing for arg =" << v << std::endl;
		// traiter un gros calcul
		//this_thread::sleep_for(1s);
		int ret = v % 255;
		std::cout << "Obtained for arg =" << arg <<  " result " << ret << std::endl;
		return ret;
	}
	int arg;
	int * ret;
public :
	SleepJob(int arg, int * ret) : arg(arg), ret(ret) {}
	void run () {
		* ret = calcul(arg);
	}
	~SleepJob(){}
};*/



class PixelJob : public Job{
	//const pr::Vec3D& vec;
	pr::Rayon& ray ;
	pr::Scene & scene;
	pr::Color & pixel ; 
	std::vector<Vec3D>& lights; 

	pr::Barrier& b ;

	int findClosestInter(const Scene & scene, const Rayon & ray) {
	auto minz = std::numeric_limits<float>::max();
	int targetSphere = -1;
	int index = 0;
	for (const auto & obj : scene) {
		// rend la distance de l'objet a la camera
		auto zinter = obj.intersects(ray);
		// si intersection plus proche  ?
		if (zinter < minz) {
			minz = zinter;
			targetSphere = index;
		}
		index++;
	}
	return targetSphere;
	}

	// Calcule l'angle d'incidence du rayon à la sphere, cumule l'éclairage des lumières
	// En déduit la couleur d'un pixel de l'écran.
	Color computeColor(const Sphere & obj, const Rayon & ray, const Vec3D & camera, std::vector<Vec3D> & lights) {
		Color finalcolor = obj.getColor();

		// calcul du rayon et de sa normale a la sphere
		// on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
		// on le normalise a la longueur 1, on multiplie par la distance à l'intersection
		Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
		// le point d'intersection
		Vec3D intersection = rayInter + camera;
		// la normale a la sphere au point d'intersection donne l'angle pour la lumiere
		Vec3D normal = obj.getNormale(intersection);
		// le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
		double dt = 0;
		// modifier par l'eclairage la couleur
		for (const auto & light : lights) {
			// le vecteur de la lumiere au point d'intersection
			Vec3D tolight = (light - intersection);
			// si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
			if (obj.intersects(Rayon(light,intersection)) >= tolight.length() - 0.05 ) {   //  epsilon 0.05 for double issues
				dt += tolight.normalize() & normal ; // l'angle (scalaire) donne la puissance de la lumiere reflechie
			}
		}
		// eclairage total
		finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

		return finalcolor;
	}

	
	
	
	public : 
		PixelJob( pr::Rayon& ray, pr::Scene & scene, std::vector<pr::Vec3D>& lights, Color& color,Barrier& b)
		: ray(ray), scene(scene), pixel(color), lights(lights), b(b)
		{}
		void run(){
			int targetSphere = findClosestInter(scene, ray);

				const Sphere & obj = *(scene.begin() + targetSphere);
				// pixel prend la couleur de l'objet
				Color finalcolor = computeColor(obj, ray, scene.getCameraPos(), lights);
				// le point de l'image (pixel) dont on vient de calculer la couleur				
				// mettre a jour la couleur du pixel dans l'image finale.
				pixel = finalcolor;
			
			b.increment_finished();

		}

};


}
