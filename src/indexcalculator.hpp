#pragma once
#include<set>
#include<vector>
#include<cmath>
#include <vector>

struct Vertex
{
    float x, y, z;
    float td1, td2;
};

constexpr float eps = 0.001;

using VPair = std::pair<Vertex, unsigned int>;

class IndexCalculator {
public:
	IndexCalculator() = default;
	~IndexCalculator() = default;
	std::vector<unsigned int> CalcIndex(std::vector<float>& verticies);
private:
    struct CmpClass // class comparing vertices in the set
    {
        bool operator() (const VPair& p1, const VPair& p2) const
        {
            if (fabs(p1.first.x - p2.first.x) > eps) return p1.first.x < p2.first.x;
            if (fabs(p1.first.y - p2.first.y) > eps) return p1.first.y < p2.first.y;
            if (fabs(p1.first.z - p2.first.z) > eps) return p1.first.z < p2.first.z;
            return false;
        }
    };
    void ComputeBuffers();

    std::vector<Vertex> input, vbuffer;
    std::set<VPair, CmpClass> vertices;
    std::vector<unsigned int> indices;
    int index = 0;
};