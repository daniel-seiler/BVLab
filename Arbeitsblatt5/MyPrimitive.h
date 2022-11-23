#include <vector>

struct CMyPoint {
    int m_x;
    int m_y;

    CMyPoint() { m_x = 0; m_y = 0 };
    CMyPoint(int x , int y) { m_x = x; m_y = y; };
    CMyPoint(const CMyPoint& toCopy) { m_x = toCopy.m_x ; m_y = toCopy.m_y; };
};

class CMyPrimitive {
public:
    std::vector<CMyPoint> m_points;

    CMyPrimitive(void);
    ~CMyPrimitive();

    void Reset() { m_points.empty(); };
    void Resize(int size) { m_points.resize(size); };
    void Append(int x, int y);
};