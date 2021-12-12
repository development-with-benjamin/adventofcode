#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

class Cave {
public:
    Cave() = delete;
    Cave(const std::string caveName): caveName(caveName){};

    bool isSmallCave() const {
        return caveName.at(0) >= 97 && caveName.at(0) <= 122;
    };

    std::string getCaveName() const {
        return caveName;
    };

    void addNeighbour(std::shared_ptr<Cave> cave) {
        neighbours.push_back(cave);
    }

    std::vector<std::shared_ptr<Cave>> getNeighbours() const {
        return neighbours;
    }

private:
    std::string caveName;
    std::vector<std::shared_ptr<Cave>> neighbours;
};

class Route {
public:
    Route() = default;

    bool canVisit(const std::shared_ptr<Cave> cave) {
        if(!cave->isSmallCave()) return true;

        for(const auto& caveName : route) {
            if(cave->getCaveName() == caveName) {
                if(!hasDuplicate()) {
                    return true;
                }
                return false;
            }
        }

        return true;
    }

    void push_back(std::string caveName) {
        route.push_back(caveName);
    }

    std::vector<std::string> getRoute() const {
        return route;
    }


    friend std::ostream& operator<<(std::ostream& os, const Route& route);
private:
    bool hasDuplicate() {
        std::vector<std::string> sortedRoutes;
        sortedRoutes.assign(route.begin(), route.end());

        for(int i = 0; i < sortedRoutes.size(); i++) {
            if(sortedRoutes.at(i).at(0) <= 90) {
                sortedRoutes.erase(sortedRoutes.begin()+i);
                i--;
            }
        }

        std::sort(sortedRoutes.begin(), sortedRoutes.end());
        const bool hasDuplicates = std::adjacent_find(sortedRoutes.begin(), sortedRoutes.end()) != sortedRoutes.end();

        return hasDuplicates;
    }
    bool smallCaveVisitedTwice = false;
    std::vector<std::string> route;
};

std::ostream& operator<<(std::ostream& os, const Route& route) {
    for(const auto caveName : route.getRoute()) {
        os << caveName << " - ";
    }
    return os;

}


std::shared_ptr<Cave> findOrAddCave(std::vector<std::shared_ptr<Cave>>& caves, std::string caveName) {
    for(const auto & cave : caves) {
        if(cave->getCaveName() == caveName) {
            return cave;
        }
    }

    std::shared_ptr<Cave> cave = std::make_shared<Cave>(Cave(caveName));
    caves.push_back(cave);

    return cave;
}

std::vector<std::shared_ptr<Cave>> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<std::shared_ptr<Cave>> ();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    std::vector<std::shared_ptr<Cave>> input;
    
    while(std::getline(ss, line, '\n')) {
        size_t splitPos = line.find('-');
        if(splitPos == std::string::npos) {
            continue;
        }
        std::string caveOneName = line.substr(0, splitPos);
        std::string caveTwoName = line.substr(splitPos+1);
        std::shared_ptr<Cave> caveOne = findOrAddCave(input, caveOneName);
        std::shared_ptr<Cave> caveTwo = findOrAddCave(input, caveTwoName);
        caveOne->addNeighbour(caveTwo);
        caveTwo->addNeighbour(caveOne);
    }

    return input;
}

std::shared_ptr<Cave> findStart(std::vector<std::shared_ptr<Cave>> & caves) {
    for(const auto& cave : caves) {
        if(cave->getCaveName() == "start") {
            return cave;
        }
    }

    return nullptr;
}
void calcRoutes(std::vector<Route>& routes, const std::shared_ptr<Cave> cave, Route curRoute=Route()) {

    curRoute.push_back(cave->getCaveName());
    
    if(cave->getCaveName() == "end") {
        routes.push_back(curRoute);
        return;
    }

    for(const auto& neighour: cave->getNeighbours()) {
        if(neighour->getCaveName() == "start") continue;
        if(curRoute.canVisit(neighour)) {
            calcRoutes(routes, neighour, curRoute);
        }
    }

}

int main(const int argc, const char* argv[]) {
    if(argc != 2) {
        return -1;
    }

    std::vector<std::shared_ptr<Cave>> caves = getInput(argv[1]);
    std::vector<Route> routes;

    auto startCave = findStart(caves);
    if(startCave) {
        calcRoutes(routes, startCave);
    }

    std::cout << routes.size() << std::endl;
    
    return 0;
}