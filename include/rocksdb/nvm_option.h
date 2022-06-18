//
//
//
#pragma once

#include <memory>
#include <string>

namespace rocksdb {

  static const double Beyond_this_delay_column_compaction = 4;  //当其它层(除了L0层)的数据量/阈值比值超过这个值时，其它层compaction优先
  static const uint64_t Level0_column_compaction_limited_size = 1ul * 1024 * 1024 * 1024;  //防止L0全部刷写到L1
  
struct NvmSetup {
  bool use_nvm_module = false;

  bool reset_nvm_storage = false;

  std::string pmem_path;  //目录

  //uint64_t pmem_size = 0;  //主管理模块大小

  uint64_t Level0_column_compaction_trigger_size = 3ul * 1024 * 1024 * 1024; //3G trigger
  uint64_t Level0_column_compaction_slowdown_size = 3ul * 1024 * 1024 * 1024 + 512ul * 1024 * 1024; //3.5G slowdown
  uint64_t Level0_column_compaction_stop_size = 4ul * 1024 * 1024 * 1024; //4G stop

  int Column_compaction_no_L1_select_L0 = 64;     //column compaction时没有L1文件交集时,至少选择L0数据量进行column compaction的文件个数
  int Column_compaction_have_L1_select_L0 = 64;   //column compaction时有L1文件交集时,至少选择L0数据量进行column compaction的文件个数

  NvmSetup& operator=(const NvmSetup& setup) = default;
  NvmSetup() = default;
};

struct NvmCfOptions {
  NvmCfOptions() = delete;

  NvmCfOptions(const std::shared_ptr<NvmSetup> setup,uint64_t s_write_buffer_size,int s_max_write_buffer_number,int s_level0_stop_writes_trigger,uint64_t s_target_file_size_base);

  ~NvmCfOptions() {}

  bool use_nvm_module;
  bool reset_nvm_storage;
  std::string pmem_path;
  //uint64_t cf_pmem_size;

  uint64_t Level0_column_compaction_trigger_size = 3ul * 1024 * 1024 * 1024; //3G trigger
  uint64_t Level0_column_compaction_slowdown_size = 3ul * 1024 * 1024 * 1024 + 512ul * 1024 * 1024; //3.5G slowdown
  uint64_t Level0_column_compaction_stop_size = 4ul * 1024 * 1024 * 1024; //4G stop

  int Column_compaction_no_L1_select_L0 = 64;     //column compaction时没有L1文件交集时,至少选择L0数据量进行column compaction的文件个数
  int Column_compaction_have_L1_select_L0 = 16;   //column compaction时有L1文件交集时,至少选择L0数据量进行column compaction的文件个数

  int waitbanlanced = 0;  //是否进行平衡

  uint64_t Level0_compaction_limited_size = 512ul * 1024 * 1024; //L0合并参与数据量
  int limited_size_state = 1;
  int zones_num = 8;  //compaction的挑选文件数

  uint64_t write_buffer_size;
  int max_write_buffer_number;
  int level0_stop_writes_trigger;
  uint64_t target_file_size_base;

};

}  // namespace rocksdb