
#include <stdio.h>
#include <locale.h>
#include <time.h>

#include "Test.h"

// Tests
#include "lang/tests.h"
#include "integration/tests.h"

int main(int argc, char **argv) 
{
    fl_test_run_all_suites(
        argc,
        argv,
        fl_test_suite("Parser", 
            { "Basic",          &sbs_test_parser_basic          },
        ),
        fl_test_suite("Integration", 
            { "Basic",          &sbs_test_integration_basic     },
        ),
        NULL
    );
}
