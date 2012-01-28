OUTDIR = ..
INFILES = $(wildcard *.html) $(wildcard *.shtml) $(wildcard *.txt) $(wildcard *.js)
XXDFILES = $(addprefix $(OUTDIR)/, $(addsuffix .c, $(INFILES)))
ALLFILES = $(XXDFILES) $(OUTDIR)/files.h $(OUTDIR)/dir.h

all: $(ALLFILES)

clean:
	rm -f $(ALLFILES)

$(OUTDIR)/%.shtml.c : %.shtml
	xxd -i $< | gawk -f ../xxd.awk > $@

$(OUTDIR)/%.html.c : %.html
	xxd -i $< | gawk -f ../xxd.awk > $@

$(OUTDIR)/%.txt.c : %.txt
	xxd -i $< | gawk -f ../xxd.awk > $@

$(OUTDIR)/%.js.c : %.js
	xxd -i $< | gawk -f ../xxd.awk > $@

$(OUTDIR)/files.h : $(XXDFILES)
	gawk -f ../files.awk $(XXDFILES) > $@

$(OUTDIR)/dir.h : $(XXDFILES)
	gawk -f ../dir.awk $(XXDFILES) > $@
