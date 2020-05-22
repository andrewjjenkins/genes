#pragma once

#include <string.h>

#include <algorithm>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>

#include "basepair.h"
#include "gbuf.h"

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
