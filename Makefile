#Programs
SHELL 	= bash
CC     	= g++
CP = cp
MV = mv
RM 	= rm
ECHO	= /bin/echo
PRINTF	= printf
DOXYGEN = doxygen

# Project Paths
PROJECT_ROOT=.
EXTERNAL_ROOT=$(PROJECT_ROOT)/external
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/obj
BINDIR = $(PROJECT_ROOT)/bin
DOCDIR = $(PROJECT_ROOT)/doc
DATDIR = $(PROJECT_ROOT)/data
SPTDIR = $(PROJECT_ROOT)/scripts

# Library Paths
BOX2D_ROOT=$(EXTERNAL_ROOT)
GLUI_ROOT=/usr
GL_ROOT=/usr

#Libraries
LIBS = -lBox2D -lglui -lglut -lGLU -lGL

# Compiler and Linker flags
CPPFLAGS =-g -pg -Wall 
CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include -O3 
LDFLAGS+=-pg -O3 -L $(BOX2D_ROOT)/lib -L $(GLUI_ROOT)/lib

######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJS_WITH_GUI_DISABLED := $(filter-out $(OBJDIR)/main.o, $(OBJS))
OBJS_WITH_GUI_ENABLED := $(filter-out $(OBJDIR)/main_gui_disabled.o, $(OBJS))

.PHONY: install setup build  exe1 doc profile timimg gen_html_timing report exe2 unbuild clean

install: setup build exe1 doc profile timimg gen_html_timing report exe2
	
setup:
	@$(ECHO) "Setting up compilation..."
	@mkdir -p obj
	@mkdir -p bin
	@mkdir -p plots
		
exe1 : create_objects
	@$(CC) -o $(BINDIR)/cs296_exe $(LDFLAGS) $(OBJS_WITH_GUI_DISABLED) $(LIBS)

exe2 : create_objects
	@$(CC) -o $(BINDIR)/cs296_exe $(LDFLAGS) $(OBJS_WITH_GUI_ENABLED) $(LIBS)
	
create_objects : build 
	@$(CC) $(CPPFLAGS) -c $(SRCDIR)/*.cpp
	@$(MV) *.o	$(OBJDIR)
	
doc : exe1
	@$(ECHO) -n "Generating Doxygen Documentation ...  "
	@$(RM) -rf doc/html
	@$(DOXYGEN) $(DOCDIR)/Doxyfile 2 > /dev/null
	@$(ECHO) "Done"
		
build: setup $(EXTERNAL_ROOT)/src/Box2D 
	@$(ECHO) "Building in Release mode...."
	@mkdir -p $(EXTERNAL_ROOT)/src/Box2D/build296
	@cd $(EXTERNAL_ROOT)/src/Box2D/build296/; \
	cmake -DCMAKE_BUILD_TYPE=Release ../
	@cd $(EXTERNAL_ROOT)/src/Box2D/build296/; \
	make
	@cd $(EXTERNAL_ROOT)/src/Box2D/build296/; \
	make install
	@$(ECHO) "Release mode build : Done...."
	
$(EXTERNAL_ROOT)/src/Box2D: 
	@$(ECHO) "Extracting Box2D...."
	@tar -xf ./external/src/Box2D.tar.gz
	@mv Box2D $(EXTERNAL_ROOT)/src/
	@$(ECHO) "Extracted!"


profile: exe1
	@$(BINDIR)/cs296_exe 200000
	@$(ECHO) "Generating .dat file containing profile data"
	@gprof $(BINDIR)/cs296_exe gmon.out >g02_release_prof.dat
	@$(ECHO) "Generating .png file containing call graph"
	@ cp $(SPTDIR)/gprof2dot.py ./
	@gprof $(BINDIR)/cs296_exe gmon.out | ./gprof2dot.py | dot -Tpng -o release.png
	@$(MV) g02_release_prof.dat ./data/
	@$(MV) release.png ./data/
	@ rm gprof2dot.py	
	
timing : exe1 
	@cp $(SPTDIR)/g02_gen_csv.py ./
	@$(ECHO) "Running CSV file script...."
	@python3 g02_gen_csv.py
	@cp $(SPTDIR)/g02_gen_plots.py ./
	@$(ECHO) "Generating the plots...."
	@python3 g02_gen_plots.py
	@ rm *.py	

report: profile timing g02_project_report.dvi
	@$(ECHO) "Converting into pdf...."
	@dvipdf g02_project_report.dvi g02_project_report.pdf
	@$(MV) g02_project_report.pdf $(DOCDIR)
	@$(RM) -rf g02_project_report* *.eps *.bib

g02_project_report.dvi:
	@cp $(DOCDIR)/g02_project_report.tex ./
	@cp $(DOCDIR)/*.bib ./
	@cp $(DOCDIR)/*.eps ./
	@$(ECHO) "Compiling latex file...."
	@latex g02_project_report
	@bibtex g02_project_report
	@latex g02_project_report
	@latex g02_project_report
		
gen_html_timing : timing
	@cp $(SPTDIR)/g02_gen_html.py ./
	@cp $(DOCDIR)/g02_project_report.tex ./
	@$(ECHO) "Running Tex2Html script...."
	@python3 g02_gen_html.py
	@$(MV) g02_timing_report.html ./html
	@$(RM) *.py *.tex
	
	
unbuild:
	@$(ECHO) "Removing the build...."
	@$(RM) -rf $(EXTERNAL_ROOT)/src/Box2D/
	@$(RM) -rf $(EXTERNAL_ROOT)/include/*
	@$(RM) -rf $(EXTERNAL_ROOT)/lib/*	
	@$(ECHO) "Done!"	
	
clean:
	@$(ECHO) "Cleaning up the files...."
	@$(RM) -rf $(OBJDIR) $(BINDIR) $(DATDIR)/* *~ $(SRCDIR)/*~ $(DOCDIR)/*pdf $(DOCDIR)/html *.csv *.sh
	@$(RM) -rf ./plots
	@$(RM) -rf ./scripts/*~
	@$(RM) -f *.out
	@$(ECHO) "Done!"	
