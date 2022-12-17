#include "game.h"
#include <iostream>
#include "hittable_list.h"
#include "sphere.h"
#include "spotlight.hpp"
#include "directionalLight.h"
#include "plane.h"
#include <cmath>
#include <fstream>
#include <string>
#include "sstream"

#define MAX_REFLECTIONS_ALLOWED 1

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

std::vector<std::string> splitLine(std::string line, char delimiter) {
    std::vector<std::string> lineParts;
    std::istringstream iss(line);
    std::string part;
    while (std::getline(iss, part, delimiter))
        lineParts.push_back(part);
    return lineParts;
}

glm::vec4 getVectorFromCoordinates(std::vector<std::string> lineParts) {
    auto vector = glm::vec4 (std::stof(lineParts[1]),
                             std::stof(lineParts[2]),
                             std::stof(lineParts[3]),
                             std::stof(lineParts[4]));
    return vector;
}

glm::vec3 camera;
glm::vec3 Ia;
std::vector<glm::vec4> objectColors;
std::vector<glm::vec4> lightDirections;
std::vector<std::tuple<std::string,glm::vec4>> objects;
std::vector<glm::vec4> spotlightsPosition;
std::vector<glm::vec4> lightIntensities;

std::tuple<std::string, glm::vec4> getTuple(std::string name, glm::vec4 vector) {
    return std::tuple<std::string,glm::vec4>(name, vector);
}

void parse(std::string filePath) {
    std::ifstream file(filePath, std::ios::in);
    if (!file) {
        printf("can't open file");
    }
    std::string line;

    while (std::getline(file, line)) {
        // Split the line into words
        auto lineParts = splitLine(line, ' ');
        std::string type = lineParts[0];
        auto vector = getVectorFromCoordinates(lineParts);
        if (type == "e") {
            camera = glm::vec3(vector[0], vector[1], vector[2]);
        }
        else if (type == "o") {
            objects.push_back(getTuple("object", vector));
        }
        else if (type == "c") {
            objectColors.push_back(vector);
        }
        else if (type == "d") {
            lightDirections.push_back(vector);
        }
        else if (type == "a") {
            Ia = glm::vec3(vector[0], vector[1], vector[2]);
        }
        else if (type == "p") {
            spotlightsPosition.push_back(vector);
        }
        else if (type == "i") {
            lightIntensities.push_back(vector);
        }
        else if (type == "r") {
            objects.push_back(getTuple("reflective", vector));
        }
        else if (type == "t") {
            objects.push_back(getTuple("transparent", vector));
        }
    }
}



hittable_list createWorld() {
    auto index = 0;
    hittable_list world;
    for (auto object : objects){
        auto name = std::get<0>(object);
        auto hittableVector = std::get<1>(object);
        auto color = objectColors[index];
        auto shininess = color[3];
        auto colorVec = glm::vec3(color[0], color[1], color[2]);

        bool isPlane = hittableVector[3] < 0;
        auto transparent = 0.f;
        auto reflectiveValue = 0.f;
        if (name[0] == 't') {
            transparent = 1.0f;
            colorVec = glm::vec3(0.1f,0.1f,0.1f);
        }
        else if (name[0] == 'r'){
            reflectiveValue = 1.0f;
            colorVec = glm::vec3(0.f,0.f,0.f);
        }
        if (isPlane) {
            world.add(std::make_shared<plane>(transparent, reflectiveValue, shininess, colorVec, hittableVector));
        }
        else {
            auto center = glm::vec3(hittableVector[0],hittableVector[1],hittableVector[2]);
            auto radius = hittableVector[3];
            world.add(std::make_shared<sphere>(center, radius,transparent,reflectiveValue, shininess, colorVec));
        }
        index++;
    }
    return world;
}

std::vector<shared_ptr<light>> createLights() {
    std::vector<shared_ptr<light>> lights;
    auto indexForSpotlights = 0;
    auto indexForColors = 0;
    for (auto lightDirectionWithInfoOnType : lightDirections)
    {
        auto isSpotlight = lightDirectionWithInfoOnType[3] == 1.0;
        auto lightDirection = glm::vec3(lightDirectionWithInfoOnType[0], lightDirectionWithInfoOnType[1], lightDirectionWithInfoOnType[2]);
        auto lightColor = glm::vec3(lightIntensities[indexForColors][0], lightIntensities[indexForColors][1], lightIntensities[indexForColors][2]);
        indexForColors++;
        if (isSpotlight)
        {
            auto pos = spotlightsPosition[indexForSpotlights];
            indexForSpotlights++;
            float cutOff = std::acos(glm::clamp(pos[3], -0.99f, 0.99f));
            auto realPos = glm::vec3(pos[0], pos[1], pos[2]);
            lights.push_back(std::make_shared<spotlight>(realPos,lightDirection, cutOff, lightColor));
        }
        else
        {
            lights.push_back(std::make_shared<directionalLight>(lightDirection, lightColor));
        }
    }
    return lights;
}

