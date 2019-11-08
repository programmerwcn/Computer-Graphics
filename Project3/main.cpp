#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

using namespace std;

int main () {
glm::vec3 a(0,0,1);
glm::vec3 b(0,1,0);
glm::vec3 c = glm::cross(a,b);
cout << glm::to_string(c) << endl;

}