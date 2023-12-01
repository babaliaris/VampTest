# VampTest
#### A simple C testing framework for my VampEngine project.

## How to Compile:
From now on, I will reference the root directory of this repo using `<VampTestRepo>`

#### Using [Premake5](https://premake.github.io/):
Create a [premake5.lua workspace](https://premake.github.io/docs/Your-First-Script) and just include `<VampTestRepo>/projects/VampTest`.

Also, you have to link the *VampTest* project to your **ConsoleApp** project, since *VampTest* is compiled as a [StaticLib](https://premake.github.io/docs/kind/).

Also, **MAKE SURE** you add in your [includedirs](https://premake.github.io/docs/includedirs/) `<VampTestRepo>/projects/VampTest/src/include`. Every header inclusion is absolute to `VampTest`.
```lua
workspace "MyWorkspace"
  platforms "x64"
  configurations {"debug", "dist", "release"}

  --Replace <VampTestRepo> with the root directory of this repo in the location where you cloned it.
  include "<VampTestRepo>/projects/VampTest"

  --Link VampTest in your project.
  project "Tests"
    kind "ConsoleApp"
    ...
    includedirs {
      "<VampTestRepo>/projects/VampTest/src/include",
      other...
    }

    links {
      "VampTest",
      ...other
    }
```

#### Using your own IDE or compiler configurations:
This VampTest project has only two files:

`<VampTestRepo>/projects/VampTest/src/VampTest.c`

`<VampTestRepo>/projects/VampTest/src/include/VampTest/VampTest.h`

Just compile the .c file and DO NOT FORGET to specify the include directory: `<VampTestRepo>/projects/VampTest/src/include`
in your compiler or IDE settings, because the .c file includes the header file using absolute paths:
```C
//<VampTestRepo>/projects/VampTest/src/VampTest.c
#include <VampTest/VampTest.h>
```

## How to Use:
Simple Example Usage :
```C
//My main.c file
#include <VampTest/VampTest.h>

//This MUST be defined by you, or else you gonna get a link error.
void VampRunBeforeAllTests()
{
    printf("Starting Tests...\n\n");
}

//This MUST be defined by you, or else you gonna get a link error.
void VampRunAfterAllTests()
{
    printf("Finished running tests.\n\n");
}

VAMP_TEST(Suite1, Test1)
{
    VAMP_EXPECT(1 == 2, "1 should not be %s", "equal to 2") //This will FAIL the test.
}


VAMP_TEST(Suite1, Test2)
{
    VAMP_EXPECT(1 == 1, "1 should be equal to 1") //This will PASS the test
}


VAMP_TEST(Suite2, Test1)
{
    VAMP_EXPECT(1 == 1, "1 should be equal to 1") //This will PASS the test
}


VAMP_TEST_FRAMEWORK(
    VAMP_REGISTER_TEST(Suite1, Test123456789);
    VAMP_REGISTER_TEST(Suite1, Test2);
    VAMP_REGISTER_TEST(Suite2, Test1);
)
```

### Macros:
Use `VAMP_TEST(suite_name, test_name)` to define a test function. The combination of `suite_name` and `test_name` must be **unique** across the entire project!!!

Use `VAMP_TEST_FRAMEWORK(registers)` to initialize the testing framework **ONLY in one .c file!!!**

use `VAMP_REGISTER_TEST(suite_name, test_name)` **INSIDE** the `VAMP_TEST_FRAMEWORK(registers)` macro, to register a test. Unfortunately, you have to register for every test manually.

use `VAMP_DECLARE_TEST(suite_name, test_name)`  **BEFORE** the `VAMP_TEST_FRAMEWORK(registers)` macro in order to declare tests that have been defined in other .c files than the main.c file.

#### ASSERT Macros:
Use `VAMP_EXPECT(cond, ...)` **INSIDE** a test to assert it. If `cond` is `false`, the test will fail. The rest of the arguments are `printf()` formatted.

Example:
```C
VAMP_EXPECT(myFuncAdd(1, 2) == 2, "1 + 2 should be equal to 3. The value return from myFuncAdd(1, 2) = %d", myFuncAdd(1, 2)); //This will FAIL.
VAMP_EXPECT(myFuncAdd(1, 2) == 3, "1 + 2 should be equal to 3"); //This will PASS.
```


## How to Run:
Once you have compiled your project, just run the executable (ConsoleApp) that you have created.
This will run all the tests.

### How to run specific tests:
Use the `--filter` option as follows:

`./my_executable --filter SuiteName.Testname` This will run only ONE test, the test that you specified in the filter.

`./my_executable --filter SuiteName` This will run ALL the tests that share the SAME SuiteName.
