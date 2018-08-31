#include <stdio.h>
#include <fllib.h>

#include "types.h"
#include "lexer.h"
#include "parser.h"
#include "file.h"

struct SbsFile* sbs_file_parse(const char *filename)
{
    const char *source = (const char*)fl_io_file_read_all_text(filename);

    if (source == NULL)
        return NULL;

    struct SbsFile *file = fl_malloc(sizeof(struct SbsFile));
    
    file->filename = fl_cstring_replace(filename, "\\", "/");

    struct SbsLexer lexer = sbs_lexer_new(source, strlen(source));
    
    if (!sbs_parser_parse(&lexer, file))
    {
        fl_free(file);
        file = NULL;
    }

    fl_cstring_delete((char*)source);

    return file;
}

void sbs_file_delete(struct SbsFile *file)
{
    if (file->filename)
        fl_cstring_delete(file->filename);

    if (file->presets)
        fl_hashtable_delete(file->presets);

    if (file->envs)
        fl_hashtable_delete(file->envs);

    if (file->targets)
        fl_hashtable_delete(file->targets);

    if (file->toolchains)
        fl_hashtable_delete(file->toolchains);

    if (file->configurations)
        fl_hashtable_delete(file->configurations);

    if (file->actions)
        fl_hashtable_delete(file->actions);

    fl_free(file);
}
