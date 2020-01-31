#pragma once

/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include "node/Node.h"

typedef struct FsHandle
{
    Lock lock;
    FsNode *node;
    size_t offset;
    OpenFlag flags;

    union {
        void *attached;
        Message *message;
    };

    size_t attached_size;
} FsHandle;

bool fshandle_has_flag(FsHandle *handle, OpenFlag flag);

FsHandle *fshandle_create(FsNode *node, OpenFlag flags);

void fshandle_destroy(FsHandle *handle);
FsHandle *fshandle_clone(FsHandle *handle);

bool fshandle_can_select(FsHandle *handle, SelectEvent events);
bool fshandle_is_locked(FsHandle *handle);
void fshandle_acquire_lock(FsHandle *handle, int who_acquire);
void fshandle_release_lock(FsHandle *handle, int who_release);

error_t fshandle_read(FsHandle *handle, void *buffer, size_t size, size_t *readed);
error_t fshandle_write(FsHandle *handle, const void *buffer, size_t size, size_t *written);

error_t fshandle_seek(FsHandle *handle, int offset, Whence whence);
error_t fshandle_tell(FsHandle *handle, Whence whence, int *offset);

error_t fshandle_call(FsHandle *handle, int request, void *args);
error_t fshandle_stat(FsHandle *handle, FileState *stat);

error_t fshandle_connect(FsNode *node, FsHandle **connection_handle);
error_t fshandle_accept(FsHandle *handle, FsHandle **connection_handle);
error_t fshandle_send(FsHandle *handle, Message *message);
error_t fshandle_receive(FsHandle *handle, Message *message);
error_t fshandle_payload(FsHandle *handle, Message *message);
error_t fshandle_discard(FsHandle *handle);
