//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0),
    initial_bits(n_bits), msb(63),register_num(1 << n_bits), left_most_posi(0){
      register_m = std::vector<uint64_t>(register_num,0);
    }

/**
 * @brief Function that computes binary.
 *
 * @param[in] hashP
 * @returns binary of a given hash
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  // 将哈希值转换为二进制流P
  std::bitset<BITSET_CAPACITY> binary_s(hash);
  std::cout << "computeBinary : " << binary_s << std::endl;
  return binary_s;
}

/**
 * @brief Function that computes leading zeros.P
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  std::cout << "计算最左1函数 二进制流为：" << bset << std::endl;
  uint64_t pos = 0;
  for(int i=msb; i>=0; i--){
    pos++;
    if(bset[i] != 0) break;
  }

  return pos;
}

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  hash_t hash_value = CalculateHash(val);
  std::bitset<BITSET_CAPACITY> hash_bitset = ComputeBinary(hash_value);

  msb = hash_bitset.size()-1;
  std::cout << "MSB : " << msb << std::endl;
  std::bitset<BITSET_CAPACITY> tmp_bitset;
  for(int i=0;i<initial_bits;i++){
    tmp_bitset[i] = hash_bitset[msb-i];
    hash_bitset[msb-i] = 0;
  }
  msb -= initial_bits;
  std::cout << "更新MSB : " << msb << std::endl;


  std::cout << "提取前b位后的二进制流：" << hash_bitset << std::endl;
  std::cout << "提取前b位后的temppp流：" << tmp_bitset << std::endl;

  uint64_t index= tmp_bitset.to_ulong();
  std::cout << "index ：" << index << std::endl;

  left_most_posi = PositionOfLeftmostOne(hash_bitset);
  std::cout << "leftmostposi ：" << left_most_posi << std::endl << std::endl;
  register_m[index] = std::max(register_m[index], left_most_posi);

}

/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  double sum = 0;
  for(size_t j=0;j<register_num;j++){
    sum += 1.0 / (1UL << register_m[j]);
  }
  cardinality_ = CONSTANT * register_num * register_num / sum;
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
