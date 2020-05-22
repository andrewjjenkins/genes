#pragma once
#include <exception>

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


