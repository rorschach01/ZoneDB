#! /bin/sh

db="/home/ymh/db_data"
wal_dir="/mnt/pmem_ymh/wal"
#bench_level0_file_path="/pmem/ceshi"
#level0_file_path=""
value_size="4096"

pmem_path="/mnt/pmem_ymh/ycsb_nvm"
use_nvm="true"

const_params=""

COPY_OUT_FILE(){
    mkdir $bench_file_dir/result > /dev/null 2>&1
    res_dir=$bench_file_dir/result/value-$value_size
    #res_dir=$bench_file_dir/result/ycsb-value-$value_size
    mkdir $res_dir > /dev/null 2>&1
    \rm -r $res_dir/*
    \cp -f $bench_file_dir/compaction.csv $res_dir/
    \cp -f $bench_file_dir/OP_DATA $res_dir/
    \cp -f $bench_file_dir/OP_TIME.csv $res_dir/
    \cp -f $bench_file_dir/out.out $res_dir/
    \cp -f $bench_file_dir/Latency.csv $res_dir/
    \cp -f $bench_file_dir/PerSecondLatency.csv $res_dir/
    \cp -f $db/OPTIONS-* $res_dir/
    #\cp -f $db/LOG $res_dir/
}

bench_file_path="$(dirname $PWD )/db_bench"
bench_file_dir="$(dirname $PWD )/ZoneDB"

COPY_OUT_FILE