void Game::Init()
{		
	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");
	
	//AddTexture("../../res/textures/box0.bmp",false);
	auto aspect_ratio = (16.0/16.0);
	auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

	double image_width = 800;
	double image_height = static_cast<int>(image_width / aspect_ratio);;

	auto horizontal = glm::vec3(viewport_width, 0, 0);
    auto vertical = glm::vec3(0, viewport_height, 0);

	auto origin = camera;
	auto lower_left_corner = origin - horizontal/2 - vertical/2 - glm::vec3(0, 0, focal_length);

    unsigned char *data = (unsigned char*)malloc(image_width * image_height * 4);

	// Create World and Lights from file
    auto lights =  std::vector<shared_ptr<light>>();
    hittable_list world;
    parse("../../assignment 2/scene1.txt");
    world = createWorld();
    lights = createLights();

	int counter =0;
	for (int j = image_height-1; j >= 0; --j) 
	{
		for (int i = 0; i < image_width; ++i) 
		{
			auto u = double(i) / (image_width-1);
			auto v = double(j) / (image_height-1);
			ray r = ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            glm::vec3 rayColor = ray_color(r, world, lights, 0);
            glm::vec3 pixel_color;
            pixel_color = boundCheck(rayColor + Ia * 0.7);
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


ray getReflectedVector(ray r, glm::vec3 normal, glm::vec3 hitPoint) {
    auto n = glm::normalize(normal);
    auto u = glm::normalize(r.direction());
    glm::vec3 reflection = glm::normalize(u - (2 * n * glm::dot(u, n)));
    return ray(hitPoint + 0.001f * reflection, reflection);
}

double getAngle(glm::vec3 first, glm::vec3 second) {
    // Calculate the dot product of the two vectors
    float dot = first[0] * second[0] + first[1] * second[1] + first[2] * second[2];

    // Calculate the magnitudes of the two vectors
    float mag1 = sqrt(first[0] * first[0] + first[1] * first[1] + first[2] * first[2]);
    float mag2 = sqrt(second[0] * second[0] + second[1] * second[1] + second[2] * second[2]);

    // Calculate and return the angle between the two vectors using the dot product and magnitudes
    return acos(glm::clamp(dot / (mag1 * mag2), -0.99f, 0.99f));

}

glm::vec3 rotate(glm::vec3 first, glm::vec3 second, double angle) {
    double c = cos(angle);
    double s = sin(angle);

    // Calculate the rotated vector using a matrix representation of the rotation
    glm::vec3 rotated;
    rotated[0] = first[0] * (c + second[0] * second[0] * (1 - c)) +
                first[1] * (second[0] * second[1] * (1 - c) - second[2] * s) +
                first[2] * (second[0] * second[2] * (1 - c) + second[1] * s);
    rotated[1] = first[0] * (second[1] * second[0] * (1 - c) + second[2] * s) +
            first[1] * (c + second[1] * second[1] * (1 - c)) +
            first[2] * (second[1] * second[2] * (1 - c) - second[0] * s);
    rotated[2] = first[0] * (second[2] * second[0] * (1 - c) - second[1] * s) +
            first[1] * (second[2] * second[1] * (1 - c) + second[0] * s) +
            first[2] * (c + second[2] * second[2] * (1 - c));
    return rotated;
}

glm::vec3 Game::getRefractedDirection(glm::vec3 normal, float n1, float n2, glm::vec3 originalRayDirection) {
    double theta1 = getAngle(-1*originalRayDirection, normal); // angle between the incident ray and the normal
    double theta2 = asin(glm::clamp((n1 / n2 * sin(theta1)), -0.99, 0.99)); // refracted angle
    return rotate(originalRayDirection,normal, theta2 - theta1); // refracted direction
}

ray Game::getSnellsRay(ray original, float n1, float n2, hit_record hit) {
    ray newRay = ray(hit.p, getRefractedDirection(hit.normal,n1,n2,original.direction()));
    return newRay;
}


glm::vec3 Game::ray_color(const ray& r, const hittable& world, const std::vector<shared_ptr<light>> lights, int numOfReflection) {
    if (numOfReflection == MAX_REFLECTIONS_ALLOWED)
    {
        return ZERO_VECTOR;
    }
    hit_record rec;
    hit_record rec2;

    if (world.hit(r, 0.001, infinity, rec))
    {
        float n1,n2;
        glm::vec3 color = ZERO_VECTOR;
        for(shared_ptr<light> light : lights) {
            ray checkRay = light->getRayFromPointToLight(rec.p);
            if (!world.hit(checkRay, 0.001, infinity, rec2)) {
                color = color + light->getColor(r, rec);
            }
        }
        if (!rec.front_face)
        {
            n1 = 1.5;
            n2 = 1;
        } else
        {
            n1 = 1;
            n2 = 1.5;
        }
        color = color * rec.color + ray_color(getReflectedVector(r,rec.normal,rec.p),world,lights,numOfReflection+1) * rec.reflectiveness
                        + (ray_color(getSnellsRay(r,n1,n2,rec), world, lights, numOfReflection + 1) * rec.transparency);
        return (boundCheck(color));
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
