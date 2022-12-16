#include "game.h"
#include <iostream>
#include "hittable_list.h"
#include "sphere.h"
#include "spotlight.hpp"

#define MAX_REFLECTIONS_ALLOWED 5

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

auto Ia = glm::vec3(0.1,0.1,0.1); // TODO: according to input
glm::vec3 aboveZero(glm::vec3 v){
    return glm::vec3(std::fmax(v[0], 0), std::fmax(v[1],0), std::fmax(v[2], 0));
}

glm::vec3 belowOne(glm::vec3 v){
    return glm::vec3(std::fmin(v[0], 1), std::fmin(v[1],1), std::fmin(v[2], 1));
}

glm::vec3 boundCheck(glm::vec3 color)
{
    return belowOne(aboveZero(color));
}

void Game::Init()
{		
	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");
	
	//AddTexture("../../res/textures/box0.bmp",false);
	auto aspect_ratio = (16.0/9.0);
	auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

	double image_width = 400;
	double image_height = static_cast<int>(image_width / aspect_ratio);;

	auto horizontal = glm::vec3(viewport_width, 0, 0);
    auto vertical = glm::vec3(0, viewport_height, 0);

	auto origin = glm::vec3(0,0,0);
	auto lower_left_corner = origin - horizontal/2 - vertical/2 - glm::vec3(0, 0, focal_length);

    unsigned char *data = (unsigned char*)malloc(image_width * image_height * 4);

	// World
    hittable_list world;
	auto light_list =  std::vector<shared_ptr<light>>();
	light_list.push_back(std::make_shared<spotlight>(glm::vec3(3 ,10.0, 3), glm::vec3(0.0 ,-10.0, -1.0), pi,glm::vec3(1,1,1)));
	//light_list.push_back(std::make_shared<spotlight>(glm::vec3(0.0 ,-10.0, 1.0), glm::vec3(0.0 ,10.0, -1.0), 0.1,glm::vec3(1,1,1)));
    world.add(make_shared<sphere>(glm::vec3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(glm::vec3(0,-100.5,-1), 100));


	int counter =0;
	for (int j = image_height-1; j >= 0; --j) 
	{
		for (int i = 0; i < image_width; ++i) 
		{
			auto u = double(i) / (image_width-1);
			auto v = double(j) / (image_height-1);
			ray r = ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);

			glm::vec3 pixel_color = boundCheck(Ia * 0.7 +ray_color(r, world, light_list, 0));
			data[counter] = pixel_color[0]*255;
			data[counter+1] = pixel_color[1]*255;
			data[counter+2] = pixel_color[2]*255;
			data[counter+3] = 255;
			counter = counter+4;
		}
    }
	AddTexture(image_width, image_height, data);

	AddShape(Plane,-1,TRIANGLES);
	
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;


	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}


// double Game::hit_sphere(const glm::vec3& center, double radius, const ray& r) {
//     glm::vec3 oc = r.origin() - center;
//     auto a = dot(r.direction(), r.direction());
//     auto b = 2.0 * dot(oc, r.direction());
//     auto c = dot(oc, oc) - radius*radius;
//     auto discriminant = b*b - 4*a*c;
//     if (discriminant < 0) {
//         return -1.0;
//     } else {
// 		auto first = (-b - sqrt(discriminant) ) / (2.0*a);
// 		return first;
//     }
// }

// glm::vec3 Game::ray_color(const ray& r) 
// {
// 	auto t = hit_sphere(glm::vec3(0,0,-1), 0.5, r);
//     if (t > 0.0) {
//         glm::vec3 N = glm::vec3(r.at(t) - glm::vec3(0,0,-1));
//         return 0.5*glm::vec3(N[0]+1, N[1]+1, N[2]+1);
//     }

//     glm::vec3 unit_direction = glm::vec3(r.direction());
// 	t = 0.5*(unit_direction[1] + 1.0);
//     return (1.0-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
// }





ray getReflectedVector(ray r, glm::vec3 normal, glm::vec3 hitPoint) {
    auto n = glm::normalize(normal);
    auto u = glm::normalize(r.direction());
    glm::vec3 reflection = glm::normalize(u - (2 * n * glm::dot(u, n)));
    return ray(hitPoint + 0.001f * reflection, reflection);
}

glm::vec3 Game::ray_color(const ray& r, const hittable& world, std::vector<shared_ptr<light>> lights, int numOfReflection) {
    if (numOfReflection == MAX_REFLECTIONS_ALLOWED)
    {
        return ZERO_VECTOR;
    }
    hit_record rec;
    hit_record rec2;
	glm::vec3 color = ZERO_VECTOR;
    if (world.hit(r, 0, infinity, rec))
    {
        for(shared_ptr<light> light : lights)
        {
            ray checkRay = light->getRayFromPointToLight(rec.p);
            if(!world.hit(checkRay, 0.001, infinity, rec2))
            {
                color = color + light->getColor(r,rec);
            }
        }
        color = boundCheck(color) + 0.2*ray_color(getReflectedVector(r,rec.normal,rec.p),world,lights,numOfReflection+1);
        return boundCheck(color);
    }
    //No Hit
    return ZERO_VECTOR;
}



void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
