#! /bin/bash

bench_db_path="/home/ymh/db_data"
#bench_db_path="/home/inspurssd/ymh"
wal_dir="/mnt/pmem_ymh/wal"
bench_value="16384"
bench_compression="none" #"snappy,none"

test_all_size=102400000000 #100G
read_all_size=10240000000 #10G

bench_benchmarks="fillrandom,stats" 
#bench_benchmarks="fillrandom,fillseq,readseq,readrandom,stats"
#bench_benchmarks="fillrandom,stats,readseq,readrandom,readrandom,readrandom,stats"
#bench_benchmarks="fillrandom,stats,wait,stats,readseq,readrandom,readrandom,readrandom,stats"
#bench_benchmarks="fillrandom,stats,wait,clean_cache,stats,readseq,readrandom,stats"
#bench_benchmarks="fillseq,stats"
#bench_benchmarks="fillrandom,stats,wait,stats,readrandom,stats"
#bench_benchmarks="fillrandom,stats"
#bench_benchmarks="fillrandom,stats,wait,stats,readseq,stats,readrandom,stats"
bench_num="25000000"
bench_readnum="0"
#bench_max_open_files="1000"

bench_num="`expr $test_all_size / $bench_value`"
bench_readnum="`expr $read_all_size / $bench_value`"

#线程数
max_background_jobs="3"

#控制 L1 层大小的参数
max_bytes_for_level_base="`expr 4 \* 1024 \* 1024 \* 1024`" 
level0_column_compaction_trigger_size="`expr 3 \* 1024 \* 1024 \* 1024`"    #3G
level0_column_compaction_slowdown_size="`expr 3 \* 1024 \* 1024 \* 1024 + 512 \* 1024 \* 1024`"    #3.5G
level0_column_compaction_stop_size="`expr 4 \* 1024 \* 1024 \* 1024`"       #4G

#max_bytes_for_level_base="`expr 256 \* 1024 \* 1024`" 
#level0_column_compaction_trigger_size="`expr 128 \* 1024 \* 1024`"   
#level0_column_compaction_slowdown_size="`expr 192 \* 1024 \* 1024`"   
#level0_column_compaction_stop_size="`expr 256 \* 1024 \* 1024`"  

#写停顿
level0_file_num_compaction_trigger="12"
level0_slowdown_writes_trigger="50"
level0_stop_writes_trigger="64"

pmem_path="/mnt/pmem_ymh/nvm"
use_nvm="true"

const_params="
    --db=$bench_db_path \
    --wal_dir=$wal_dir \
    --value_size=$bench_value \
    --benchmarks=$bench_benchmarks \
    --num=$bench_num \
    --reads=$bench_readnum \
    --compression_type=$bench_compression \
    --max_background_jobs=$max_background_jobs \
    --max_bytes_for_level_base=$max_bytes_for_level_base \
    --level0_file_num_compaction_trigger=$level0_file_num_compaction_trigger \
    --level0_slowdown_writes_trigger=$level0_slowdown_writes_trigger \
    --level0_stop_writes_trigger=$level0_stop_writes_trigger\
    --use_nvm_module=$use_nvm \
    --reset_nvm_storage=$use_nvm \
    --pmem_path=$pmem_path \
    --level0_column_compaction_trigger_size=$level0_column_compaction_trigger_size \
    --level0_column_compaction_slowdown_size=$level0_column_compaction_slowdown_size \
    --level0_column_compaction_stop_size=$level0_column_compaction_stop_size \
    --histogram \
    "

bench_file_path="$(dirname $PWD )/db_bench"

if [ ! -f "${bench_file_path}" ];then
bench_file_path="$PWD/db_bench"
fi

if [ ! -f "${bench_file_path}" ];then
echo "Error:${bench_file_path} or $(dirname $PWD )/db_bench not find!"
exit 1
fi

cmd="numactl --cpunodebind=1 --membind=1 $bench_file_path $const_params >>out.out 2>&1"

if [ -n "$1" ];then
cmd="nohup $bench_file_path $const_params >>out.out 2>&1 &"
echo $cmd >out.out
fi

echo $cmd
eval $cmd
