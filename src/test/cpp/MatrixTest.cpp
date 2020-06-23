#include "util/Matrix.hpp"
#include "util/Log.hpp"
using namespace util;
using namespace std;

/*
 * Tests equality
 */
void testEquality() {
    Matrix<double> t2(3, 3);

    t2.setRow(0, 1.0, 2.0, 3.0);
    t2.setRow(1, 4.0, 5.0, 9.0);
    t2.setRow(2, 2.0, 1.0, 3.0);

    ASSERT(t2 == t2);

    Matrix<double> t1(3, 3);

    t1.setArray(0, 1.0, 2.0, 3.0, 4.0, 5.0, 9.0, 2.0, 1.0, 3.0);

    ASSERT(t1 == t1);

    Matrix<double> t3(3, 3);
    t3.set(0, 0, 1);
    t3.set(0, 1, 2);
    t3.set(0, 2, 3);

    t3.set(1, 0, 4);
    t3.set(1, 1, 5);
    t3.set(1, 2, 9);

    t3.set(2, 0, 2);
    t3.set(2, 1, 1.0);
    t3.set(2, 2, 3);

    ASSERT(t1 == t3);

    ASSERT(t1 == t2);
    ASSERT(!(t1 != t2));

    Matrix<double> t4(3, 3, 1.0, 2.0, 3.0, 4.0, 5.0, 9.0, 2.0, 1.0, 3.0);
    ASSERT(t4 == t1);
    ASSERT(t4 == t3);
    ASSERT(t1 == t1);

    ASSERT(t2 == t2);
    ASSERT(t1 == t1);
    ASSERT(!(t2 != t2));
    ASSERT(!(t1 != t1));
    ASSERT(t1 == t2);

    t1.set(0, 0, 0);
    ASSERT(t1 != t2);
    ASSERT(t1 == t1);
    ASSERT(t2 == t2);

    t2.set(0, 0, 0);
    ASSERT(t1 == t2);
}

/*
 * Tests determinant
 */
void testDeterminant() {
    Matrix<double> t(4, 4);
    t.setArray(0, 2.0, 1.0, 3.0, 4.0, 2.0, -1.0, -3.0, 4.0, 1.0, 0.0, 1.0, 2.0,
            -2.0, -1.0, 2.0, 1.0);
    ASSERT(t.determinant() == -20);

    Matrix<double> t2(3, 3);
    t2.setArray(0, 1.0, 2.0, 3.0, 4.0, 5.0, 9.0, 2.0, 1.0, 3.0);
    ASSERT(t2.determinant() == 0);
}

/*
 * Tests minor
 */
void testMinor() {
    Matrix<double> t2(3, 3);
    t2.setArray(0, 1.0, 2.0, 3.0, 4.0, 5.0, 9.0, 2.0, 1.0, 3.0);

    Matrix<double> t1 = t2.minor(1, 0);
    ASSERT(t1.height() == 2);
    ASSERT(t1.width() == 2);

    ASSERT(t1.at(0, 0) == 2);
    ASSERT(t1.at(0, 1) == 3);

    ASSERT(t1.at(1, 0) == 1);
    ASSERT(t1.at(1, 1) == 3);
}

/*
 * Tests transpose
 */
void testTranspose() {
    Matrix<double> t2(2, 3);
    t2.setArray(0, 1.0, 2.0, 3.0, 4.0, 5.0, 9.0);

    Matrix<double> t1 = t2.transpose();
    ASSERT(t1.height() == 3);
    ASSERT(t1.width() == 2);

    ASSERT(t1.at(0, 0) == 1);
    ASSERT(t1.at(0, 1) == 4);

    ASSERT(t1.at(1, 0) == 2);
    ASSERT(t1.at(1, 1) == 5);

    ASSERT(t1.at(2, 0) == 3);
    ASSERT(t1.at(2, 1) == 9);
}

/*
 * Tests adjoint
 */
void testAdjoint() {
    Matrix<double> t2(3, 3);
    t2.setArray(0, 1.0, 2.0, 3.0, 4.0, 5.0, 9.0, 2.0, 1.0, 3.0);

    Matrix<double> t1 = t2.adjoint();
    // TODO: Check if this is correct...

}

/*
 * Tests inverse
 */
void testInverse() {
    Matrix<double> t(4, 4);
    t.setArray(0, 2.0, 1.0, 3.0, 4.0, 2.0, -1.0, -3.0, 4.0, 1.0, 0.0, 1.0, 2.0,
            -2.0, -1.0, 2.0, 1.0);

    Matrix<double> t2 = t.inverse();

    ASSERT((t * t2) == Matrix<double>::identity(4));
    ASSERT((t2 * t) == Matrix<double>::identity(4));
}

/*
 * Tests multiplication
 */
void testMultiplication() {
    Matrix<double> t(2, 4);
    t.setArray(0, 1.0, 2.0, 0.0, 0.0, 0.0, -2.0, 3.0, 4.0);

    Matrix<double> t2(4, 3);
    t2.setArray(0, 1.0, -2.0, 1.0, 3.0, 4.0, -1.0, 2.0, -3.0, 1.0, 2.0, 1.0,
            -1.0);

    Matrix<double> t3(t * t2);
    ASSERT(t3.at(0, 0) == 7);
    ASSERT(t3.at(0, 1) == 6);
    ASSERT(t3.at(0, 2) == -1);
    ASSERT(t3.at(1, 0) == 8);
    ASSERT(t3.at(1, 1) == -13);
    ASSERT(t3.at(1, 2) == 1);
}

int main(int argc, char **argv) {
    cout << "Beginning matrix test" << endl;
    cout << "-------------------------" << endl;

    // Test equality
    cout << "Equality ............. ";
    testEquality();
    cout << "ok" << endl;

    // Test multiplication
    cout << "Multiplication ....... ";
    testMultiplication();
    cout << "ok" << endl;

    // Test determinant
    cout << "Determinant .......... ";
    testDeterminant();
    cout << "ok" << endl;

    // Test minor
    cout << "Minor ................ ";
    testMinor();
    cout << "ok" << endl;

    // Transpose
    cout << "Transpose ............ ";
    testTranspose();
    cout << "ok" << endl;

    // Adjoint
    cout << "Adjoint .............. ";
    testAdjoint();
    cout << "ok" << endl;

    // Inverse
    cout << "Inverse .............. ";
    testInverse();
    cout << "ok" << endl;

    cout << "-------------------------" << endl;
    cout << "All tests passed!" << endl;

    return 0;
}

