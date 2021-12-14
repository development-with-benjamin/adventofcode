#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>

struct PairInsert
{
    PairInsert() = default;
    char first;
    char second;
    char insert;
};

std::mutex getInsert_mutex;

class Polymer
{
public:
    Polymer() = default;
    Polymer(const std::string& initial) : polymer(initial) {};
    Polymer(const Polymer& other) {
        this->polymer = other.getPolymer();
        this->inserts = other.getInserts();
    };

    void setPolymer(const std::string& initial) { this->polymer = initial; };
    void setInserts(const std::vector<PairInsert> insert) { this->inserts = insert; };

    std::string getPolymer() const { return polymer; };
    std::vector<PairInsert> getInserts() const { return inserts; }; 

    void runInsertsThreaded() {
        const size_t processor_count = std::thread::hardware_concurrency();
        if(this->polymer.size() > processor_count * 2) {
            std::vector<std::string> subPolymers(processor_count, "");
            size_t subPolymersSize = this->polymer.size() / processor_count;
           
            // polymer string into equally large sub strings 
            size_t i;
            for(i = 0; i < processor_count - 1; ++i) {
                subPolymers.at(i) = this->polymer.substr(i * subPolymersSize, subPolymersSize + 1);
            }
            subPolymers.at(i) = this->polymer.substr(i * subPolymersSize);

            // start for each substring a thread that inserts the necessary elements
            std::vector<std::thread> threads;
            for(i = 0; i < subPolymers.size(); ++i) {
                threads.push_back(std::thread(runInserts_th, std::ref(subPolymers.at(i)), std::ref(*this)));
            }
            for(auto& thread : threads){
                thread.join();
            }
            
            // merge strings back together
            std::string temp = subPolymers.at(0);
            for(i = 1; i < subPolymers.size(); ++i) {
                temp += subPolymers.at(i).substr(1);
            }
            this->polymer = temp;
        }
        else {
            this->runInserts();
        }
    };

    static void runInserts_th(std::string& subpolymer, const Polymer& self) {
        std::string tempSubPolymer = subpolymer;
        size_t n_inserts = 1;
        char insert;

        for(size_t i = 0; i < subpolymer.size() - 1; ++i) {
            // I am such a moron. Locking a whole function where data is not altered.
            // getInsert_mutex.lock();
            insert = self.getInsert(subpolymer.at(i), subpolymer.at(i + 1));
            // getInsert_mutex.unlock();
            if(insert != 0) {
                tempSubPolymer.insert(i + n_inserts++, std::string(1, insert));
            }
        }
        subpolymer = tempSubPolymer;
    }

    void runInserts() {
        char insert;
        size_t n_inserts = 1;
        std::string tempPolymer = this->polymer;
        for(size_t i = 0; i < polymer.size() - 1; ++i) {
            insert = this->getInsert(polymer.at(i), polymer.at(i + 1));
            if(insert != 0) {
                tempPolymer.insert(i + n_inserts++, std::string(1, insert));
            }
        }
        this->polymer = tempPolymer;
    };
private:
    std::vector<PairInsert> inserts;
    std::string polymer;

    char getInsert(const char first, const char second) const {
        for(const PairInsert& pair : inserts) {
            if(pair.first == first && pair.second == second) {
                return pair.insert;
            }
        }
        return 0;
    }
};


Polymer getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return Polymer();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    std::getline(ss, line, '\n');
    
    Polymer input(line);
    std::vector<PairInsert> inserts;

    size_t pos;
    std::string arrow = " -> ";

    while(std::getline(ss, line, '\n')) {
        pos = line.find(arrow);
        if(pos == std::string::npos) {
            continue;
        }
        std::string pair = line.substr(0, pos);
        std::string insert = line.substr(pos + arrow.size());
        PairInsert pairInsert;
        pairInsert.first = pair.at(0);
        pairInsert.second = pair.at(1);
        pairInsert.insert = insert.at(0);
        inserts.push_back(pairInsert);
    }
    input.setInserts(inserts);
   return input;
}

int main(const int argc, const char** argv)
{
    if(argc != 2) {
        return -1;
    }

    Polymer polymer = getInput(argv[1]);
    const size_t STEPS = 40;

    for(size_t i = 0; i < STEPS; ++i) {
        polymer.runInsertsThreaded();
        std::cout << "step: " << i << std::endl;
    }

    std::string polymerstring = polymer.getPolymer();
    std::vector<size_t> alphabet(26, 0);

    for(size_t letter = 0; letter < alphabet.size(); ++letter) {
        for(size_t i = 0; i < polymerstring.size(); ++i) {
            if((char)letter + 'A' == polymerstring.at(i)) {
                alphabet.at(letter)++;
            }
        }
    }

    size_t max = 0;
    size_t min = (size_t) -1;

    for(size_t i = 0; i < alphabet.size(); ++i) {
        if(alphabet.at(i) > max) {
            max = alphabet.at(i);
        }
        if(alphabet.at(i) < min && alphabet.at(i) != 0) {
            min = alphabet.at(i);
        }
    }

    std::cout << max - min << std::endl;
    
    return 0;
}
