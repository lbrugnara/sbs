#include <stdio.h>
#include <fllib/IO.h>

#include "../Test.h"
#include "tests.h"
#include "../../src/lang/file.h"

void sbs_test_integration_basic(void)
{
    const char *filename = "./basic.sbs";
    const char *sbs = "env basic {}";
    
    flm_assert(fl_io_file_write_all_text(filename, sbs), "Test setup");
    
    SbsFile *file = sbs_file_parse(filename);
    flm_assert(file != NULL, "Basic build file parses without errors");
    sbs_file_free(file);

    fl_io_file_unlink(filename);
}
