for i in {0..22}
do
    wget ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask/chr${i}.subst.fa.gz
done

# for i in {0..22}
# do
#     gunzip chr${i}.subst.fa.gz
# done