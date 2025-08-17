#include "core/Level.hpp"

Level::Level(const std::string& levelName) : name(levelName) {
    currentMapIndex = 0;
}

Level::~Level() {
    // Destructor implementation
}

void Level::addMap(std::unique_ptr<Map> map) {
    maps.push_back(std::move(map));
}

Map* Level::getCurrentMap() const {
    if (currentMapIndex < maps.size()) {
        return maps[currentMapIndex].get();
    }
    return nullptr;
}

int Level::getCurrentMapIndex() const {
    return currentMapIndex;
}

Map* Level::getMap(int index) const {
    if (index >= 0 && index < maps.size()) {
        return maps[index].get();
    }
    return nullptr;
}

Map* Level::nextMap() {
    currentMapIndex = (currentMapIndex + 1) % maps.size();
    return maps[currentMapIndex].get();
}

std::vector<Map*> Level::getAllMaps() const {
    std::vector<Map*> allMaps;
    for (const auto& map : maps) {
        allMaps.push_back(map.get());
    }
    return allMaps;
}
