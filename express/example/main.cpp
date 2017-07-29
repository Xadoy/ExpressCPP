#include <assert.h>
#include <iostream>
#include "../express/express.h"

using namespace CExpress;

void testRoute() {
  Route r1(CExpress::POST, "/user/:user/age/:age");
  Route r2(CExpress::POST, "/user/josh/age/19");
  Route r3(CExpress::POST, "/user/josh/age/19/");

  assert(r1.matches(r2)); // matches test
  assert(r2.matches(r3));
  assert(r1.matches(r3));

  assert(r1.matches(r1));
  assert(r2.matches(r2));
  assert(r3.matches(r3));

  Route r4(CExpress::PATCH, "/name/:name");
  Route r5(CExpress::GET, "/name/:name");
  Route r6(CExpress::GET, "/age/:age");

  assert(!r4.matches(r5)); // check request type test
  assert(!r5.matches(r6)); // different path string test

  Route r7(CExpress::CONNECT, "/case/cAsE/index.cshtml");
  Route r8(CExpress::CONNECT, "/case/CaSe/index.cshtml");

  assert(r7.matches(r8)); // case insensitive test

  Route r9(CExpress::CONNECT, "/age/:age");
  Route r10(CExpress::CONNECT, "/age/:age?id=123");

  assert(r9.matches(r10)); // ignore query params test

  Route r11(CExpress::CONNECT, "/age/:age");
  Route r12(CExpress::CONNECT, "/age/:age/");
  Route r13(CExpress::CONNECT, "/age/:age/id/:id");

  assert(r11.matches(r12)); // ignore trailing / test
  assert(!r12.matches(r13)); // different length test

  std::vector<std::string> exceptionTests = { "?", "", "//", "user/:user" };

  for (int i = 0; i < (int)exceptionTests.size(); ++i) {
    bool exceptionThrown = false;
    try {
      Route r7(CExpress::POST, exceptionTests[i]);
    }
    catch (char const* c) {
      exceptionThrown = true;
    }

    assert(exceptionThrown);
  }

  Route r14(CExpress::POST, "/book/user/id/902?shareid=299388301&token=1239dl-124sdf");
  Route r15(CExpress::POST, "/book/user/id/:id");
  Route r16(CExpress::POST, "/book/:user/id/:id");
  Route r17(CExpress::POST, "/book/tony/id/9020?shareid=299388301&token=1239dl-124sdf");

  assert(r14.matches(r15));
  assert(r15.matches(r14));
  assert(r14.matches(r16));
  assert(r16.matches(r14));
  assert(r16.matches(r17));
  assert(r17.matches(r16));

  Route r18(CExpress::POST, "/");
  Route r19(CExpress::POST, "/:id");
  Route r20(CExpress::POST, "/index?id=123");

  assert(r18.matches(r18));
  assert(r19.matches(r20));
  assert(r20.matches(r19));

  Route r21(CExpress::POST, "/user/book");
  Route r22(CExpress::POST, "/user/book?bookid=199283442");

  assert(r21.matches(r22));
  assert(r22.matches(r21));

  std::cout << "route unit test completed." << std::endl;
}

void testExpress() {
  CExpress::Express ex;
  ex.onerror("404", [=](Request & req, Response & res) {

  });
  ex.use([=](Request & req, Response & res) {

  });
  ex.route("GET", "/user/:userid", [=](Request & req, Response & res) {

  });
  ex.start(8080);
}

void testResponse() {
  CExpress::Response r;
  r.setBodyWithFile("./hello.html");
  assert(r.body == "<HTML>\n\
<HEAD>\n\
<META NAME=\"GENERATOR\" Content=\"Microsoft Visual Studio\">\n\
<TITLE></TITLE>\n\
</HEAD>\n\
<BODY>\n\
  <h1>Hello From ExpressCPP</h1>\n\
\n\
\n\
</BODY>\n\
</HTML>\n");

  try {
    r.setBodyWithFile("nonexist.html");
    // expected to jump to catch as "nonexist.html" does not
    // exists in the context
    assert(false);
  }
  catch (const char * e) {
    assert(true);
  }

  std::cout << "response unit test complete" << std::endl;
}

int main() {
  testRoute();
  testResponse();
  testExpress();
}
