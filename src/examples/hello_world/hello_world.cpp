/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 Segfault by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include "common/examplebase.h"

namespace {

using segfault::examples::ExampleBase;
using segfault::examples::ExampleConfig;

//-------------------------------------------------------------------------------------------------
/// @class HelloWorld
/// @brief The minimal Segfault example: open a window and clear the frame every loop.
//-------------------------------------------------------------------------------------------------
class HelloWorld final : public ExampleBase {
public:
    HelloWorld() :
            ExampleBase(ExampleConfig{"hello_world", "hello, world!", 50, 50, 800, 600, false}) {
        // empty
    }
};

} // namespace

int main(int argc, char *argv[]) {
    HelloWorld example;
    return example.run(argc, argv);
}
