#pragma once

// I ended up not needing this class for the project,
// But I am leaving it here as a sample of some 
// template programming and unit tests
class AudioBuffer {
private:
    std::vector<double> mBufImpl;
public:
    AudioBuffer( size_t Size);

    size_t size() const noexcept;
    void pokeSample(size_t Index, double Value);
    double peekSample( size_t Index) const;

    // This template allows you get the audio out
    // in whatever unsigned integer precision you would like.
    // This is useful for supporting higher or lower bit-depth
    // audio DACs
    template< typename T>
    void getBuffer(T** Buf) const noexcept {
        constexpr double halfMax = 
            static_cast<double>(std::numeric_limits<T>::max() / 2); // [1.]

        for (int i = 0; i < mBufImpl.capacity(); ++i) {
            (*Buf)[i] = (i < mBufImpl.size()) ?
                static_cast<T>(mBufImpl[i] * halfMax + halfMax) :
                0;
        }
    }
};

// [1.]
// Want to see this work? Try this example program.
// If I'm unsure of some functionality of the language,
// I use online compilers to test things and prove them
// to myself.
//
// I built this in about 2 minutes on 
// https://www.onlinegdb.com/online_c++_compiler
//
//#include <iostream>
//
//using namespace std;
//
//template< typename T>
//T MyCounter() {
//    static T i = 0;
//    return i++;
//}
//
//int main()
//{
//    cout << "char: " << MyCounter<uint32_t>() << endl;
//    cout << "char: " << MyCounter<uint32_t>() << endl;
//    cout << "char: " << MyCounter<uint32_t>() << endl;
//    cout << "int:  " << MyCounter<int>() << endl;
//
//    return 0;
//}