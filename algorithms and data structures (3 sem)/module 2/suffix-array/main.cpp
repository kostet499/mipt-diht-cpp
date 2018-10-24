#include <iostream>
#include <vector>


const std::size_t ALPHABET_SIZE = 256;

class CSuffixArray
{
public:

  explicit CSuffixArray (const std::string &data) :
      data(data)
  {
    computePreliminaryCalculation();
  }

  void Reset (const std::string &data)
  {
    this->data = data;
    unique_substrings_cnt = -1;
    computePreliminaryCalculation();
  }

  int ComputeUniqueSubstrings ()
  {
    if (unique_substrings_cnt == -1)
    {
      computeUniqueSubstrings();
    }
    return unique_substrings_cnt;
  }

private:

  void computePreliminaryCalculation ()
  {
    data += '\0';
    computePermutations();

  }

  void computePermutations ()
  {
    std::vector<std::size_t> counter(ALPHABET_SIZE, 0);
    for (char character: data)
    {
      auto counter_idx = static_cast<std::size_t >(character);
      ++counter[counter_idx];
    }
    for (std::size_t i = 1; i < ALPHABET_SIZE; ++i)
    {
      counter[i] += counter[i - 1];
    }
    permutation.resize(data.size());
    for (std::size_t i = 0; i < ALPHABET_SIZE; ++i)
    {
      std::size_t permutation_idx = --counter[i];
      permutation[permutation_idx] = i;
    }
  }

  void computeEquivalenceClasses ()
  {
    std::size_t data_size = data.size();
    equivalence_class.emplace_back(std::vector<std::size_t>(data_size, 0));
    std::size_t classes_counter = 1;
    for (std::size_t i = 1; i < data_size; ++i)
    {
      if (data[permutation[i - 1]] != data[permutation[i]])
      {
        classes_counter++;
      }
      equivalence_class[0][permutation[i]] = classes_counter - 1;
    }
    std::vector<std::size_t> next_permutation_(data_size);
    for (std::size_t j = 0; (1 << j) < data_size; ++j)
    {
      for (std::size_t i = 0; i < data_size; ++i)
      {
        std::size_t next_permutation_value = permutation[i] + data_size - (1 << j);
        next_permutation_[i] = (next_permutation_value < data_size)
            ? next_permutation_value
            : next_permutation_value % data_size;
      }
      std::vector<std::size_t> counter(ALPHABET_SIZE, 0);
      for (std::size_t i = 0; i < data_size; ++i)
      {
        auto counter_idx = equivalence_class[j][next_permutation_[i]];
        ++counter[counter_idx];
      }
      for (std::size_t i = 1; i < classes_counter; ++i)
      {
        counter[i] += counter[i - 1];
      }
      for (std::size_t i = data_size - 1; (i + 1) >= 1; --i)
      {
        auto counter_idx = equivalence_class[j][next_permutation_[i]];
        auto permutation_idx = --counter[counter_idx];
        permutation[permutation_idx] = next_permutation_[i];
      }
      equivalence_class.emplace_back(std::vector<std::size_t>(data_size));
      classes_counter = 1;
      /*
       *
       *  TO-DO: ПОДСЧИТАТЬ НОВЫЕ КЛАССЫ ЭКВИВАЛЕНТНОСТИ
       *
       */
    }
  }

  void computeUniqueSubstrings ()
  {
    auto data_size = data.length();
    std::size_t j = 0;
    for (; (1 << j) < data_size; ++j);
    std::vector<std::size_t> largest_common_prefix(data_size, 0);
    for (int i = 1; i < data_size - 1; ++i)
    {
      auto curr_permutation = permutation[i];
      auto next_permutation_ = permutation[i + 1];
      for (std::size_t k = j - 1; (k + 1) >= 1; --k)
      {
        std::vector<std::size_t>& on_phase = equivalence_class[k];
        if (on_phase[curr_permutation] == on_phase[next_permutation_])
        {
          largest_common_prefix[i - 1] += 1 << k;
          curr_permutation += 1 << k;
          next_permutation_ += 1 << k;
        }
      }
    }
    unique_substrings_cnt = 0;
    for (std::size_t i = 1; i < data_size; ++i)
    {
      unique_substrings_cnt += data_size - 1 - permutation[i];
    }
    for (std::size_t i = 0; i <  data_size - 2; ++i)
    {
      unique_substrings_cnt -= largest_common_prefix[i];
    }
  }

private:

  int unique_substrings_cnt = -1;
  std::string data;
  std::vector<std::size_t> permutation;
  std::vector<std::vector<std::size_t>> equivalence_class;
};

int main ()
{
  std::string data;
  std::cin >> data;
  CSuffixArray suffix_array(data);
  std::cout << suffix_array.ComputeUniqueSubstrings() << std::endl;
  return 0;
}