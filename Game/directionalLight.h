
#ifndef GAME_DIRECTIONALLIGHT_H
#define GAME_DIRECTIONALLIGHT_H


#include "light.h"

class directionalLight : public light {
public:
    directionalLight(glm::vec3 direction, glm::vec3 color);
    virtual ray getRayFromPointToLight(glm::vec3 point)const override;
};


#endif //GAME_DIRECTIONALLIGHT_H
