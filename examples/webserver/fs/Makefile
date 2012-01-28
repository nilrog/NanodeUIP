OUTDIR = ..
INFILES = $(wildcard *.html) $(wildcard *.shtml) $(wildcard *.txt) $(wildcard *.js)
XXDDIR = xxd
XXDFILES = $(addprefix $(XXDDIR)/, $(addsuffix .cpp, $(INFILES)))
OUTFILES = $(addprefix $(OUTDIR)/, $(addsuffix .cpp, $(subst .,_,$(INFILES))))
ALLFILES = $(OUTDIR)/fs.cpp $(OUTDIR)/files.h $(OUTDIR)/dir.h

all: $(ALLFILES)

clean:
	rm -f $(ALLFILES) $(XXDFILES)

$(XXDDIR)/%.shtml.cpp : %.shtml
	xxd -i $< | gawk -f ../xxd.awk > $@

$(XXDDIR)/%.html.cpp : %.html
	xxd -i $< | gawk -f ../xxd.awk > $@

$(XXDDIR)/%.txt.cpp : %.txt
	xxd -i $< | gawk -f ../xxd.awk > $@

$(XXDDIR)/%.js.cpp : %.js
	xxd -i $< | gawk -f ../xxd.awk > $@

$(OUTDIR)/files.h : $(XXDFILES)
	gawk -f ../files.awk $(XXDFILES) > $@

$(OUTDIR)/dir.h : $(XXDFILES)
	gawk -f ../dir.awk $(XXDFILES) > $@

$(OUTDIR)/fs.cpp : $(XXDFILES)
	cat $(XXDFILES) > $@
