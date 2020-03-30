#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "htslib/hts.h"
#include "htslib/sam.h"
using namespace std;

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << "\t--cram \tSpecify the input cram path"
        << std::endl;
}

std::string check_format(std::string fname) {
    htsFile *fp = hts_open(fname.c_str(), "r");
    const htsFormat *fmt = hts_get_format(fp);
    return std::string(hts_format_description(fmt));
}

// reference: https://rosettacode.org/wiki/Count_occurrences_of_a_tubstring#C.2B.2B
int count_kmer_seq(const std::string& seq, const std::string& kmer)
{
    if (kmer.length() == 0) return 0;
    int count = 0;
    for (size_t offset = seq.find(kmer); offset != std::string::npos;
            offset = seq.find(kmer, offset + kmer.length()))
    {
        ++count;
    }
    return count;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        show_usage(argv[0]);
        return 1;
    }
    std::string cram_fname;
    std::string kmer("gtttgcattcaagtcacagagttgaacattccgtttcatggagcaggttt");
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "--cram")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                cram_fname = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                std::cerr << "--cram option requires one argument." << std::endl;
                return 1;
            }  
        }
    }

    // printf("cram file is %s\n", cram_fname.c_str()); 

    std::string crai_fname(cram_fname + ".crai");

    // cout << check_format(cram_fname) << endl;

    htsFile *fp = hts_open(cram_fname.c_str(), "r");
    hts_idx_t *idx = sam_index_load(fp, crai_fname.c_str());
    bam_hdr_t *hdr = sam_hdr_read(fp);
    hts_itr_t *itr = sam_itr_querys(idx, hdr, ".");
    hts_idx_destroy(idx);

    bam1_t *b = NULL;
    b = bam_init1();
    int r = 0;
    uint8_t *seq = NULL;
    bam1_core_t *c = NULL;
    std::string seq_str("");
    int counts = 0;
    int sequence_count = 0;

    while((r = sam_itr_next(fp, itr, b)) >= 0) {
        c = &b->core;
        seq = bam_get_seq(b);
        seq_str = "";
        for(int i = 0; i < c->l_qseq; ++i) {
            seq_str += std::tolower(seq_nt16_str[bam_seqi(seq, i)]);
        }
        sequence_count = count_kmer_seq(seq_str, kmer);
        counts += sequence_count;
    }

    hts_itr_destroy(itr);
    
    std::cout << counts << endl;

    return 0;
}
