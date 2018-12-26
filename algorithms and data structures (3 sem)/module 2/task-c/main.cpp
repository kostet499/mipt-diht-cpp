/*
 *
 *  ContestID: 13362149
 *
 */

#include <iostream>   // std::cin, std::cout
#include <vector>     // std::vector


const std::size_t CODE_TABLE_SIZE = 256;


class CSuffixArray
{
public:

  explicit CSuffixArray () = default;

  explicit CSuffixArray (std::string data);

  void Reset (const std::string &source);

  void ComputeLcp (std::vector<int32_t> &lcp) const;

  int32_t ComputeUniqueSubstrings ();

  std::size_t operator[] (std::size_t index) const;

private:

  std::size_t getEqClass (std::size_t phase_idx, std::size_t source_idx) const;

  void setEqClass (std::size_t phase_idx, std::size_t source_idx, int32_t eq_class);

  void addNewEqClass ();

  void build ();

  void computeUniqueSubstrings ();

  void computeEquivalenceClasses ();

  void computeZeroPhase (int32_t &classes_cnt);

  void computeCyclicShiftsSort (std::size_t phase_cnt, int32_t &classes_cnt);

private:

  int32_t unique_substrings_cnt = 0;
  std::string source;
  std::vector<std::size_t> permutation;
  std::vector<std::vector<std::size_t>> equivalence_classes;
};



void ShowKthCommon (const std::string &lhs, const std::string &rhs, int64_t kth_common);

std::pair<std::size_t, std::size_t> ComputeRangeOfKthCommon
    (const std::string &concated, std::size_t lhs_size, std::size_t rhs_size, int64_t kth_common);


int main ()
{
  std::string str_s, str_t;
  int64_t kth_common;
  std::cin >> str_s >> str_t >> kth_common;
  ShowKthCommon(str_s, str_t, kth_common);
  return 0;
}


void ShowKthCommon (const std::string &lhs, const std::string &rhs, int64_t kth_common)
{
  std::string concated = lhs + '!' + rhs;
  std::size_t lhs_size = lhs.size();
  std::size_t rhs_size = rhs.size();
  auto range = ComputeRangeOfKthCommon(concated, lhs_size, rhs_size, kth_common);
  if (range.second - range.first <= 0 || range.second == concated.size() + 2)
  {
    std::cout << -1;
  }
  else
  {
    for (std::size_t i = range.first; i < range.second; ++i)
    {
      std::cout << concated[i];
    }
  }
}

std::pair<std::size_t, std::size_t> ComputeRangeOfKthCommon
    (const std::string &concated, std::size_t lhs_size, std::size_t rhs_size, int64_t kth_common)
{
  CSuffixArray suffix_array(concated);
  std::vector<int32_t> lcp_values;
  suffix_array.ComputeLcp(lcp_values);
  int64_t current_lexical = 0;
  int64_t current_lcp = 0;
  int64_t previous_lcp = 0;
  auto is_from_different = \
    [lhs_size, suffix_array, concated] (std::size_t idx1, std::size_t idx2)
  {
    std::size_t shorter_suffix = std::min(suffix_array[idx1], suffix_array[idx2]);
    std::size_t longer_suffix = std::max(suffix_array[idx1], suffix_array[idx2]);
    return shorter_suffix < lhs_size && longer_suffix >= lhs_size + 1;
  };
  std::size_t concated_idx = 2;
  for (; concated_idx < (lhs_size + rhs_size + 1); ++concated_idx)
  {
    if (!is_from_different(concated_idx, concated_idx + 1))
    {
      current_lcp = std::min(
          current_lcp,
          static_cast<int64_t>(lcp_values[concated_idx])
      );
    }
    else
    {
      previous_lcp = current_lcp;
      current_lcp = lcp_values[concated_idx];
      if (previous_lcp >= current_lcp)
      {
        continue;
      }
      else
      {
        current_lexical += current_lcp - previous_lcp;
        if (current_lexical >= kth_common)
        {
          auto substr_len = static_cast<std::size_t>(kth_common + current_lcp - current_lexical);
          auto from = static_cast<std::size_t>(suffix_array[concated_idx]);
          auto to = from + substr_len;
          return std::make_pair(from, to);
        }
      }
    }
  }
  return std::make_pair(0, 0);
}


CSuffixArray::CSuffixArray (std::string data) :
    source(std::move(data))
{
  build();
}

void CSuffixArray::Reset (const std::string &source)
{
  this->source = source;
  unique_substrings_cnt = 0;
  build();
}

int32_t CSuffixArray::ComputeUniqueSubstrings ()
{
  // сохраняем результат вычисления кол-ва различных
  // строк во избежание избыточных повторных вычислений
  auto source_size = static_cast<int32_t>(source.size());
  if (unique_substrings_cnt == 0 && source_size != 1)
  {
    computeUniqueSubstrings();
  }
  if (source_size < 2)
  {
    unique_substrings_cnt = source_size;
  }
  return unique_substrings_cnt;
}

std::size_t CSuffixArray::operator[] (std::size_t index) const
{
  return permutation[index];
}

