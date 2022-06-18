//
//功能：nvm对应column_family
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

//#include "db/version_set.h"
#include "table/merging_iterator.h"

#include "common.h"
#include "rocksdb/nvm_option.h"
#include "persistent_sstable.h"
#include "my_log.h"
#include "sstable_meta.h"
#include "column_compaction.h"
#include "keys_merge_iterator.h"
#include "trie.h"


namespace rocksdb {
struct FileEntry;
class SstableMetadata;
class VersionStorageInfo;
class Trie;

class NvmCfModule {
 public:
  NvmCfModule(NvmCfOptions* nvmcfoption,const std::string &cf_name,uint32_t cf_id,const InternalKeyComparator* icmp);
  ~NvmCfModule();
  void Delete();
  bool AddL0TableRoom(uint64_t filenum,char** raw,FileEntry** file);
  // 返回 ptr_sstable 条带的大小
  uint64_t GetSstableEachSize() { return ptr_sst_->GetEachSize(); }
  void UpdateKeyNext(FileEntry* file) { sst_meta_->UpdateKeyNext(file); }
  int UpdateCompactionState(std::vector<FileMetaData*>& L0files, int compaction_files_num, int waitbanlanced) { 
    return sst_meta_->UpdateCompactionState(L0files, compaction_files_num, waitbanlanced); 
  }

  ColumnCompactionItem* PickZoneCompaction(VersionStorageInfo* vstorage);
  ColumnCompactionItem* PickColumnCompaction(VersionStorageInfo* vstorage);
  const InternalKeyComparator* GetInternalKeyComparator() { return icmp_; }
  double GetCompactionScore();

  char* GetIndexPtr(int index) { return ptr_sst_->GetIndexPtr(index); }

  void DeleteL0file(uint64_t filenumber);
  void DeleteColumnCompactionFile(uint64_t filenumber) {  sst_meta_->DeleteCompactionFile(filenumber); }

  bool Get(VersionStorageInfo* vstorage,Status *s,const LookupKey &lkey,std::string *value);
  bool Get_hash(VersionStorageInfo* vstorage,Status *s,const LookupKey &lkey,std::string *value);
  void AddIterators(VersionStorageInfo* vstorage,MergeIteratorBuilder* merge_iter_builder);

  FileEntry* FindFile(uint64_t filenumber,bool forward = true,bool have_error_print = true) { return sst_meta_->FindFile(filenumber,forward,have_error_print); }
  const NvmCfOptions* GetNvmCfOptions() { return nvmcfoption_; }

  Trie* GetTrie() { return trie; }

  void setwait(){nvmcfoption_->waitbanlanced = 1;}
  int getwait(){return nvmcfoption_->waitbanlanced;};

  FileEntry* GetFile(uint64_t index){return sst_meta_->files_.at(index);}
  uint64_t GetFileNum(){return sst_meta_->files_.size();}

 private:
  bool UserKeyInRange(Slice *user_key,InternalKey *start,InternalKey *end);
  int UserKeyCompareRange(Slice *user_key,InternalKey *start,InternalKey *end);
  bool BinarySearchInFile(FileEntry* file,int first_key_index,Slice *user_key,int *find_index,int *pre_left = nullptr,int *pre_right = nullptr);
  bool GetValueInFile(FileEntry* file,int find_index,std::string *value);
  int UserKeyCompare(Slice *user_key,InternalKey *t_key);

  NvmCfOptions* nvmcfoption_;
  SstableMetadata* sst_meta_;
  PersistentSstable* ptr_sst_;
  // 前缀树
  Trie* trie;
  
  const InternalKeyComparator* icmp_;
  int updatestate = 0;
};

NvmCfModule* NewNvmCfModule(NvmCfOptions* nvmcfoption,const std::string &cf_name,uint32_t cf_id,const InternalKeyComparator* icmp);
}  // namespace rocksdb