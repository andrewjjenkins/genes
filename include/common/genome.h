#pragma once

#include <string.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
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

inline Basepair complement(const Basepair &bp) {
  switch (bp) {
    case Basepair::A:
      return Basepair::T;
    case Basepair::C:
      return Basepair::G;
    case Basepair::G:
      return Basepair::C;
    case Basepair::T:
      return Basepair::A;
    default:
      throw std::invalid_argument("Basepair not A, C, G or T");
  }
}

class BPString;
class GBuf : public std::enable_shared_from_this<GBuf> {
public:
  typedef std::shared_ptr<GBuf> SharedPtr;
  typedef std::shared_ptr<const GBuf> SharedConstPtr;

  // Prefer the make* factories
  explicit GBuf(size_t len) : basepairs_(len) {}

  static SharedConstPtr makeFromString(const char *basepairs) {
    size_t len = strlen(basepairs);
    std::shared_ptr<GBuf> g = std::make_shared<GBuf>(len);
    for (size_t i = 0; i < len; i++) {
      g->basepairs_[i] = toBasepair(basepairs[i]);
    }
    return g;
  }

  static SharedConstPtr makeFromIfstream(std::ifstream *f);

  const Basepair &operator[](size_t offset) const {
    return basepairs_[offset];
  }

  BPString slice(size_t offset, size_t len) const;
  size_t size() const { return basepairs_.size(); }

private:
  std::vector<Basepair> basepairs_;
};

class BPString {
  friend class GBuf;

public:
  static BPString makeFromString(const char *basepairs) {
    GBuf::SharedConstPtr g(GBuf::makeFromString(basepairs));
    return g->slice(0, g->size());
  }

  size_t size() const { return len_; }

  Basepair operator[](const size_t offset) const {
    return isComplement_
               ? ::complement((*gbuf_)[(offset_ + len_ - 1 - offset)])
               : (*gbuf_)[(offset_ + offset)];
  }

  bool operator<(const BPString &rhs) const {
    for (size_t i = 0; i < size(); i++) {
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
    if (this->gbuf_ == rhs.gbuf_ && this->offset_ == rhs.offset_
        && this->len_ == rhs.len_
        && this->isComplement_ == rhs.isComplement_) {
      return true;
    }
    if (this->size() != rhs.size()) {
      return false;
    }
    for (size_t i = 0; i < size(); i++) {
      if ((*this)[i] != rhs[i]) {
        return false;
      }
    }
    return true;
  }

  struct Hasher {
    std::size_t operator()(const BPString &k) const {
      std::size_t out = 0;
      for (size_t i = 0; i < k.size(); i++) {
        out ^= (out << 2 | static_cast<uint8_t>(k[i]));
      };
      return out;
    }
  };

  std::string toString() const {
    std::string s(len_, '\0');
    for (size_t i = 0; i < size(); i++) {
      s[i] = toChar((*this)[i]);
    }
    return s;
  }

  BPString complement() const {
    BPString c(gbuf_, offset_, len_);
    c.isComplement_ = true;
    return c;
  }

private:
  BPString(const GBuf::SharedConstPtr &gbuf, size_t offset, size_t len)
      : gbuf_(gbuf), offset_(offset), len_(len), isComplement_(false) {}

  GBuf::SharedConstPtr gbuf_;
  size_t offset_;
  size_t len_;
  bool isComplement_;
};

inline BPString GBuf::slice(size_t offset, size_t len) const {
  return BPString(shared_from_this(), offset, len);
}

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
  Genome(const std::string &description, const GBuf::SharedConstPtr &bps)
      : description(description), pairs(bps){};

  size_t size() const { return pairs->size(); }
  BPString slice(size_t offset, size_t len) const {
    return pairs->slice(offset, len);
  }

  StringFrequency stringFrequency(size_t n) const;

  std::vector<size_t> findString(const BPString &needle) const {
    return findString_(needle, false);
  }
  std::vector<size_t> findStringOrComplement(const BPString &needle) const {
    return findString_(needle, true);
  }

  std::string description;
  GBuf::SharedConstPtr pairs;

private:
  std::vector<size_t> findString_(const BPString &needle,
                                  bool alsoComplement) const;
};

std::ostream &operator<<(std::ostream &os, const Genome &g);

Genome loadGenomeFromPlainFile(const std::string &filename);
