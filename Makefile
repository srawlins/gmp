.PHONY : performance
performance: performance.pdf performance.html

performance.latex: performance.md
	gpp -T manual.md |pandoc --template manual_template.latex -o performance.latex

performance.pdf: performance.md
	gpp -T performance.md |pandoc --template manual_template.latex -o performance.pdf

performance.html: performance.md
	gpp -T -DHTML performance.md |pandoc --mathjax -o performance.html

