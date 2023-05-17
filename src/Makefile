# qruov1q7L10v740m100       (2023/4/5)

# qruov1q31L3v165m60
# qruov1q31L10v600m70 

# qruov1q127L3v156m54 
# qruov1q127L10v550m60 

# qruov1q7L3v189m72         ((Apr 5 2023) obsolete)
# qruov1q7L10v650m80        ((Apr 5 2023) obsolete)
# qruov1q7L33v2409m99       ((Apr 5 2023) obsolete)
# qruov3q7L3v291m111        ((Apr 5 2023) obsolete)
# qruov3q7L10v1050m130      ((Apr 5 2023) obsolete)
# qruov3q7L40v4640m160      ((Apr 5 2023) obsolete)
# qruov5q7L3v411m162        ((Apr 5 2023) obsolete)
# qruov5q7L10v1450m180      ((Apr 5 2023) obsolete)
# qruov5q7L45v7335m225      ((Apr 5 2023) obsolete)

# qruov1q31L26v1664m78 
# qruov1q127L24v1440m72 

# qruov3q7L10v1100m140      (2023/4/5)
# qruov3q31L3v246m87 
# qruov3q31L10v890m100 
# qruov3q31L39v3783m117 
# qruov3q127L3v228m78 
# qruov3q127L10v830m90 
# qruov3q127L36v3348m108 

# qruov5q7L10v1490m190      (2023/4/5)
# qruov5q31L3v324m114 
# qruov5q31L10v1120m120 
# qruov5q31L37v4699m148 
# qruov5q127L3v306m105 
# qruov5q127L10v1120m120 
# qruov5q127L48v6000m144 

# qruov1q127L10v550m60 qruov3q127L10v830m90 qruov5q127L10v1120m120 ((Apr 26 2023) deplecated)

subdirs := qruov1q7L10v740m100 qruov3q7L10v1100m140 qruov5q7L10v1490m190 qruov1q31L3v165m60  qruov3q31L3v246m87   qruov5q31L3v324m114 qruov1q31L10v600m70 qruov3q31L10v890m100 qruov5q31L10v1120m120 qruov1q127L3v156m54 qruov3q127L3v228m78  qruov5q127L3v306m105

.PHONY: all clean $(subdirs)

all: $(subdirs)

$(subdirs): qruov_config.src ref
	mkdir -p $@/ref
	grep $@ qruov_config.src > $@/ref/qruov_config.txt
	sh -c "cd $@/ref ; ln -s ../../ref/* . || true"
	$(MAKE) -C $@/ref

clean:
	rm -rf $(subdirs)
