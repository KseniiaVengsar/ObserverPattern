#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observable {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void warning(const std::string& message) const {
        for (auto observer : observers) {
            if (observer) {
                observer->onWarning(message);
            }
        }
    }

    void error(const std::string& message) const {
        for (auto observer : observers) {
            if (observer) {
                observer->onError(message);
            }
        }
    }

    void fatalError(const std::string& message) const {
        for (auto observer : observers) {
            if (observer) {
                observer->onFatalError(message);
            }
        }
    }
};
class ConsoleWarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};


class FileErrorObserver : public Observer {
private:
    std::string filePath;

public:
    FileErrorObserver(const std::string& path) : filePath(path) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "Error: " << message << std::endl;
        }
    }
};
class ConsoleAndFileFatalErrorObserver : public Observer {
private:
    std::string filePath;

public:
    ConsoleAndFileFatalErrorObserver(const std::string& path) : filePath(path) {}

    void onFatalError(const std::string& message) override {
       
        std::cout << "Fatal Error: " << message << std::endl;

        
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "Fatal Error: " << message << std::endl;
        }
    }
};
int main() {
  
    Observable logger;

   
    ConsoleWarningObserver warningObserver;
    FileErrorObserver errorObserver("errors.log");
    ConsoleAndFileFatalErrorObserver fatalErrorObserver("fatal_errors.log");

    
    logger.addObserver(&warningObserver);
    logger.addObserver(&errorObserver);
    logger.addObserver(&fatalErrorObserver);

    
    logger.warning("This is a warning message");
    logger.error("This is an error message");
    logger.fatalError("This is a fatal error message");

    return 0;
}
