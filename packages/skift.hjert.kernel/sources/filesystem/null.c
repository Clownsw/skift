/* Copyright © 2018-2019 MAKER.                                               */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <string.h>

#include "kernel/system.h"
#include "kernel/filesystem.h"

int null_read(stream_t *s, void *buffer, uint size)
{
    UNUSED(s);
    UNUSED(size);
    UNUSED(buffer);

    return 0;
}

int null_write(stream_t *s, void *buffer, uint size)
{
    UNUSED(s);
    UNUSED(buffer);

    return size;
}

static device_t null;

void null_setup(void)
{
    null = (device_t){null_read, null_write, NULL};
    if (filesystem_mkdev("/Devices/null", null))
    {
        PANIC("Failled to create the 'null' device.");
    }
}