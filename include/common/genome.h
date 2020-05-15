#pragma once

#include <string.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum class Basepair : std::uint8_t { A, C, G, T };

inline Basepair toBasepair(const char bp) {
  switch (bp) {
    case 'A':
      return Basepair::A;
    case 'C':
      return Basepair::C;
    case 'G':
      return Basepair::G;
    case 'T':
      return Basepair::T;
    default:
      throw std::invalid_argument("Basepair not A, C, G or T");
  }
}

inline char toChar(const Basepair &bp) {
  switch (bp) {
    case Basepair::A:
      return 'A';
    case Basepair::C:
      return 'C';
    case Basepair::G:
      return 'G';
    case Basepair::T:
      return 'T';
    default:
      throw std::invalid_argument("Basepair not A, C, G or T");
  }
}

typedef std::vector<Basepair> Basepairs;
class BPString : public Basepairs {
public:
  BPString(const char *bpString) : Basepairs(strlen(bpString)) {
    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] = toBasepair(bpString[i]);
    }
  }
  BPString(const Basepairs::const_iterator &begin, size_t n) : Basepairs(n) {
    Basepairs::const_iterator j = begin;
    for (size_t i = 0; i < n; ++i, ++j) {
      (*this)[i] = *j;
    }
  }

  struct Hasher {
    std::size_t operator()(const BPString &k) const {
      std::size_t out = 0;
      std::for_each(k.begin(), k.end(), [&out](const Basepair &b) {
        out ^= (out << 2 | static_cast<uint8_t>(b));
      });
      return out;
    }
  };

  std::string toString() const {
    std::string s(this->size(), '\0');
    size_t j = 0;
    for (Basepairs::const_iterator i = this->begin(); i != this->end();
         i++, j++) {
      s[j] = toChar(*i);
    }
    return s;
  }

  BPString complement() const {
    BPString s = BPString(this->toString().c_str());
    for (size_t i = 0; i < this->size(); i++) {
      Basepair complement;
      switch ((*this)[i]) {
        case Basepair::A:
          complement = Basepair::T;
          break;
        case Basepair::C:
          complement = Basepair::G;
          break;
        case Basepair::G:
          complement = Basepair::C;
          break;
        case Basepair::T:
          complement = Basepair::A;
          break;
        default:
          throw std::invalid_argument("Basepair not A, C, G or T");
      }
      s[this->size() - 1 - i] = complement;
    }
    return s;
  }

  bool operator<(const BPString &rhs) const {
    for (size_t i = 0; i < this->size(); i++) {
      if (i >= rhs.size()) {
        // LHS is longer than RHS, but prefix matches RHS,
        // so RHS sorts lexicographically earlier ("book" < "bookworm")
        return false;
      }
      if ((*this)[i] < rhs[i]) {
        return true;
      }
      if ((*this)[i] != rhs[i]) {
        return false;
      }
    }
    // LHS == RHS
    return false;
  }

  bool operator==(const BPString &rhs) const {
    if (this->size() != rhs.size()) {
      return false;
    }
    for (size_t i = 0; i < this->size(); i++) {
      if ((*this)[i] != rhs[i]) {
        return false;
      }
    }
    return true;
  }
};

class Genome;
class StringFrequency {
public:
  typedef std::unordered_map<BPString, size_t, BPString::Hasher> Map;
  typedef std::vector<std::pair<BPString, size_t>> List;

  StringFrequency(const Genome &g, size_t n);

  const Map &map() const { return map_; }
  const List &list() const { return list_; }
  size_t N() const { return n_; }

private:
  size_t n_;
  Map map_;
  List list_;
};

std::ostream &operator<<(std::ostream &os, const StringFrequency &fm);

class Genome {
public:
  Genome(const std::string &description, const Basepairs &bps)
      : description(description), pairs(bps){};
  Genome(const std::string &description, size_t bpSize)
      : description(description), pairs(bpSize){};

  StringFrequency stringFrequency(size_t n) const;

  std::vector<size_t> findString(const BPString &needle) const {
    return findString_(needle, false);
  }
  std::vector<size_t> findStringOrComplement(const BPString &needle) const {
    return findString_(needle, true);
  }

  std::string description;
  Basepairs pairs;

private:
  std::vector<size_t> findString_(const BPString &needle,
                                  bool alsoComplement) const;
};

std::ostream &operator<<(std::ostream &os, const Genome &g);

Genome loadGenomeFromPlainFile(const std::string &filename);
