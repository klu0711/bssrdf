#ifndef S0008E_ALLOCATOR_h
#define S0008E_ALLOCATOR_h

#include <memory>
class allocator
{
public:
    allocator(){};
    char* memory;
    unsigned int offset;
    void init(unsigned int sizeInBytes)
    {
       // this->memory = (char*)malloc(sizeInBytes);
    }

    void* allocate(unsigned int sizeOfObject)
    {
       // return memory += offset;
    }
private:

};

#endif //S0008E_ALLOCATOR_h