void CSuffixArray::ComputeLcp (std::vector<int32_t> &lcp) const
{
  auto data_size = source.length();
  lcp.resize(data_size - 1, 0);
  // вектор sorted_suffixes для того, чтобы знать положение
  // суффикса в отсортированном списке суффиксов
  std::vector<int32_t> sorted_suffixes(data_size, 0);
  for (int32_t i = 0; i < data_size; ++i)
  {
    sorted_suffixes[permutation[i]] = i;
  }
  int32_t lcp_value = 0;
  for (int32_t i = 0; i < data_size; ++i)
  {
    if (sorted_suffixes[i] == data_size - 1)
    {
      lcp_value = 0;
      continue;
    }
    auto j = permutation[sorted_suffixes[i] + 1];
    auto check_suffixes_matching = [&] () -> bool
    {
      bool ith_out_of_range = i + lcp_value >= data_size;
      bool jth_out_of_range = j + lcp_value >= data_size;
      if (ith_out_of_range || jth_out_of_range)
      {
        return false;
      }
      bool suffixes_matching = source[i + lcp_value] == source[j + lcp_value];
      return suffixes_matching;
    };
    for (; check_suffixes_matching(); ++lcp_value);
    lcp[sorted_suffixes[i]] = lcp_value;
    if (lcp_value > 0)
    {
      --lcp_value;
    }
  }
}

std::size_t CSuffixArray::getEqClass (std::size_t phase_idx, std::size_t source_idx) const
{
  return equivalence_classes[phase_idx][source_idx];
}

void CSuffixArray::setEqClass (std::size_t phase_idx, std::size_t source_idx, int32_t eq_class)
{
  equivalence_classes[phase_idx][source_idx] = static_cast<std::size_t>(eq_class);
}

void CSuffixArray::addNewEqClass ()
{
  equivalence_classes.emplace_back(
      std::vector<std::size_t>(source.size(), 0)
  );
}

void CSuffixArray::build ()
{
  source += '\0';
  computeEquivalenceClasses();
}

void CSuffixArray::computeUniqueSubstrings ()
{
  auto data_size = static_cast<int32_t>(source.length());
  int64_t answer = 1LL * data_size * (data_size - 1) / 2;
  std::vector<int32_t> lcp;
  ComputeLcp(lcp);
  for (std::size_t i = 0; i < data_size - 1; ++i)
  {
    answer -= lcp[i];
  }
  unique_substrings_cnt = static_cast<int32_t>(answer);
}

void CSuffixArray::computeEquivalenceClasses ()
{
  std::size_t source_size = source.size();
  int32_t classes_cnt = 1;
  computeZeroPhase(classes_cnt);
  for (std::size_t phase_cnt = 0; (1LL << phase_cnt) < source_size; ++phase_cnt)
  {
    computeCyclicShiftsSort(phase_cnt, classes_cnt);
  }
}

void CSuffixArray::computeZeroPhase (int32_t &classes_cnt)
{
  std::vector<std::size_t> counter(CODE_TABLE_SIZE, 0);
  for (char character: source)
  {
    auto counter_idx = static_cast<std::size_t>(character);
    ++counter[counter_idx];
  }
  for (std::size_t i = 1; i < CODE_TABLE_SIZE; ++i)
  {
    counter[i] += counter[i - 1];
  }
  std::size_t source_size = source.size();
  permutation.resize(source_size);
  for (std::size_t i = 0; i < source_size; ++i)
  {
    auto counter_idx = static_cast<std::size_t>(source[i]);
    std::size_t permutation_idx = --counter[counter_idx];
    permutation[permutation_idx] = i;
  }
  addNewEqClass();
  std::size_t phase_cnt = 0;
  for (std::size_t i = 1; i < source_size; ++i)
  {
    if (source[permutation[i - 1]] != source[permutation[i]])
    {
      ++classes_cnt;
    }
    setEqClass(phase_cnt, permutation[i], classes_cnt - 1);
  }
}

void CSuffixArray::computeCyclicShiftsSort (std::size_t phase_cnt, int32_t &classes_cnt)
{
  std::size_t source_size = source.size();
  std::vector<std::size_t> next_permutation(source_size, 0);
  for (std::size_t i = 0; i < source_size; ++i)
  {
    int64_t next_permutation_value = permutation[i] - (1LL << phase_cnt);
    if (next_permutation_value < 0)
    {
      next_permutation_value += source_size;
    }
    next_permutation[i] = static_cast<std::size_t>(next_permutation_value);
  }
  std::vector<std::size_t> counter(classes_cnt, 0);
  for (std::size_t i = 0; i < source_size; ++i)
  {
    auto counter_idx = getEqClass(phase_cnt, next_permutation[i]);
    ++counter[counter_idx];
  }
  for (std::size_t i = 1; i < classes_cnt; ++i)
  {
    counter[i] += counter[i - 1];
  }
  for (std::size_t i = source_size; i >= 1; --i)
  {
    auto counter_idx = getEqClass(phase_cnt, next_permutation[i - 1]);
    auto permutation_idx = --counter[counter_idx];
    permutation[permutation_idx] = next_permutation[i - 1];
  }
  addNewEqClass();
  setEqClass(phase_cnt + 1, permutation[0], 0);
  classes_cnt = 1;
  auto getposition = [&] (std::size_t idx) -> std::size_t
  {
    return (permutation[idx] + (1LL << phase_cnt)) % source_size;
  };
  for (std::size_t i = 1; i < source_size; ++i)
  {
    std::size_t first_substr_last_part = getposition(i);
    std::size_t second_substr_last_part = getposition(i - 1);
    // каждая циклическая подстрока длины 2^m является конкатенацией
    // двух других циклических подстрок длины 2^(m-1)
    bool substrings_matching = getEqClass(phase_cnt, permutation[i]) != \
          getEqClass(phase_cnt, permutation[i - 1]);
    substrings_matching |= getEqClass(phase_cnt, first_substr_last_part) != \
        getEqClass(phase_cnt, second_substr_last_part);
    if (substrings_matching)
    {
      ++classes_cnt;
    }
    setEqClass(phase_cnt + 1, permutation[i], classes_cnt - 1);
  }
}
