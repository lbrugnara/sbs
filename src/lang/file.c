#include <stdio.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>

#include "file.h"
#include "../io.h"
#include "../utils.h"

/*
 * Function: sbs_file_free
 *  Releases the memory used by the *file* object.
 *
 * Parameters:
 *  file - Object to release its memory
 *
 * Returns:
 *  void - This function does not return a value
 */
void sbs_file_free(SbsFile *file)
{
    if (file->filename)
        fl_cstring_free(file->filename);

    // Hashtables should deallocate its key-value objects'
    // memory. Check the init_file function in the parser.c file.
    if (file->presets)
        fl_hashtable_free(file->presets);

    if (file->envs)
        fl_hashtable_free(file->envs);

    if (file->targets)
        fl_hashtable_free(file->targets);

    if (file->toolchains)
        fl_hashtable_free(file->toolchains);

    if (file->configurations)
        fl_hashtable_free(file->configurations);

    if (file->actions)
        fl_hashtable_free(file->actions);

    fl_free(file);
}
