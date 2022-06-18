//
//
//

#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <vector>


namespace rocksdb {

extern int nvm_file_exists(char const* file) ;

extern void nvm_dir_no_exists_and_create(const std::string& name);

}  // namespace rocksdb