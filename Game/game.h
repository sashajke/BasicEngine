#pragma once
#include "scene.h"
#include "ray.cpp"
#include "hittable.h"
#include "light.h"
#include "constantsClass.h"


class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
    void Motion();
    //glm::vec3 Game::ray_color(const ray& r);
    glm::vec3 ray_color(const ray& r, const hittable& world, std::vector<shared_ptr<light>> lights, int numOfReflections);
    //double Game::hit_sphere(const glm::vec3& center, double radius, const ray& r);
	double dot_product(const glm::vec3 &v, const glm::vec3 &u);
	~Game(void);
};

