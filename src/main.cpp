#include "Matrix.hpp"
#include <iostream>

int main() {
    {
        Matrix<3, 3>
            m1 { 1.0, 0.0, 0.0,
                 0.0, 1.0, 0.0,
                 0.0, 0.0, 1.0 },
            m2 { 1.0, 0.0, 0.0,
                 0.0, 1.0, 0.0,
                 0.0, 0.0, 1.0 };

        m1 += m2;
        m1 + m2;
        std::cout << m1 << std::endl;
    }

    {
        Matrix<3, 1, double>
            m1 { 1.0, 2.0, 4.0 };
        Matrix<1, 3, double>
            m2 { 1.0, 2.0, 8.0 };

        auto m3 = m1 * m2;
        std::cout << m3 << std::endl;

        m3.reduce();
        std::cout << m3 << std::endl;

        Matrix<2, 2, double, true> m3subview(m3, 0, 0);
        std::cout << m3subview << std::endl;

        m3subview.data(0, 0) = 0;
        m3.data(1, 1) = 0;

        std::cout << m3subview << std::endl;
        std::cout << m3 << std::endl;
    }

}
