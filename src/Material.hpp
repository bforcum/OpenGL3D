#include "config.hpp"

class Material {
public:
    Material(const char* filename);
    ~Material();
    void use(int unit);
private:
    unsigned int texture;
};