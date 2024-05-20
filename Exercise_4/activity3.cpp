#include <iostream>

// Define namespace Complex
namespace Complex {
    // Define class complex
    class complex {
    private:
        int a, b; // Private integer members

    public:
        // Constructor with zero arguments, default values for a and b set to 1
        complex() : a(1), b(1) {}

        // Constructor with two arguments to initialize a and b
        complex(int x, int y) : a(x), b(y) {}

        // Public member functions to return the values of a and b
        int geta() { return a; } // Returning 'a'
        int getb() { return b; } // Returning 'b'
    };
}

// Main function
int main() {
    Complex::complex c1;

    Complex::complex c2(5, 10);

    std::cout << "Complex number 1: " << c1.geta() << " + " << c1.getb() << "i\n";
    std::cout << "Complex number 2: " << c2.geta() << " + " << c2.getb() << "i\n";

    return 0;
}
