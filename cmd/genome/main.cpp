#include <common/genome.h>

#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char** argv) {
  Genome g = loadGenomeFromPlainFile("./data/Vibrio_cholerae_ori.txt");
  std::cout << g << std::endl;
  for (size_t i = 3; i < 10; i++) {
    std::cout << "Frequency for " << i << ":" << std::endl
              << g.stringFrequency(i);
  }

  Genome gg = loadGenomeFromPlainFile("./data/Vibrio_cholerae.txt");
  std::cout << g << std::endl;
  const BPString dnaACandidate("ATGATCAAG");
  const std::vector<size_t> occurrences = gg.findString(dnaACandidate);
  std::cout << occurrences.size() << " occurrences of DnaA candidate "
            << dnaACandidate.toString()
            << " in Vibrio Cholerae:" << std::endl;
  for (size_t i = 0; i < occurrences.size(); i++) {
    if (i != 0) {
      std::cout << ", ";
    }
    std::cout << occurrences[i];
  }
  std::cout << std::endl;
  const std::vector<size_t> occurrencesC
      = gg.findStringOrComplement(dnaACandidate);
  std::cout << occurrencesC.size() << " occurrences of DnaA candidate "
            << dnaACandidate.toString() << " or complement "
            << dnaACandidate.complement().toString()
            << " in Vibrio Cholerae:" << std::endl;
  for (size_t i = 0; i < occurrencesC.size(); i++) {
    if (i != 0) {
      std::cout << ", ";
    }
    std::cout << occurrencesC[i];
  }
  std::cout << std::endl;

  return 0;
}
