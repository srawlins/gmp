.PHONY : performance
performance: performance.pdf performance.html

performance.latex: performance.md
	pandoc --template manual_template.latex -o performance.latex performance.md

performance.pdf: performance.md
	pandoc --template manual_template.latex -o performance.pdf   performance.md

performance.html: performance.md
	pandoc --mathjax -o performance.html                         performance.md

