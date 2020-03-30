#include "indexcalculator.hpp"
#include <iostream>

std::vector<unsigned int> IndexCalculator::CalcIndex(std::vector<float>& rhsverticies) {
    for (size_t index = 0; index < rhsverticies.size(); index += 5) {
        Vertex vert{};
        vert.x = rhsverticies.at(index);
        vert.y = rhsverticies.at(index + 1);
        vert.z = rhsverticies.at(index + 2);
        vert.td1 = rhsverticies.at(index + 3);
        vert.td2 = rhsverticies.at(index + 4);
        input.push_back(vert);
    }
    ComputeBuffers();
    std::vector<float> newVerticies = {};
    std::vector<unsigned int> newIndicies = {};
    for (const auto& vert : vertices) {
        newVerticies.push_back(vert.first.x);
        newVerticies.push_back(vert.first.y);
        newVerticies.push_back(vert.first.z);
        newVerticies.push_back(vert.first.td1);
        newVerticies.push_back(vert.first.td2);
        newIndicies.push_back(vert.second);

    }
    rhsverticies = newVerticies;
    for (auto& i : newIndicies) {
        std::cout << i << " ";
    }
    return this->indices;
}

void IndexCalculator::ComputeBuffers()
{
    for (int i = 0; i < input.size(); i++)
    {
        std::set<VPair>::iterator it = vertices.find(std::make_pair(input[i], 0/*this value doesn't matter*/));
        if (it != vertices.end()) indices.push_back(it->second);
        else
        {
            vertices.insert(std::make_pair(input[i], index));
            indices.push_back(index++);
        }
    }

    // Notice that the vertices in the set are not sorted by the index
    // so you'll have to rearrange them like this:
    vbuffer.resize(vertices.size());
    for (std::set<VPair>::iterator it = vertices.begin(); it != vertices.end(); it++)
        vbuffer[it->second] = it->first;
}