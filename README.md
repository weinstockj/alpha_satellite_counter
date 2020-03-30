This tool counts the number of alpha-satellite repeats in WGS data. It uses a very simple method - substring counting in each read. The substring that we are searching
for (the alpha-satellite dna) is this [50 bp sequence](https://genome.ucsc.edu/cgi-bin/hgc?hgsid=811798019_1qMOdtZQnRn7EeoDYbAdCUvJj733&c=chr2&l=94083101&r=94083151&o=94083101&t=94083151&g=multiz100way&i=multiz100way) . 

The backend to this tool is [htslib](https://github.com/samtools/htslib) .

### Installation
clone this directory, and then run `make` in the cloned directory. In order to compile, your system needs to be able to compile htslib, which requires zlib, among other system libraries. 

