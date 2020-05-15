#include <common/genome.h>

#include <boost/format.hpp>
#include <fstream>

static const std::streamsize BUF_SIZE = 102400;

Genome loadGenomeFromPlainFile(const std::string &filename) {
  std::ifstream f(filename.c_str(), std::ifstream::in);
  char *buffer = new char[BUF_SIZE];

  if (f.fail()) {
    throw std::runtime_error(
        str(boost::format("Could not open %s") % filename));
  }

  f.seekg(0, f.end);
  std::streamsize size = f.tellg();
  f.seekg(0, f.beg);

  Genome g(filename, size);

  size_t pairsCur = 0;
  while (f.good()) {
    f.read(buffer, BUF_SIZE);
    std::streamsize n = f.gcount();
    for (std::streamsize i = 0; i < n; i++) {
      switch (buffer[i]) {
        case 'A':
        case 'a':
          g.pairs[pairsCur] = Basepair::A;
          pairsCur++;
          break;
        case 'C':
        case 'c':
          g.pairs[pairsCur] = Basepair::C;
          pairsCur++;
          break;
        case 'G':
        case 'g':
          g.pairs[pairsCur] = Basepair::G;
          pairsCur++;
          break;
        case 'T':
        case 't':
          g.pairs[pairsCur] = Basepair::T;
          pairsCur++;
          break;
        case ' ':
        case '\n':
        case '\r':
        case '\t':
          break;
        default:
          throw std::runtime_error(
              str(boost::format("Unexpected basepair in %s at %d: %c")
                  % filename % i % buffer[i]));
      }
    }
  }
  return g;
}

std::ostream &operator<<(std::ostream &os, const Genome &g) {
  return os << "Genome " << g.description << " has " << g.pairs.size()
            << " pairs";
}

StringFrequency Genome::stringFrequency(size_t n) const {
  return StringFrequency(*this, n);
}

StringFrequency::StringFrequency(const Genome &g, size_t n) : n_(n) {
  // Compute a map from string to counts of that string.
  // Every string also has a complement (reversed and each nucleotide
  // replaced with its complement).  The string and its complement
  // function the same, so only count one.
  //
  // 1) If we've already recorded the string or its complement, then
  //    increment that record.
  //
  // 2) Else, prefer the version that is lexicographically earlier.
  auto inc = [=](const Basepairs::const_iterator &begin) {
    BPString s(begin, n);
    auto v = map_.find(s);
    if (v != map_.end()) {
      v->second += 1;
    } else {
      // Try to increment the complement instead.
      BPString c = s.complement();
      auto vv = map_.find(c);
      if (vv != map_.end()) {
        vv->second += 1;
      } else {
        if (s < c) {
          map_[s] = 1;
        } else {
          map_[c] = 1;
        }
      }
    }
  };

  Basepairs::const_iterator cur_begin = g.pairs.begin();
  for (size_t remaining = g.pairs.size() + 1 - n; remaining > 0;
       --remaining, ++cur_begin) {
    inc(cur_begin);
  }

  // Create a vector of entries in the map, sorted by frequency, so
  // we can easily see the max/min.
  list_.reserve(map_.size());
  std::for_each(map_.begin(), map_.end(), [=](const Map::value_type &i) {
    list_.push_back(std::pair<BPString, size_t>(i.first, i.second));
  });
  std::sort(list_.begin(), list_.end(),
            [](const std::pair<BPString, size_t> a,
               const std::pair<BPString, size_t> b) {
              return (a.second < b.second);
            });
}

std::vector<size_t> Genome::findString_(const BPString &needle,
                                        bool alsoComplement) const {
  std::vector<size_t> r;
  const BPString needleC = needle.complement();

  auto cmp = [=](size_t cur, const BPString &rhs) {
    if (needle.size() + cur > pairs.size()) {
      // Would run past
      return false;
    }
    for (size_t i = 0; i < rhs.size(); i++) {
      if (pairs[cur + i] != rhs[i]) {
        return false;
      }
    }
    return true;
  };

  for (size_t i = 0; i < pairs.size(); i++) {
    if (alsoComplement) {
      if (cmp(i, needle) || cmp(i, needleC)) {
        r.push_back(i);
      }
    } else {
      if (cmp(i, needle)) {
        r.push_back(i);
      }
    }
  }
  return r;
}

std::ostream &operator<<(std::ostream &os, const StringFrequency &fm) {
  size_t topN = 10;
  const StringFrequency::List &list = fm.list();
  auto printLine = [&os, &list](size_t i) {
    os << "[" << list[i].first.toString() << "/"
       << list[i].first.complement().toString() << "]: " << list[i].second
       << std::endl;
  };
  if (2 * topN - 1 < list.size()) {
    os << "Top/Bottom " << topN << " strings (total " << list.size() << ")"
       << std::endl;
    for (size_t i = 0; i < topN; i++) {
      printLine(i);
    }
    os << "--" << std::endl;
    for (size_t i = list.size() - topN; i < list.size(); i++) {
      printLine(i);
    }
  } else {
    os << "All " << list.size() << " strings" << std::endl;
    for (size_t i = 0; i < list.size(); i++) {
      printLine(i);
    }
  }
  return os;
}
