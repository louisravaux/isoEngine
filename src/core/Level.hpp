#pragma once

#include <string>
#include <vector>
#include <memory>

#include "core/Map.hpp"

class Level {
private:
    std::string name;
    std::string description;
    std::vector<std::unique_ptr<Map>> maps;
    int currentMapIndex;
    

public:
    Level(const std::string& levelName);
    ~Level();

    void addMap(std::unique_ptr<Map> map);
    Map* getCurrentMap() const;
    int getCurrentMapIndex() const;
    Map* getMap(int index) const;
    std::vector<Map*> getAllMaps() const;
    Map* nextMap();


};