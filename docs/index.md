# ExpressCPP

| Current Version | Code Name |
|---|---|
| v0.1.0 | Arborvitae |

## Installation

At this time, only Windows is supported. We recommend to use ExpressCPP together with [Visual Studio](https://www.visualstudio.com/).

Download the necessary files (one `.lib` and some `.h` files) from the [release](https://github.com/AlephTeam/ExpressCPP/releases) page.

Create or use existing C++ project. Right click the project in solution view, and include the path to the header and lib files. Under `C/C++ > General > Additional Include Directories`, edit to add the path that contains the the header files. Similarly, under `Linker > General > Additional Library Directories`, edit to add the path that contains the lib files.

ExpressCPP requires `Ws2_32.lib` which should be already installed on your computer.

## Get Started

First, create a `Express` instance. We recommend use singleton pattern but it is not required.

``` cpp
#include "CExpress\express.h"

#ifdef _DEBUG
#pragma comment(lib, "CExpress\\express_debug.lib")
#else
#pragma comment(lib, "CExpress\\express_release.lib")
#endif  

int main() {
    CExpress::Express ex;
}
```

The above example, I put all the headers and lib files under directory `CExpress` that is at the same level as this code file. `_DEBUG` flag will be defined if you set "Debug" in the configuration manager.

>`using namespace CExpress` is ok if you know it does not create any ambiguity.

Then, we can create a controller:

``` cpp
ex.route("GET", "/", [=](Request & req, Response & res) {
  res.body = "<h1>Hello World</h1>";
  res.headers["Content-Type"] = "text/html; charset=utf-8";
});
```

>If the request url match multiple controllers, only the first one we find will be executed.

Lastly, start the server:

```cpp
ex.start(8080);
```

That's it! The complete code for hello world program is

```cpp
#include "CExpress\express.h"

#ifdef _DEBUG
#pragma comment(lib, "CExpress\\express_debug.lib")
#else
#pragma comment(lib, "CExpress\\express_release.lib")
#endif 

int main() {
    CExpress::Express ex;
    ex.route("GET", "/", [=](Request & req, Response & res) {
        res.body = "<h1>Hello World</h1>";
        res.headers["Content-Type"] = "text/html; charset=utf-8";
    });
    ex.start(8080);
}
```

>Note that you need to set the content type to html if you are passing back html to the client. Because you are taking reference of `Request` and `Response` it is unnecessary to return anything. Also, `ex.start(8080)` needs to be the last line of your program.

## What's Next

Create a middleware by

```cpp
ex.use([=](Request & req, Response & res) {
    // process the request and response here
});
```

>The execution of middleware is the same as your declaration order.

Crate an error handler like this:

```cpp
ex.onerror("404", [=](Request & req, Response & res) {
    // process error here
});
```

>If you define multiple error handlers for the same error code, the one that actually runs will be random. We are internally using `unordered_map`.

## Contributing

We love your [feedback](https://github.com/AlephTeam/ExpressCPP/issues). This repository is under [MIT license](https://github.com/AlephTeam/ExpressCPP/blob/master/LICENSE). Fork as you like 👍.

---

©️ 2017 The Aleph Team