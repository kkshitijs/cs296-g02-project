This project has been done by Group 2 for CS 296.

The members are 

  	-> Kshitij Singh : 110050016
	-> Prithviraj Madhukar Billa : 110050065
	-> Sahil Jindal : 110020043
	
We hereby declare that we have not plagiarised this work from any source, whether offline or online.

The following references were required throughout the project:-

	->Getting to grips with LaTeX, http://www.andy-roberts.net/writing/latex
	->Learning with Python3, http://www.openbookproject.net/thinkcs/python/english3e/
	->Matplotlib, http://www.matplotlib.org/
	->GProf Tutorial, http://www.thegeekstuff.com/2012/08/gprof-tutorial/
	->Box2D Manual, http://www.box2d.org
	->MakeFile Tutorial, http://www.cs.umd.edu/class/fall2002/cmsc214/Tutorial/makefile.html
	->Python Script needed to generate call graph, https://code.google.com/p/jrfonseca/wiki/Gprof2Dot	

For various quickfire solutions, we had access to 
	
	->StackOverflow,  http://www.stackoverflow.com/	

The main project website can be found at

	http://www.cse.iitb.ac.in/~kkshitijs/g02_project.html

The directory structure followed is :

	-> src : same as base-code
	-> external : same as base-code, but b2Timer.cpp and b2Timer.h are the patched versions being used
	-> bin : created while installing the project. the executable, which has GUI enabled, is generated here
	-> html : the project main webpage is here. the timing report is generated here in html after parsing the tex file
	-> doc : contains the images in .png and .eps formats needed for the reports and html pages. Also, the Doxygen commented code is generated here under "html" directory
	-> data : created while installing the project. the profile.dat, the call graphs etc. generated from profiling are here
	-> scripts : the python scripts to generate tex2html, the csv_generator, plot-generator from Matplotlib and gprof2dot.py is here
	-> obj : created while installing the project. stores the object files of the corresponding source files in src.
	-> plots : created while installing the project. stores the timing plots generated by running the python script
