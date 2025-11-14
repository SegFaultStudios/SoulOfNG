#ifndef NET_SERIALIZATION_HPP
#define NET_SERIALIZATION_HPP

#include <vector>
#include <cstdint>

class Serialization
{
public:
    Serialization();

    Serialization(const uint8_t* data, size_t size);
private:
    std::vector<uint8_t> m_buffer;
    std::size_t m_readPosition{0};
};

#endif //NET_SERIALIZATION_HPP