/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fuse_dfs.h"
#include "fuse_impls.h"
#include "fuse_users.h"
#include "fuse_connect.h"

int dfs_chmod(const char *path, mode_t mode)
{
  TRACE1("chmod", path)
  int ret = 0;
  dfs_context *dfs = (dfs_context*)fuse_get_context()->private_data;

  assert(path);
  assert(dfs);
  assert('/' == *path);

  hdfsFS userFS = doConnectAsUser(dfs->nn_uri, dfs->nn_port);
  if (userFS == NULL) {
    ERROR("Could not connect to HDFS");
    ret = -EIO;
    goto cleanup;
  }

  if (hdfsChmod(userFS, path, (short)mode)) {
    ERROR("Could not chmod %s to %d", path, (int)mode);
    ret = (errno > 0) ? -errno : -EIO;
    goto cleanup;
  }

cleanup:
  if (doDisconnect(userFS)) {
    ret = -EIO;
  }

  return ret;
}
