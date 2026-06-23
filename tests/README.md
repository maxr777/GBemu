# Structure

There are two separate test runners: one for the CPU tests, and one for everything else.
This is because the CPU tests require a simplified memory layout, so I basically have to have a separate memory read/write functions for those only.  

The CPU tests are only ran in CI, since they take a couple of seconds to complete.
If you want to run the CPU tests locally, just uncomment the corresponding lines in the run script (or just run the ```cpu_test_runner``` file in ```tests\``` directory after running the build script).

# Sources

The CPU tests are from [here](https://github.com/adtennant/GameboyCPUTests) - I used the v2 version.  
